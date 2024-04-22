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

					glm::vec3 closestHP = shapes[i]->calcClosestHit(ts, r);
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

				//std::cout << "HITPOINTS "  << resHP.x << ", " << resHP.y << ", " << resHP.z << std::endl;

				//calculate BP coloring for our hitpoint
				glm::vec3 color = shapes[shapeIdx]->calcBP(resHP, lights);
				//std::cout << color.r << ", " << color.g << ", " << color.b << std::endl;

				img.setPixel(x, y, 255 * color.r, 255 * color.g, 255 * color.b);

			}
		}
		//std::cout << imageSize << std::endl;
		img.writeToFile(outputFileName);

	}

};

#endif