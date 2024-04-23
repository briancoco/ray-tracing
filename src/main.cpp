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
#include "Scene.cpp"
#include "Plane.cpp"
#include "Ellipsoid.cpp"

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

	std::vector<Shape*> shapes;
	std::vector<Light*> lights;
	vector<Ray*> rays = calcRayDirections(cameraPos, fov, aspectRatio, imageSize);

	if (scene == 1 || scene == 2) {

		Light* light = new Light(glm::vec3(-2.0, 1.0, 1.0), 1.0);
		lights.push_back(light);

		Sphere* redS = new Sphere();
		redS->position = glm::vec3(-0.5, -1.0, 1.0);
		redS->radius = 1;
		redS->diffuse = glm::vec3(1.0, 0.0, 0.0);
		redS->specular = glm::vec3(1.0, 1.0, 0.5);
		redS->ambient = glm::vec3(0.1, 0.1, 0.1);
		redS->exponent = 100.0;

		Sphere* greenS = new Sphere();
		greenS->position = glm::vec3(0.5, -1.0, -1.0);
		greenS->radius = 1;
		greenS->diffuse = glm::vec3(0.0, 1.0, 0.0);
		greenS->specular = glm::vec3(1.0, 1.0, 0.5);
		greenS->ambient = glm::vec3(0.1, 0.1, 0.1);
		greenS->exponent = 100.0;

		Sphere* blueS = new Sphere();
		blueS->position = glm::vec3(0.0, 1.0, 0.0);
		blueS->radius = 1;
		blueS->diffuse = glm::vec3(0.0, 0.0, 1.0);
		blueS->specular = glm::vec3(1.0, 1.0, 0.5);
		blueS->ambient = glm::vec3(0.1, 0.1, 0.1);
		blueS->exponent = 100.0;


		shapes.push_back(redS);
		shapes.push_back(greenS);
		shapes.push_back(blueS);

		Scene scene(shapes, lights, rays, imageSize, outputFileName);

		scene.draw();
	}
	else if (scene == 3) {
		Light* l1 = new Light(glm::vec3(1.0, 2.0, 2.0), 0.5);
		Light* l2 = new Light(glm::vec3(-1.0, 2.0, -1.0), 0.5);

		lights.push_back(l1);
		lights.push_back(l2);

		Plane* plane = new Plane();
		plane->planePoint = glm::vec3(0.0, -1.0, 0.0);
		plane->planeNorm = glm::normalize(glm::vec3(0.0, 1.0, 0.0));
		plane->diffuse = glm::vec3(1.0, 1.0, 1.0);
		plane->specular = glm::vec3(0.0, 0.0, 0.0);
		plane->ambient = glm::vec3(0.1, 0.1, 0.1);
		plane->exponent = 0.0;

		Sphere* greenS = new Sphere();
		greenS->position = glm::vec3(-0.5, 0.0, -0.5);
		greenS->radius = 1;
		greenS->diffuse = glm::vec3(0.0, 1.0, 0.0);
		greenS->specular = glm::vec3(1.0, 1.0, 0.5);
		greenS->ambient = glm::vec3(0.1, 0.1, 0.1);
		greenS->exponent = 100.0;

		Ellipsoid* redE = new Ellipsoid();
		redE->position = glm::vec3(0.5, 0.0, 0.5);
		redE->scale = glm::vec3(0.5, 0.6, 0.2);
		redE->diffuse = glm::vec3(1.0, 0.0, 0.0);
		redE->specular = glm::vec3(1.0, 1.0, 0.5);
		redE->ambient = glm::vec3(0.1, 0.1, 0.1);
		redE->exponent = 100.0;
		
		shapes.push_back(plane);
		shapes.push_back(greenS);
		shapes.push_back(redE);


		Scene scene(shapes, lights, rays, imageSize, outputFileName);

		scene.draw();
	}

	

	


	return 0;
}
