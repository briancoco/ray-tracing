#ifndef LIGHT_CPP
#define LIGHT_CPP

#include <glm/glm.hpp>

class Light {
public:
	glm::vec3 position;
	float intensity;
	
	Light(glm::vec3 position, float intensity) : position(position), intensity(intensity) {};
};

#endif