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
	for (int j = 0; j < triangles.size(); j++) {
		float shadowDistance = Intersection::rayTriangleIntersection(shadowRay, triangles[j]);
		if (shadowDistance != -INFINITY) {
			return true;
		}

	}
	return false;
}

// Soft Shadows
// Generate multiple light Sources in near distance to each other
// Tone Mapping
// Concept: https://64.github.io/tonemapping/
glm::vec3 softShadow(int& lightAmount,ObjectManager& objManager,const Triangle& triangle, const Ray& ray, const glm::vec3& lightPos, const float& distance) {
	glm::vec3 objColor(1, 1, 0);
	if (!triangle.textureName.empty()) {
		glm::vec3 intersectionPoint = ray.origin + distance * ray.direction;
		glm::vec2 interpolatedTexCoordinate = Graphics::getTextureCoordinate(Intersection::calculateBarycentricCoords(triangle, intersectionPoint), triangle.colorOneCoordinate, triangle.colorTwoCoordinate, triangle.colorThreeCoordinate);
		// std::cout << "Interpolated X:" << interpolatedTexCoordinate.x << "Xone:" << triangle->colorOneCoordinate.x << "Xtwo:" << triangle->colorTwoCoordinate.x << "XThree:" << triangle->colorThreeCoordinate.x << std::endl;
		unsigned char* texData = objManager.textureData[triangle.textureName];
		glm::ivec2 texDim = objManager.textureDimensions[triangle.textureName];

		size_t texIndex = (static_cast<int>(interpolatedTexCoordinate.y) *texDim.x + static_cast<int>(interpolatedTexCoordinate.x)) * 3;
		objColor.x = texData[texIndex + 0] / 255.0f;
		objColor.y = texData[texIndex + 1] / 255.0f;
		objColor.z = texData[texIndex + 2] / 255.0f;
	}
	glm::vec3 color(0.0f,0.0f,0.0f);
	glm::vec3 lightPosChanged = lightPos;
	// Generate the lightPos based on the last lightPos and change always one coordinate
	/*
	for (int i = 0; i < lightAmount; i++) {
		bool isShadow = shadowIntersection(objManager, lightPosChanged, distance, ray);
		glm::vec3 colorPhong = phongIllumination(triangle, ray, lightPosChanged, objColor, distance);
		// glm::vec3 colorPhong (240.f,1.f,1.f);
		if (isShadow) { colorPhong /= 5; };
		color += colorPhong;
		// Before it was 2
		switch (i % 3) {
		case 0:
			lightPosChanged.x += 3.0f;
			break;
		case 1:
			lightPosChanged.y += 3.0f; 
			break;
		case 2:
			lightPosChanged.z += 3.0f;
			break;
		}
	}
	*/
	// Disable Shadows for testing purposes
	color = Graphics::phongIllumination(objManager,triangle, ray, lightPosChanged, distance);


	// Reinhardt Tone Mapping 
	// color = color / (color + 1.0f);
	color = color / (color + 0.5f);
	// color = color / (color + 0.1f);
	// color = color / (color + 4.0f);

	// Add Gamma
	glm::vec3 gamma(1.1f, 1.1f, 1.1f);
	// glm::vec3 gamma (2.2f, 2.2f, 2.2f);
	color = glm::pow(color, gamma);

	return color;
}

// Ray Intersection with Boxes and Triangles
// Combines the Methods for Slab Test, Bounding Box Volume Hierarchy and Triangle Intersection
std::pair<glm::vec2, glm::vec3> rayIntersection(const Ray& ray, ObjectManager& objManager, const int& pointX,const int& pointY, const glm::vec3& lightPos) {

	glm::vec3 colorPoint(0, 0, 0);
	float distanceComparison = INFINITY;
	for (int k = 0; k < objManager.triangles.size(); k++) {

		float fDistance = Intersection::rayTriangleIntersection(ray, objManager.triangles.at(k));
		
		if (fDistance != -INFINITY && fDistance < distanceComparison) {
			distanceComparison = fDistance;

			int lightAmount = 1;
			glm::vec3 color = Graphics::phongIllumination(objManager,objManager.triangles.at(k), ray, lightPos, fDistance);
			// glm::vec3 color = softShadow(lightAmount,objManager, objManager.triangles.at(k), ray,lightPos,fDistance);
			// Convert 0...1 color values to 1...255 color Values
			colorPoint.x = int((color.x * 255));
			colorPoint.y = int((color.y * 255));
			colorPoint.z = int((color.z * 255));
		}
	} 
	glm::vec2 imagePoint(pointX, pointY);
	return { imagePoint, colorPoint };
}


// Sending out Rays
// Concept: https://cg.informatik.uni-freiburg.de/course\_notes/graphics\_01\_raycasting.pdf
// Sends out Rays and returns the corresponding color for each pixel

ImageData sendRaysAndIntersectPointsColors(const glm::vec2& imageSize, const glm::vec4& lightPos, ObjectManager& objManager) {
	Ray ray(glm::vec3(0.0f, 0.0f, 400.0f));
	glm::vec2 rayXY = glm::vec2(ray.direction.x, ray.direction.y);
	ImageData imageData;

	for (int i = 0; i < imageSize.x; ++i) {
		for (int j = 0; j < imageSize.y; ++j) {
			ray.direction.x = i + rayXY.x - imageSize.x / 2;
			ray.direction.y = j + rayXY.y - imageSize.y / 2;

			std::pair<glm::vec2, glm::vec3> points = rayIntersection(ray, objManager, i, j, lightPos);
			if (points.second != glm::vec3(0, 0, 0)) {
				imageData.imagePoints.push_back(points.first);
				imageData.imageColors.push_back(points.second);
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
