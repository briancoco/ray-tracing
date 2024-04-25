#ifndef OBJSHAPE_CPP
#define OBJSHAPE_CPP
#include <iostream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "tiny_obj_loader.h"
#include "Image.h"
#include "Sphere.cpp"
#include "Light.cpp"
#include "Ray.cpp"
#include "Shape.cpp"
#include <string>
#include "Triangle.cpp"

#include <math.h>


#define EPSILON 0.000001
#define CROSS(dest,v1,v2) \
          dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
          dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
          dest[2]=v1[0]*v2[1]-v1[1]*v2[0];
#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])
#define SUB(dest,v1,v2) \
          dest[0]=v1[0]-v2[0]; \
          dest[1]=v1[1]-v2[1]; \
          dest[2]=v1[2]-v2[2];

/* the original jgt code */


class ObjShape{
public:

    std::vector<Triangle*> triangles;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 ambient;
    float exponent;
    bool transform = false;

    int intersect_triangle(double orig[3], double dir[3],
        double vert0[3], double vert1[3], double vert2[3],
        double* t, double* u, double* v)
    {
        double edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
        double det, inv_det;

        /* find vectors for two edges sharing vert0 */
        SUB(edge1, vert1, vert0);
        SUB(edge2, vert2, vert0);

        /* begin calculating determinant - also used to calculate U parameter */
        CROSS(pvec, dir, edge2);

        /* if determinant is near zero, ray lies in plane of triangle */
        det = DOT(edge1, pvec);

        if (det > -EPSILON && det < EPSILON)
            return 0;
        inv_det = 1.0 / det;

        /* calculate distance from vert0 to ray origin */
        SUB(tvec, orig, vert0);

        /* calculate U parameter and test bounds */
        *u = DOT(tvec, pvec) * inv_det;
        if (*u < 0.0 || *u > 1.0)
            return 0;

        /* prepare to test V parameter */
        CROSS(qvec, tvec, edge1);

        /* calculate V parameter and test bounds */
        *v = DOT(dir, qvec) * inv_det;
        if (*v < 0.0 || *u + *v > 1.0)
            return 0;

        /* calculate t, ray intersects triangle */
        *t = DOT(edge2, qvec) * inv_det;

        return 1;
    }

    bool hit(Ray* r, glm::vec3& resHP, glm::vec3& resN) {
        //traverse thru triangles that make up our obj file
        //for each triangle, use the provided fn to calculate intersections
        //if intersection exists, determine if this is the closest intersection
        //if yes, save as curr best
        //in the end return the closest triangle we hit?


        //to calculate the normal we need to barycentric coords and the normals
        //for all of our verticies
        //glm::vec3 resHP;
        //glm::vec3 resN;
        bool first = true;
        glm::vec3 origin(0, 0, 5);

        for (size_t i = 0; i < triangles.size(); i++) {
            double t;
            double ba;
            double bb;
            double bc;

            double orig[] = { r->rayOrigin.x, r->rayOrigin.y, r->rayOrigin.z };
            double dir[] = { r->rayDir.x, r->rayDir.y, r->rayDir.z };
            double vec0[] = { triangles[i]->a.x, triangles[i]->a.y, triangles[i]->a.z };
            double vec1[] = { triangles[i]->b.x, triangles[i]->b.y, triangles[i]->b.z };
            double vec2[] = { triangles[i]->c.x, triangles[i]->c.y, triangles[i]->c.z };

            if (intersect_triangle(orig, dir, vec0, vec1, vec2, &t, &bb, &bc)) {
                ba = 1 - bb - bc;
                
                glm::vec3 hitPoint = triangles[i]->a * (float)ba + triangles[i]->b * (float)bb + triangles[i]->c * (float)bc;

                if (t >= 0 && (first || glm::length(hitPoint - origin) < glm::length(resHP - origin))) {
                    
                   

                    resHP = hitPoint;
                    resN = triangles[i]->na * (float)ba + triangles[i]->nb * (float)bb + triangles[i]->nc * (float)bc;
                    resN = glm::normalize(resN);

                    first = false;
                }
            }

            

        }

        return !first;

    }
   
    glm::vec3 calcBP(glm::vec3 cameraPos, glm::vec3 x, glm::vec3 n, std::vector<Light*> lights) {
        glm::vec3 result(0.0f, 0.0f, 0.0f);

        glm::vec3 ca = ambient;
        for (size_t i = 0; i < lights.size(); i++) {
            glm::vec3 l = glm::normalize(lights[i]->position - x);
            glm::vec3 e = glm::normalize(cameraPos - x);
            glm::vec3 h = glm::normalize(l + e);

            glm::vec3 cd = diffuse * glm::max(0.0f, glm::dot(l, n));
            glm::vec3 cs = specular * glm::max(0.0f, pow(glm::dot(h, n), exponent));

            glm::vec3 color = lights[i]->intensity * (cd + cs);

            result += color;

        }
        result += ca;
        return glm::clamp(result, 0.0f, 1.0f);
    }

};

#endif