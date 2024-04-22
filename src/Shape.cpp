#ifndef SHAPE_CPP
#define SHAPE_CPP

#include <vector>
#include "Ray.cpp"
#include <glm/glm.hpp>
#include "Light.cpp"

class Shape {
public:
	virtual std::vector<float> calcT(Ray* r) = 0;
	virtual glm::vec3 calcHit(float t, Ray* r) = 0;
	virtual glm::vec3 calcNorm(glm::vec3 x) = 0;
	virtual glm::vec3 calcBP(glm::vec3 x, std::vector<Light*> lights) = 0;
	virtual glm::vec3 calcClosestHit(std::vector<float> ts, Ray* r) = 0;
};

#endif