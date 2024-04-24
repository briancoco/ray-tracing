#ifndef TRIANGLE_CPP
#define TRIANGLE_CPP
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

class Triangle {
public:

	glm::vec3 a;
	glm::vec3 b;
	glm::vec3 c;

	glm::vec3 na;
	glm::vec3 nb;
	glm::vec3 nc;
};
#endif