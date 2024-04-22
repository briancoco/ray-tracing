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
	std::vector<float> calcT(Ray* r) {
		float a = glm::dot(r->rayDir, r->rayDir);
		float b = 2 * glm::dot(r->rayOrigin - position, r->rayDir);
		float c = glm::dot(r->rayOrigin - position, r->rayOrigin - position) - (radius * radius);
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

	glm::vec3 calcHit(float t, Ray* r) {
		return r->rayOrigin + r->rayDir * t;
	}

	glm::vec3 calcNorm(glm::vec3 x) {
		return glm::normalize((x - position) / radius);
	}

	glm::vec3 calcBP(glm::vec3 x, Light light) {
		//calculate normal
		//calculate light vec
		//calculate eye vec
		//calculate h vec

		glm::vec3 n = calcNorm(x);
		glm::vec3 l = glm::normalize(light.position - position);
		glm::vec3 e = glm::normalize(glm::vec3(0, 0, 5) - position);
		glm::vec3 h = glm::normalize(l + e);

		glm::vec3 ca = ambient;
		glm::vec3 cd = diffuse * glm::max(0.0f, glm::dot(l, n));
		glm::vec3 cs = specular * glm::max(0.0f, pow(glm::dot(h, n), exponent));

		glm::vec3 color = ca + light.intensity * (cd + cs);

		return glm::clamp(color, 0.0f, 1.0f);


	}


};

#endif