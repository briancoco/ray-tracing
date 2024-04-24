#ifndef SPHERE_CPP
#define SPHERE_CPP

#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include "./Ray.cpp";
#include "./Light.cpp";
#include "Shape.cpp"

class Sphere: public Shape {
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
			float t1 = (-b + sqrt(d)) / (2 * a);
			float t2 = (-b - sqrt(d)) / (2 * a);
			if (t1 >= 0) {
				result.push_back(t1);
			}
			if (t2 >= 0) {
				result.push_back(t2);
			}
			
			return result;
		}
		else {
			float t = -b / (2 * a);
			if (t >= 0) {
				return { t };
			}
			return {};
		}

	}

	glm::vec3 calcHit(float t, Ray* r) {
		return r->rayOrigin + r->rayDir * t;
	}

	glm::vec3 calcClosestHit(glm::vec3 origin, std::vector<float> ts, Ray* r) {
		//assuming there are hits
		glm::vec3 res = calcHit(ts[0], r);
		for (size_t i = 1; i < ts.size(); i++) {
			glm::vec3 hp = calcHit(ts[i], r);
			if (glm::length(hp - origin) < glm::length(res - origin)) {
				res = hp;
			}
		}

		return res;
	}

	glm::vec3 calcNorm(glm::vec3 x) {
		return glm::normalize((x - position) / radius);
	}

	glm::vec3 calcBP(glm::vec3 cameraPos, glm::vec3 x, std::vector<Light*> lights) {
		//calculate normal
		//calculate light vec
		//calculate eye vec
		//calculate h vec
		glm::vec3 result(0.0f, 0.0f, 0.0f);
		
		glm::vec3 ca = ambient;
		glm::vec3 n = calcNorm(x);
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