#include <iostream>
#include <string>
#include <vector>
#include <glm/glm.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "Image.h"
#include "Sphere.cpp"
#include "Light.cpp"
#include "Ray.cpp"

// This allows you to skip the `std::` in front of C++ standard library
// functions. You can also say `using std::cout` to be more selective.
// You should never do this in a header file.
using namespace std;

double RANDOM_COLORS[7][3] = {
	{0.0000,    0.4470,    0.7410},
	{0.8500,    0.3250,    0.0980},
	{0.9290,    0.6940,    0.1250},
	{0.4940,    0.1840,    0.5560},
	{0.4660,    0.6740,    0.1880},
	{0.3010,    0.7450,    0.9330},
	{0.6350,    0.0780,    0.1840},
};

int scene;
int imageSize;
string outputFileName;

vector<Ray*> calcRayDirections(glm::vec3 cameraPos, float fov, float aspectRatio, int dimensions) {
	float sideLength = 2 * tan(fov/2);
	float stepSize = sideLength / dimensions;
	float startingPoint = -sideLength / 2;

	vector<Ray*> result;

	for (int row = 0; row < dimensions; row++) {
		float y = startingPoint + (row * stepSize) + (stepSize / 2);
		for (int col = 0; col < dimensions; col++) {
			float x = startingPoint + (col * stepSize) + (stepSize / 2);
			glm::vec3 pixelPos(x, y, cameraPos.z - 1);
			glm::vec3 rayDir = glm::normalize(pixelPos - cameraPos);
			Ray* r = new Ray(rayDir, cameraPos);
			result.push_back(r);
		}

	}
	return result;
}

int main(int argc, char **argv)
{
	if(argc < 2) {
		cout << "Usage: A1 meshfile" << endl;
		return 0;
	}
	scene = stoi(argv[1]);
	imageSize = stoi(argv[2]);
	outputFileName = argv[3];

	glm::vec3 cameraPos(0, 0, 5);
	float fov = glm::radians(45.0);
	float aspectRatio = 1;
	int dimensions = 3;



	//vector<glm::vec3> results = calcRayDirections(cameraPos, fov, aspectRatio, dimensions);
	//SHOULD CREATE A SCENE CLASS

	//Scene setup
	Image img(imageSize, imageSize);

	Light light(glm::vec3(-2.0, 1.0, 1.0), 1.0);

	Sphere redS;
	redS.position = glm::vec3(-0.5, -1.0, 1.0);
	redS.radius = 1;
	redS.diffuse = glm::vec3(1.0, 0.0, 0.0);
	redS.specular = glm::vec3(1.0, 1.0, 0.5);
	redS.ambient = glm::vec3(0.1, 0.1, 0.1);
	redS.exponent = 100.0;

	vector<Ray*> rays = calcRayDirections(cameraPos, fov, aspectRatio, imageSize);
	
	for (size_t i = 0; i < rays.size(); i++) {
		//cout << rays[i]->rayDir.x << ", " << rays[i]->rayDir.y << ", " << rays[i]->rayDir.z << endl;
	}

	for (int y = 0; y < imageSize; y++) {
		for (int x = 0; x < imageSize; x++) {

			//ray corresponding to given pixel
			Ray* r = rays[imageSize * y + x];

			//calculating closest hitpoint
			std::vector<float> ts = redS.calcT(r);
			if (ts.empty()) {
				continue;
			}
			glm::vec3 resHP = redS.calcHit(ts[0], r);
			for (size_t i = 1; i < ts.size(); i++) {
				glm::vec3 hitPoint = redS.calcHit(ts[i], r);
				if (hitPoint.z > resHP.z) {
					resHP = hitPoint;
				}
			}

			//calculate pixel color
			glm::vec3 color = redS.calcBP(resHP, light);
			
			//std::cout << color.r << ", " << color.g << ", " << color.b << endl;

			img.setPixel(x, y, 255 * color.r, 255 * color.g, 255 * color.b);
		}
	}

	//Draw Scene HERE
	
	img.writeToFile(outputFileName);


	return 0;
}
