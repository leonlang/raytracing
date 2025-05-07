// simple_raytracer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// Import my own header files
#include "code/Transformation.h"
#include "code/Object.h"
#include "code/Scene.h"
#include "code/Graphics.h"
#include "code/Intersection.h"

// Import external libraries
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#define TINYOBJLOADER_USE_MAPBOX_EARCUT
#include "tiny_obj_loader.h"



// Notes:
// Each covered Function presents an introduction. For example: Triangle Intersection corresponds to the same section in the report
// For Vec Math the library glm is used: https://github.com/g-truc/glm
// For OBJ Objects the library tinyObjLoader is used: https://github.com/tinyobjloader/tinyobjloader
// For Image Reading and Output the libraries stb: https://github.com/nothings/stb
// and CImg: https://cimg.eu/ are used.


// Shadow Intersection
// Sends out Ray from intersection to light source. If object is in between, there is shadow
bool shadowIntersection(ObjectManager& objManager, const glm::vec3& lightPos, const float& fDistance, const Ray& ray) {
	const std::vector<Triangle>& triangles = objManager.triangles;
	// const std::vector<Triangle>& trianglesBox = pairShadow.second;
	Ray shadowRay(lightPos - ray.direction * fDistance);
	shadowRay.origin = ray.direction * fDistance;
	// shadowRay.origin += ray.direction * 0.001f; // add small value to prevent shadowAcne
		// Prevents intersection between same object
		// if (shadowObjFilename != currentObjFilename) {
	for (int i = 0; i < triangles.size(); i++) {
		float shadowDistance = Intersection::rayTriangleIntersection(shadowRay, triangles[i]);
		if (shadowDistance != -INFINITY) {
			return true;
		}

	}
	return false;
}

void reinhardtToneMapping(glm::vec3& color) {
	color = color / (color + 1.0f);
	// color = color / (color + 0.5f);
	// color = color / (color + 0.1f);
	// color = color / (color + 4.0f);

	// Add Gamma
	glm::vec3 gamma(1.1f, 1.1f, 1.1f);
	// glm::vec3 gamma (2.2f, 2.2f, 2.2f);
	color = glm::pow(color, gamma);
}
std::vector<glm::vec3> generateRandomCoordinates(int amount, float range) {
	std::vector<glm::vec3> coordinates;
	glm::vec3 coordinate(0, 0, 0);
	for (int i = 0; i < amount; i++) {
		switch (i % 3) {
		case 0:
			coordinate.x += range;
			break;
		case 1:
			coordinate.y += range; 
			break;
		case 2:
			coordinate.z += range;
			break;
		}
		coordinates.push_back(coordinate);
	}
	return coordinates;
}


// Ray Intersection with Boxes and Triangles
// Combines the Methods for Slab Test, Bounding Box Volume Hierarchy and Triangle Intersection
glm::vec3 rayIntersection(const Ray& ray, ObjectManager& objManager, const glm::vec3& lightPos, std::vector<glm::vec3>& randomCoordinates) {

	glm::vec3 colorPoint(0, 0, 0);
	float distanceComparison = INFINITY;
	for (int k = 0; k < objManager.triangles.size(); k++) {

		float fDistance = Intersection::rayTriangleIntersection(ray, objManager.triangles.at(k));
		
		if (fDistance != -INFINITY && fDistance < distanceComparison) {
			distanceComparison = fDistance;

			glm::vec3 color = Graphics::phongIllumination(objManager,objManager.triangles.at(k), ray, lightPos, fDistance);

			int shadowAmount = 0;
			for (const glm::vec3& point : randomCoordinates) {
				glm::vec3 lightPosChanged = lightPos + point;
				if (shadowIntersection(objManager, lightPosChanged, fDistance, ray)) { shadowAmount++; };
			}
			std::cout << "Shadow Amount: " << shadowAmount << std::endl;
			// glm::vec3 color = softShadow(lightAmount,objManager, objManager.triangles.at(k), ray,lightPos,fDistance);
			// Convert 0...1 color values to 1...255 color Values
			colorPoint.x = int((color.x * 255));
			colorPoint.y = int((color.y * 255));
			colorPoint.z = int((color.z * 255));
		}
	} 
	return colorPoint;
}




// Sending out Rays
// Concept: https://cg.informatik.uni-freiburg.de/course\_notes/graphics\_01\_raycasting.pdf
// Sends out Rays and returns the corresponding color for each pixel
ImageData sendRaysAndIntersectPointsColors(const glm::vec2& imageSize, const glm::vec4& lightPos, ObjectManager& objManager) {
	Ray ray(glm::vec3(0.0f, 0.0f, 400.0f));
	glm::vec2 rayXY = glm::vec2(ray.direction.x, ray.direction.y);
	ImageData imageData;
	std::vector<glm::vec3> randomCoordinates = generateRandomCoordinates(5,3.0f);
	for (int i = 0; i < imageSize.x; ++i) {
		for (int j = 0; j < imageSize.y; ++j) {
			ray.direction.x = i + rayXY.x - imageSize.x / 2;
			ray.direction.y = j + rayXY.y - imageSize.y / 2;

			glm::vec3 colorPoint = rayIntersection(ray, objManager, lightPos, randomCoordinates);
			if (colorPoint != glm::vec3(0, 0, 0)) {
				imageData.imagePoints.push_back(glm::vec2(i, j));
				imageData.imageColors.push_back(colorPoint);
			}
		}	
	}
	return imageData;
}

int main()
{

	// save images at different degrees based on camera
	for (float angleDegree = 0; angleDegree < 360; angleDegree = angleDegree + 400) {

		// Start the timer
		auto startInit = std::chrono::high_resolution_clock::now();

		// Create an ObjectManager instance 
		glm::mat4 viewMatrix;
		ObjectManager objManager;
		glm::vec2 imageSize; 		
		glm::vec4 lightPos;

		// Choose Szene
		// szene1(objManager,viewMatrix,angleDegree,imageSize,lightPos);
		Scene::scene1(objManager,viewMatrix,angleDegree,imageSize,lightPos);
		// Transform the view matrix to the object space
		objManager.applyViewTransformation(glm::inverse(viewMatrix));
		lightPos = glm::inverse(viewMatrix) * lightPos;

		// End the timer 
		auto endInit = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsedInit = endInit - startInit;
		std::cout << "Time taken for OBJ Loading: " << elapsedInit.count() << " seconds " << std::endl;

		// Print the loaded objects
		for (const auto& [name, obj] : objManager.objObjects) {
			std::cout << "Filename: " << name << "\n";
		}
				
		// Start the timer for RaymIntersection
		auto start = std::chrono::high_resolution_clock::now();
		ImageData points = sendRaysAndIntersectPointsColors(imageSize, lightPos, objManager);
		// End the timer 
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = end - start;
		std::cout << "Time taken for Intersection: " << elapsed.count() << " seconds " << std::endl;

		// Draw Image based on found Points
		// drawImage(imageSize, points.imagePoints, points.imageColors, angleDegree, true, false);
		Graphics::drawImage(imageSize, points.imagePoints, points.imageColors, angleDegree, true, false);
	}
}
