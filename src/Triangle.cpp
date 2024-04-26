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

	float* Triangle::getBoundingBox() {
		float xmin = a.x;
		float xmax = a.x;
		float ymin = a.y;
		float ymax = a.y;
		float zmin = a.z;
		float zmax = a.z;

		//xmin
		if (b.x < xmin) {
			xmin = b.x;
		}
		if (c.x < xmin) {
			xmin = c.x;
		}

		//xmax
		if (b.x > xmax) {
			xmax = b.x;
		}
		if (c.x > xmax) {
			xmax = c.x;
		}

		//ymin
		if (b.y < ymin) {
			ymin = b.y;
		}
		if (c.y < ymin) {
			ymin = c.y;
		}

		//ymax
		if (b.y > ymax) {
			ymax = b.y;
		}
		if (c.y > ymax) {
			ymax = c.y;
		}

		//zmin
		if (b.z < zmin) {
			zmin = b.z;
		}
		if (c.z < zmin) {
			zmin = c.z;
		}

		//zmax
		if (b.z > zmax) {
			zmax = b.z;
		}
		if (c.z > zmax) {
			zmax = c.z;
		}


		float boundingBox[] = { xmin, xmax, ymin, ymax, zmin, zmax };

		return boundingBox;
	};
};
#endif