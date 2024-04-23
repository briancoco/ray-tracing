#ifndef PLANE_CPP
#define PLANE_CPP

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


class Plane: public Shape {
public:
	glm::vec3 planeNorm;
	glm::vec3 planePoint;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 ambient;
	float exponent;

	std::vector<float> calcT(Ray* r) {
		float t = (glm::dot(planeNorm, planePoint) - glm::dot(planeNorm, r->rayOrigin)) / glm::dot(planeNorm, r->rayDir);
		if (t >= 0) {
			return { t };
		}
		return {};
	}

	glm::vec3 calcHit(float t, Ray* r) {
		return r->rayOrigin + r->rayDir * t;
	}

	glm::vec3 calcNorm(glm::vec3 x) {
		return planeNorm;
	}

	glm::vec3 calcBP(glm::vec3 x, std::vector<Light*> lights) {
		glm::vec3 result(0.0f, 0.0f, 0.0f);

		glm::vec3 ca = ambient;
		glm::vec3 n = planeNorm;
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
		return calcHit(ts[0], r);
	}
};

#endif