#include <iostream>
#include <string>
#include <vector>
#include <glm/glm.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "Image.h"

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

vector<glm::vec3> calcRayDirections(glm::vec3 cameraPos, float fov, float aspectRatio, int dimensions) {
	float sideLength = 2 * tan(fov/2);
	float stepSize = sideLength / dimensions;
	float startingPoint = -sideLength / 2;

	vector<glm::vec3> result;

	for (int row = 0; row < dimensions; row++) {
		float y = startingPoint + (row * stepSize) + (stepSize / 2);
		for (int col = 0; col < dimensions; col++) {
			float x = startingPoint + (col * stepSize) + (stepSize / 2);
			glm::vec3 pixelPos(x, y, cameraPos.z - 1);
			glm::vec3 rayDir = glm::normalize(pixelPos - cameraPos);
			result.push_back(rayDir);
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

	vector<glm::vec3> results = calcRayDirections(cameraPos, fov, aspectRatio, dimensions);

	for (size_t i = 0; i < results.size(); i++) {
		cout << results[i].x << ", " << results[i].y << ", " << results[i].z << endl;
	}
	return 0;

	// Load geometry
	vector<float> posBuf; // list of vertex positions
	vector<float> norBuf; // list of vertex normals
	vector<float> texBuf; // list of vertex texture coords
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	string errStr;
	bool rc = tinyobj::LoadObj(&attrib, &shapes, &materials, &errStr, outputFileName.c_str());
	if(!rc) {
		cerr << errStr << endl;
	} else {
		// Some OBJ files have different indices for vertex positions, normals,
		// and texture coordinates. For example, a cube corner vertex may have
		// three different normals. Here, we are going to duplicate all such
		// vertices.
		// Loop over shapes
		for(size_t s = 0; s < shapes.size(); s++) {
			// Loop over faces (polygons)
			size_t index_offset = 0;
			for(size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				size_t fv = shapes[s].mesh.num_face_vertices[f];
				// Loop over vertices in the face.
				for(size_t v = 0; v < fv; v++) {
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					posBuf.push_back(attrib.vertices[3*idx.vertex_index+0]);
					posBuf.push_back(attrib.vertices[3*idx.vertex_index+1]);
					posBuf.push_back(attrib.vertices[3*idx.vertex_index+2]);
					if(!attrib.normals.empty()) {
						norBuf.push_back(attrib.normals[3*idx.normal_index+0]);
						norBuf.push_back(attrib.normals[3*idx.normal_index+1]);
						norBuf.push_back(attrib.normals[3*idx.normal_index+2]);
					}
					if(!attrib.texcoords.empty()) {
						texBuf.push_back(attrib.texcoords[2*idx.texcoord_index+0]);
						texBuf.push_back(attrib.texcoords[2*idx.texcoord_index+1]);
					}
				}
				index_offset += fv;
				// per-face material (IGNORE)
				shapes[s].mesh.material_ids[f];
			}
		}
	}
	cout << "Number of vertices: " << posBuf.size()/3 << endl;
	
	return 0;
}
