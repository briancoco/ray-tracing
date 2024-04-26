#ifndef SCENE_CPP
#define SCENE_CPP

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

class Scene {
public:
	std::vector<Shape*> shapes;
	std::vector<Light*> lights;
	std::vector<Ray*> rays;
	int imageSize;
	std::string outputFileName;
	glm::vec3 cameraPos = glm::vec3(0, 0, 5);
	
	Scene(std::vector<Shape*> shapes, std::vector<Light*> lights, std::vector<Ray*> rays, int imageSize, std::string outputFileName): shapes(shapes), lights(lights), rays(rays), imageSize(imageSize), outputFileName(outputFileName) {}



	void draw() {
		//draw the scene
		//for each pixel in our scene we want to calculate the hitpoints of that pixel with the objects in our scnee
		//the hitpoint that is closest to the camera will be the one shown
		// for the closest HP, we're going to want to calculate the blnn-phong coloring
		// and set the color for the given pixel
		//in the end write the image to file

		Image img(imageSize, imageSize);

		for (int y = 0; y < imageSize; y++) {
			for (int x = 0; x < imageSize; x++) {
				int shapeIdx = -1;
				glm::vec3 resHP;
				Ray* r = rays[y * imageSize + x];

				//traverse shapes in scene find the closest hitpoint
				for (size_t i = 0; i < shapes.size(); i++) {
					std::vector<float> ts = shapes[i]->calcT(r);
					if (ts.empty()) {
						continue;
					}
					

					glm::vec3 closestHP = shapes[i]->calcClosestHit(cameraPos, ts, r);
					if (shapeIdx == -1 || closestHP.z > resHP.z) {
						resHP = closestHP;
						shapeIdx = i;
					}
				}

				//if no hits then dont color pixel
				if (shapeIdx == -1) {
					//std::cout << "NO HITPOINTS" << std::endl;
					continue;
				}
				
				//calculate shadow rays for lights and create a filtered list of lights
				//that affect the BP for this hitpoint
				std::vector<Light*> filteredLights = Shape::calcVisibleLights(resHP, lights, shapes);

				//calculate BP coloring for our hitpoint
				glm::vec3 color = shapes[shapeIdx]->isReflective ? shapes[shapeIdx]->calcRefl(resHP, r, shapes, lights) : shapes[shapeIdx]->calcBP(cameraPos, resHP, filteredLights);
				
				img.setPixel(x, y, 255 * color.r, 255 * color.g, 255 * color.b);

			}
		}
		//std::cout << imageSize << std::endl;
		img.writeToFile(outputFileName);

	}

};

#endif