#ifndef ELLIPSOID_CPP
#define ELLIPSOID_CPP

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

class Ellipsoid: public Shape {
public:
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 ambient;
	float exponent;

	//FOR ELLIPSOID IT MIGHT BE EASIER TO ASSUME INPUTS ARE GIVEN AS THE SPHERE

	glm::mat4 calcTransformMat() {
		glm::mat4 scaleMat = glm::mat4(1.0f);
		glm::mat4 translateMat = glm::mat4(1.0f);
		glm::mat4 ellipsoid_E = glm::mat4(
			scale.x, 0, 0, 0,
			0, scale.y, 0, 0,
			0, 0, scale.z, 0,
			position.x, position.y, position.z, 1
		);


		
		scaleMat[0][0] = scale.x;
		scaleMat[1][1] = scale.y;
		scaleMat[2][2] = scale.z;

		translateMat[0][3] = position.x;
		translateMat[1][3] = position.y;
		translateMat[2][3] = position.z;

		return ellipsoid_E;
	}

	std::vector<float> calcT(Ray* r) {
		//convert ray into sphere space
		//calculate hitpoints in sphere space
		//convert hitpoints back into elip space
		//calculate and return t values
		glm::mat4 transformMat = calcTransformMat();
		glm::mat4 inverseTransformMat = glm::inverse(transformMat);
		glm::vec3 sphereRayDir = inverseTransformMat * glm::vec4(r->rayDir, 0.0f);
		sphereRayDir = glm::normalize(sphereRayDir);
		glm::vec3 sphereRayOrigin = inverseTransformMat * glm::vec4(r->rayOrigin, 1.0f);
		
		Ray sr(sphereRayDir, sphereRayOrigin);
		Sphere s;
		s.position = glm::vec3(0.0, 0.0, 0.0);
		s.radius = 1;
		std::vector<float> ts = s.calcT(&sr);

		std::vector<float> resTS;
		for (size_t i = 0; i < ts.size(); i++) {
			glm::vec3 sHit = s.calcHit(ts[i], &sr);
			glm::vec3 hit = transformMat * glm::vec4(sHit, 1.0f);
			glm::vec3 xp = hit - r->rayOrigin;
			float t = glm::length(xp);
			t = glm::dot(r->rayDir, xp) < 0 ? -t : t;
			if (t < 0) {
				continue;
			}
			resTS.push_back(t);
		}

		return resTS;
		 
	}
	glm::vec3 calcHit(float t, Ray* r) {
		//takes in t values and computes hitpoints on the ray
		return r->rayOrigin + r->rayDir * t;
	}
	glm::vec3 calcNorm(glm::vec3 x) {
		//convert hitpoint back into sphere space
		//compute normal
		//convert normal into elip space
		//return the normal
		glm::mat4 transformMat = calcTransformMat();
		glm::mat4 inverseTransformMat = glm::inverse(transformMat);
		
		glm::vec3 sx = inverseTransformMat * glm::vec4(x, 1.0f);
		glm::vec3 sn = glm::normalize((sx - glm::vec3(0.0f, 0.0f, 0.0f) / 1.0f));

		glm::vec3 n = glm::transpose(inverseTransformMat) * glm::vec4(sn, 0.0f);
		n = glm::normalize(n);

		return n;

	}
	glm::vec3 calcBP(glm::vec3 x, std::vector<Light*> lights) {
		glm::vec3 result(0.0f, 0.0f, 0.0f);

		glm::vec3 ca = ambient;
		glm::vec3 n = calcNorm(x);
		for (size_t i = 0; i < lights.size(); i++) {
			glm::vec3 l = glm::normalize(lights[i]->position - x);
			glm::vec3 e = glm::normalize(glm::vec3(0, 0, 5) - x);
			glm::vec3 h = glm::normalize(l + e);

			glm::vec3 cd = diffuse * glm::max(0.0f, glm::dot(l, n));
			glm::vec3 cs = specular * glm::max(0.0f, pow(glm::dot(h, n), exponent));

			glm::vec3 color = lights[i]->intensity * (cd + cs);

			result += color;

		}
		result += ca;
		return glm::clamp(result, 0.0f, 1.0f);
	}
	glm::vec3 calcClosestHit(std::vector<float> ts, Ray* r) {
		glm::vec3 res = calcHit(ts[0], r);
		for (size_t i = 1; i < ts.size(); i++) {
			glm::vec3 hp = calcHit(ts[i], r);
			if (hp.z > res.z) {
				res = hp;
			}
		}

		return res;
	}
};


#endif