#ifndef SPHERE_CPP
#define SPHERE_CPP

#include <glm/glm.hpp>
#include <vector>
#include "./Ray.cpp";
#include "./Light.cpp";

class Sphere {
public:
	glm::vec3 position;
	float radius;
	glm::vec3 rotation;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 ambient;
	float exponent;


	//calculate the t values of hits on the sphere
	std::vector<float> calcT(Ray r) {
		float a = glm::dot(r.rayDir, r.rayDir);
		float b = 2 * glm::dot(r.rayOrigin - position, r.rayDir);
		float c = glm::dot(r.rayOrigin - position, r.rayOrigin - position) - (radius * radius);
		float d = pow(b, 2) - 4 * a * c;

		std::vector<float> result;
		if (d < 0) {
			return {};
		}
		else if (d > 0) {
			return { (-b + sqrt(d)) / (2 * a), (-b - sqrt(d)) / (2 * a) };
		}
		else {
			return { -b / (2 * a)};
		}

	}

	glm::vec3 calcHit(float t, Ray r) {
		return r.rayOrigin + r.rayDir * t;
	}

	glm::vec3 calcNorm(glm::vec3 x) {
		return (x - position) / radius;
	}

	glm::vec3 calcBP(glm::vec3 x, Light light) {
	
	}


};

#endif