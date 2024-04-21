#ifndef RAY_CPP
#define RAY_CPP

#include <glm/glm.hpp>

class Ray {
public:
	glm::vec3 rayDir;
	glm::vec3 rayOrigin;

	Ray(glm::vec3 rayDir, glm::vec3 rayOrigin) : rayDir(rayDir), rayOrigin(rayOrigin) {};
};

#endif