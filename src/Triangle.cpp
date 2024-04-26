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

	float* Triangle::getBoundingBox(glm::mat4 transformMat = glm::mat4(1.0f)) {
		glm::vec3 ta = transformMat * glm::vec4(a, 1.0f);
		glm::vec3 tb = transformMat * glm::vec4(b, 1.0f);
		glm::vec3 tc = transformMat * glm::vec4(c, 1.0f);

		float xmin = ta.x;
		float xmax = ta.x;
		float ymin = tb.y;
		float ymax = tb.y;
		float zmin = tc.z;
		float zmax = tc.z;

		//xmin
		if (tb.x < xmin) {
			xmin = tb.x;
		}
		if (tc.x < xmin) {
			xmin = tc.x;
		}

		//xmax
		if (tb.x > xmax) {
			xmax = tb.x;
		}
		if (tc.x > xmax) {
			xmax = tc.x;
		}

		//ymin
		if (tb.y < ymin) {
			ymin = tb.y;
		}
		if (tc.y < ymin) {
			ymin = tc.y;
		}

		//ymax
		if (tb.y > ymax) {
			ymax = tb.y;
		}
		if (tc.y > ymax) {
			ymax = tc.y;
		}

		//zmin
		if (tb.z < zmin) {
			zmin = tb.z;
		}
		if (tc.z < zmin) {
			zmin = tc.z;
		}

		//zmax
		if (tb.z > zmax) {
			zmax = tb.z;
		}
		if (tc.z > zmax) {
			zmax = tc.z;
		}


		float boundingBox[] = { xmin, xmax, ymin, ymax, zmin, zmax };

		return boundingBox;
	};
};
#endif