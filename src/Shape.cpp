#ifndef SHAPE_CPP
#define SHAPE_CPP

#include <vector>
#include "Ray.cpp"
#include <glm/glm.hpp>
#include "Light.cpp"

class Shape {
public:
	bool isReflective = false;
	virtual std::vector<float> calcT(Ray* r) = 0;
	virtual glm::vec3 calcHit(float t, Ray* r) = 0;
	virtual glm::vec3 calcNorm(glm::vec3 x) = 0;
	virtual glm::vec3 calcBP(glm::vec3 cameraPos, glm::vec3 x, std::vector<Light*> lights) = 0;
	virtual glm::vec3 calcClosestHit(glm::vec3 origin, std::vector<float> ts, Ray* r) = 0;

	glm::vec3 calcRefl(glm::vec3 x, Ray* r, std::vector<Shape*> shapes, std::vector<Light*> lights) {
		//calculate reflection ray
		//create ray object
		//traverse and find closest hitpoint for the reflection ray
		//find the color of the given hitpoint
		//return that color

		glm::vec3 n = calcNorm(x);
		glm::vec3 reflDir = glm::reflect(r->rayDir, n);
		reflDir = glm::normalize(reflDir);
		glm::vec3 offset = reflDir * 0.01f;
		Ray reflRay(reflDir, x + offset);

		int shapeIdx = -1;
		int resT = -1;
		glm::vec3 resVec;

		for (size_t i = 0; i < shapes.size(); i++) {
			std::vector<float> ts = shapes[i]->calcT(&reflRay);
			if (ts.empty()) {
				continue;
			}
			glm::vec3 closestHP = shapes[i]->calcClosestHit(reflRay.rayOrigin, ts, &reflRay);
			if (shapeIdx == -1 || glm::length(closestHP - reflRay.rayOrigin) < glm::length(resVec - reflRay.rayOrigin)) {
				resVec = closestHP;
				shapeIdx = i;
			}


			
		}

		if (shapeIdx == -1 || shapes[shapeIdx]->isReflective) {
			return glm::vec3(0, 0, 0);
		}

		glm::vec3 resHP = shapes[shapeIdx]->calcHit(resT, &reflRay);
		resHP = resVec;

		std::vector<Light*> filteredLights;
		for (size_t i = 0; i < lights.size(); i++) {
			glm::vec3 rayDir = glm::normalize(lights[i]->position - resHP);
			glm::vec3 offset = rayDir * 0.1f;
			Ray* shadowRay = new Ray(glm::normalize(lights[i]->position - resHP), resHP + offset);
			//traverse thru shapes in scene, if shadowRay hits any dont include light in filtered light list
			bool inShadow = false;
			for (size_t j = 0; j < shapes.size(); j++) {
				//if (j == shapeIdx) continue;
				std::vector<float> ts = shapes[j]->calcT(shadowRay);
				if (!ts.empty()) {
					for (size_t z = 0; z < ts.size(); z++) {
						if (ts[z] >= 0 && glm::length(lights[i]->position - shadowRay->rayOrigin) > glm::length(shapes[j]->calcHit(ts[z], shadowRay) - shadowRay->rayOrigin)) {
							inShadow = true;
							break;
						}
					}
					if (inShadow) {
						break;
					}
				}
			}
			if (!inShadow) {
				filteredLights.push_back(lights[i]);
			}
		}

		glm::vec3 color = shapes[shapeIdx]->calcBP(x, resHP, filteredLights);

		return color;
	}

};

#endif