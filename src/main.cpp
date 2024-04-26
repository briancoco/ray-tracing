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
#include "Triangle.cpp"
#include "ObjShape.cpp"

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
	else if (scene == 4 || scene == 5) {
		Light* l1 = new Light(glm::vec3(-1.0, 2.0, 1.0), 0.5);
		Light* l2 = new Light(glm::vec3(0.5, -0.5, 0.0), 0.5);

		lights.push_back(l1);
		lights.push_back(l2);

		Plane* plane = new Plane();
		plane->planePoint = glm::vec3(0.0, -1.0, 0.0);
		plane->planeNorm = glm::normalize(glm::vec3(0.0, 1.0, 0.0));
		plane->diffuse = glm::vec3(1.0, 1.0, 1.0);
		plane->specular = glm::vec3(0.0, 0.0, 0.0);
		plane->ambient = glm::vec3(0.1, 0.1, 0.1);
		plane->exponent = 0.0;

		Plane* backPlane = new Plane();
		backPlane->planePoint = glm::vec3(0.0, 0.0, -3.0);
		backPlane->planeNorm = glm::normalize(glm::vec3(0.0, 0.0, 1.0));
		backPlane->diffuse = glm::vec3(1.0, 1.0, 1.0);
		backPlane->specular = glm::vec3(0.0, 0.0, 0.0);
		backPlane->ambient = glm::vec3(0.1, 0.1, 0.1);
		backPlane->exponent = 0.0;

		Sphere* redS = new Sphere();
		redS->position = glm::vec3(0.5, -0.7, 0.5);
		redS->radius = 0.3;
		redS->diffuse = glm::vec3(1.0, 0.0, 0.0);
		redS->specular = glm::vec3(1.0, 1.0, 0.5);
		redS->ambient = glm::vec3(0.1, 0.1, 0.1);
		redS->exponent = 100.0;

		Sphere* blueS = new Sphere();
		blueS->position = glm::vec3(1.0, -0.7, 0.0);
		blueS->radius = 0.3;
		blueS->diffuse = glm::vec3(0.0, 0.0, 1.0);
		blueS->specular = glm::vec3(1.0, 1.0, 0.5);
		blueS->ambient = glm::vec3(0.1, 0.1, 0.1);
		blueS->exponent = 100.0;

		Sphere* refS1 = new Sphere();
		refS1->position = glm::vec3(-0.5, 0.0, -0.5);
		refS1->radius = 1;
		refS1->diffuse = glm::vec3(1.0, 0.0, 0.0);
		refS1->specular = glm::vec3(1.0, 1.0, 0.5);
		refS1->ambient = glm::vec3(0.1, 0.1, 0.1);
		refS1->exponent = 100.0;
		refS1->isReflective = true;

		Sphere* refS2 = new Sphere();
		refS2->position = glm::vec3(1.5, 0.0, -1.5);
		refS2->radius = 1;
		refS2->diffuse = glm::vec3(0.0, 0.0, 1.0);
		refS2->specular = glm::vec3(1.0, 1.0, 0.5);
		refS2->ambient = glm::vec3(0.1, 0.1, 0.1);
		refS2->exponent = 100.0;
		refS2->isReflective = true;

		shapes.push_back(plane);
		shapes.push_back(backPlane);
		shapes.push_back(redS);
		shapes.push_back(blueS);
		shapes.push_back(refS1);
		shapes.push_back(refS2);

		Scene scene(shapes, lights, rays, imageSize, outputFileName);

		scene.draw();

	}
	else if (scene == 6) {
		// Load geometry
		vector<float> posBuf; // list of vertex positions
		vector<float> norBuf; // list of vertex normals
		vector<float> texBuf; // list of vertex texture coords
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		string errStr;
		string meshName = "../resources/bunny.obj";
		bool rc = tinyobj::LoadObj(&attrib, &shapes, &materials, &errStr, meshName.c_str());
		if (!rc) {
			cerr << errStr << endl;
		}
		else {
			// Some OBJ files have different indices for vertex positions, normals,
			// and texture coordinates. For example, a cube corner vertex may have
			// three different normals. Here, we are going to duplicate all such
			// vertices.
			// Loop over shapes
			for (size_t s = 0; s < shapes.size(); s++) {
				// Loop over faces (polygons)
				size_t index_offset = 0;
				for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
					size_t fv = shapes[s].mesh.num_face_vertices[f];
					// Loop over vertices in the face.
					for (size_t v = 0; v < fv; v++) {
						// access to vertex
						tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
						posBuf.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
						posBuf.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
						posBuf.push_back(attrib.vertices[3 * idx.vertex_index + 2]);
						if (!attrib.normals.empty()) {
							norBuf.push_back(attrib.normals[3 * idx.normal_index + 0]);
							norBuf.push_back(attrib.normals[3 * idx.normal_index + 1]);
							norBuf.push_back(attrib.normals[3 * idx.normal_index + 2]);
						}
						if (!attrib.texcoords.empty()) {
							texBuf.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
							texBuf.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
						}
					}
					index_offset += fv;
					// per-face material (IGNORE)
					shapes[s].mesh.material_ids[f];
				}
			}

		}

		cout << "Number of vertices: " << posBuf.size() / 3 << endl;

		//construct triangles vector
		std::vector<Triangle*> triangles;
		int numTriangles = posBuf.size() / 9;

		//traverse thru position and normal buffers constructing triangle objects for each triangle
		for (int i = 0; i < numTriangles; i++) {
			Triangle* t = new Triangle();
			t->a = glm::vec3(posBuf[9 * i + 0], posBuf[9 * i + 1], posBuf[9 * i + 2]);
			t->na = glm::vec3(norBuf[9 * i + 0], norBuf[9 * i + 1], norBuf[9 * i + 2]);
			t->b = glm::vec3(posBuf[9 * i + 3], posBuf[9 * i + 4], posBuf[9 * i + 5]);
			t->nb = glm::vec3(norBuf[9 * i + 3], norBuf[9 * i + 4], norBuf[9 * i + 5]);
			t->c = glm::vec3(posBuf[9 * i + 6], posBuf[9 * i + 7], posBuf[9 * i + 8]);
			t->nc = glm::vec3(norBuf[9 * i + 6], norBuf[9 * i + 7], norBuf[9 * i + 8]);
		
			triangles.push_back(t);

		}

		//calculating mesh's bounding box
		float xmin = posBuf[0];
		float xmax = posBuf[0];
		float ymin = posBuf[1];
		float ymax = posBuf[1];
		float zmin = posBuf[2];
		float zmax = posBuf[2];

		for (size_t i = 0; i < triangles.size(); i++) {
			//get the bounding box for the given triangle
			//compare it's values to see if they're smaller than the global
			// if yes replace global

			float* boundingBox = triangles[i]->getBoundingBox();
			float txmin = boundingBox[0];
			float txmax = boundingBox[1];
			float tymin = boundingBox[2];
			float tymax = boundingBox[3];
			float tzmin = boundingBox[4];
			float tzmax = boundingBox[5];

			if (txmin < xmin) {
				xmin = txmin;
			}
			if (txmax > xmax) {
				xmax = txmax;
			}
			if (tymin < ymin) {
				ymin = tymin;
			}
			if (tymax > ymax) {
				ymax = tymax;
			}
			if (tzmin < zmin) {
				zmin = tzmin;
			}
			if (tzmax > zmax) {
				zmax = tzmax;
			}

		}

		ObjShape* bunny = new ObjShape();
		bunny->triangles = triangles;
		bunny->diffuse = glm::vec3(0.0, 0.0, 1.0);
		bunny->specular = glm::vec3(1.0, 1.0, 0.5);
		bunny->ambient = glm::vec3(0.1, 0.1, 0.1);
		bunny->exponent = 100.0;
		bunny->transform = false;

		glm::vec3 boundingSpherePos = glm::vec3((xmin + xmax) / 2, (ymin + ymax) / 2, (zmin + zmax) / 2);
		float radius = -1;
		
		for (size_t i = 0; i < triangles.size(); i++) {
			//for the verticies that make up the triangle
			//calculate the distance from the bounding sphere position to the vertex
			//if that distance is greater than the current radius set it as new radius
			float ra = glm::length(triangles[i]->a - boundingSpherePos);
			float rb = glm::length(triangles[i]->b - boundingSpherePos);
			float rc = glm::length(triangles[i]->c - boundingSpherePos);
			if (ra > radius) {
				radius = ra;
			}
			if (rb > radius) {
				radius = rb;
			}
			if (rc > radius) {
				radius = rc;
			}
		}

		Sphere* boundingSphere = new Sphere();
		boundingSphere->position = boundingSpherePos;
		boundingSphere->radius = radius;


		Light* l = new Light(scene == 6 ? glm::vec3(-1.0, 1.0, 1.0) : glm::vec3(1.0, 1.0, 2.0), 1.0);
		lights.push_back(l);

		Image img(imageSize, imageSize);
		for (int y = 0; y < imageSize; y++) {
			for (int x = 0; x < imageSize; x++) {
				glm::vec3 resHP;
				glm::vec3 resN;
				Ray* r = rays[y * imageSize + x];
				std::vector<float> ts = boundingSphere->calcT(r);
				if (ts.empty() || !bunny->hit(r, resHP, resN)) {
					continue;
				}
				glm::vec3 color = bunny->calcBP(cameraPos, resHP, resN, lights);

				img.setPixel(x, y, 255 * color.r, 255 * color.g, 255 * color.b);
			}
		}

		img.writeToFile(outputFileName);

	}
	else if (scene == 7) {
		// Load geometry
		vector<float> posBuf; // list of vertex positions
		vector<float> norBuf; // list of vertex normals
		vector<float> texBuf; // list of vertex texture coords
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		string errStr;
		string meshName = "../resources/bunny.obj";
		bool rc = tinyobj::LoadObj(&attrib, &shapes, &materials, &errStr, meshName.c_str());
		if (!rc) {
			cerr << errStr << endl;
		}
		else {
			// Some OBJ files have different indices for vertex positions, normals,
			// and texture coordinates. For example, a cube corner vertex may have
			// three different normals. Here, we are going to duplicate all such
			// vertices.
			// Loop over shapes
			for (size_t s = 0; s < shapes.size(); s++) {
				// Loop over faces (polygons)
				size_t index_offset = 0;
				for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
					size_t fv = shapes[s].mesh.num_face_vertices[f];
					// Loop over vertices in the face.
					for (size_t v = 0; v < fv; v++) {
						// access to vertex
						tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
						posBuf.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
						posBuf.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
						posBuf.push_back(attrib.vertices[3 * idx.vertex_index + 2]);
						if (!attrib.normals.empty()) {
							norBuf.push_back(attrib.normals[3 * idx.normal_index + 0]);
							norBuf.push_back(attrib.normals[3 * idx.normal_index + 1]);
							norBuf.push_back(attrib.normals[3 * idx.normal_index + 2]);
						}
						if (!attrib.texcoords.empty()) {
							texBuf.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
							texBuf.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
						}
					}
					index_offset += fv;
					// per-face material (IGNORE)
					shapes[s].mesh.material_ids[f];
				}
			}

		}

		cout << "Number of vertices: " << posBuf.size() / 3 << endl;

		//construct triangles vector
		std::vector<Triangle*> triangles;
		int numTriangles = posBuf.size() / 9;

		//traverse thru position and normal buffers constructing triangle objects for each triangle
		for (int i = 0; i < numTriangles; i++) {
			Triangle* t = new Triangle();
			t->a = glm::vec3(posBuf[9 * i + 0], posBuf[9 * i + 1], posBuf[9 * i + 2]);
			t->na = glm::vec3(norBuf[9 * i + 0], norBuf[9 * i + 1], norBuf[9 * i + 2]);
			t->b = glm::vec3(posBuf[9 * i + 3], posBuf[9 * i + 4], posBuf[9 * i + 5]);
			t->nb = glm::vec3(norBuf[9 * i + 3], norBuf[9 * i + 4], norBuf[9 * i + 5]);
			t->c = glm::vec3(posBuf[9 * i + 6], posBuf[9 * i + 7], posBuf[9 * i + 8]);
			t->nc = glm::vec3(norBuf[9 * i + 6], norBuf[9 * i + 7], norBuf[9 * i + 8]);

			triangles.push_back(t);

		}

		Light* l1 = new Light(glm::vec3(1.0, 1.0, 2.0), 1.0);
		lights.push_back(l1);

		ObjShape* bunny = new ObjShape();
		bunny->triangles = triangles;
		bunny->diffuse = glm::vec3(0.0, 0.0, 1.0);
		bunny->specular = glm::vec3(1.0, 1.0, 0.5);
		bunny->ambient = glm::vec3(0.1, 0.1, 0.1);
		bunny->exponent = 100.0;
		bunny->transform = true;

		glm::mat4 transformMat(
			1.5000,         0,         0,    0.3000,
			0,    1.4095, -0.5130, -1.5000,
			0,    0.5130,    1.4095,        0,
			0,        0,         0,    1.0000
		);
		transformMat = glm::transpose(transformMat);

		Image img(imageSize, imageSize);
		for (int y = 0; y < imageSize; y++) {
			for (int x = 0; x < imageSize; x++) {
				glm::vec3 resHP;
				glm::vec3 resN;
				Ray* r = rays[y * imageSize + x];
				if (!bunny->hitWithTransform(r, resHP, resN, transformMat)) {
					continue;
				}
				glm::vec3 color = bunny->calcBP(cameraPos, resHP, resN, lights);

				img.setPixel(x, y, 255 * color.r, 255 * color.g, 255 * color.b);
			}
		}

		img.writeToFile(outputFileName);

	}

	

	


	return 0;
}
