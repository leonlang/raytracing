// simple_raytracer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// Import my own header files
#include "code/Transformation.h"
#include "code/Object.h"
#include "code/Scene.h"
#include "code/Graphics.h"
#include "code/Intersection.h"
#include "code/Datastructure.h"

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

// Combines the Methods for Data Hierarchies Triangle Intersection and Shadows
glm::vec3 computeColorPoint(const Ray &ray, ObjectManager &objManager, Datastructure &datastructure, const glm::vec3 &lightPos, std::vector<glm::vec3> &randomCoordinates)
{
	glm::vec3 colorPoint(0, 0, 0);
	float distanceComparison = INFINITY;
	// Add data hierarchies here. They should result a vector
	// which contains the indexes of the triangles which are left
	// The for looop goes through those triangles

	// for (int k = 0; k < objManager.triangles.size(); k++)
	//{
	bool isIntersection = false;
	for (int k : datastructure.checkIntersection(ray))
	{
		float fDistance = Intersection::rayTriangleIntersection(ray, objManager.triangles.at(k));

		// Ray has hit the triangle
		if (fDistance != -INFINITY && fDistance < distanceComparison)
		{
			isIntersection = true;
			distanceComparison = fDistance;
			glm::vec3 color = Graphics::phongIllumination(objManager, objManager.triangles.at(k), ray, lightPos, fDistance);

			// Check for Shadows
			bool isShadow = false;
			int shadowAmount = 0;
			if (isShadow)
			{
				for (const glm::vec3 &point : randomCoordinates)
				{
					glm::vec3 lightPosChanged = lightPos + point;
					// Add data hierarchies here. They should result a vector
					// which contains the indexes of the triangles which are left
					// The for looop goes through those triangles
					if (Intersection::shadowIntersection(objManager, datastructure, lightPosChanged, fDistance, ray))
					{
						shadowAmount++;
					};
				}
			}
			Graphics::reinhardtToneMapping(color, 0.25f, 1.f);
			// color = color * float(randomCoordinates.size() - shadowAmount) + color * float(shadowAmount) * 0.2f;
			colorPoint = glm::vec3(glm::ceil(color * 255.0f));			// Check if any point is over 255 and cout it
		}
	}
	// If no intersection was found, set the colorPoint to -1 so we can color it as background
	if (!isIntersection)
	{
		colorPoint = glm::vec3(-1.f); // No intersection found
	}
	return colorPoint;
}

// Sending out Rays
// Concept: https://cg.informatik.uni-freiburg.de/course\_notes/graphics\_01\_raycasting.pdf
// Sends out Rays and returns the corresponding color for each pixel
ImageData sendRaysAndIntersectPointsColors(const glm::vec2 &imageSize, const glm::vec4 &lightPos, ObjectManager &objManager, Datastructure &datastructure)
{
	Ray ray(glm::vec3(0.0f, 0.0f, 400.0f));
	glm::vec2 rayXY = glm::vec2(ray.direction.x, ray.direction.y);
	ImageData imageData;
	std::vector<glm::vec3> randomCoordinates = Graphics::generateRandomCoordinates(1, 3.0f);
	for (int i = 0; i < imageSize.x; ++i)
	{
		for (int j = 0; j < imageSize.y; ++j)
		{
			ray.direction.x = i + rayXY.x - imageSize.x / 2;
			ray.direction.y = j + rayXY.y - imageSize.y / 2;

			glm::vec3 colorPoint = computeColorPoint(ray, objManager, datastructure, lightPos, randomCoordinates);

			// If no intersection was found, the colorPoint is set to -1 so we can color it as background
			if (colorPoint == glm::vec3(-1.f))
			{
				colorPoint = glm::vec3(173, 216, 230); // Background color
			}
			imageData.imagePoints.push_back(glm::vec2(i, j));
			imageData.imageColors.push_back(colorPoint);


		}
	}
	return imageData;
}

int main()
{

	// save images at different degrees based on camera
	for (float angleDegree = 0; angleDegree < 360; angleDegree = angleDegree + 400)
	{

		// Start the timer
		auto startInit = std::chrono::high_resolution_clock::now();

		// Create an ObjectManager instance
		glm::mat4 viewMatrix;
		ObjectManager objManager;
		Datastructure datastructure;
		glm::vec2 imageSize;
		glm::vec4 lightPos;


		/* Lbvh lbvh;
		glm::vec3 sampleCoordinate(65531.0f, 30000.0f, 1600.0f);
		lbvh.coordinateToBits(sampleCoordinate);
		*/

		// Choose Szene
		// szene1(objManager,viewMatrix,angleDegree,imageSize,lightPos);
		Scene::sceneChair(objManager, viewMatrix, angleDegree, imageSize, lightPos);
		// Transform the view matrix to the object space
		objManager.applyViewTransformation(glm::inverse(viewMatrix));
		lightPos = glm::inverse(viewMatrix) * lightPos;

		// End the timer
		auto endInit = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsedInit = endInit - startInit;
		std::cout << "Time taken for OBJ Loading: " << elapsedInit.count() << " seconds " << std::endl;

		// Print the loaded objects
		for (const auto &[name, obj] : objManager.objObjects)
		{
			std::cout << "Filename: " << name << "\n";
		}

		// Start the timer for RaymIntersection
		auto startDatastructureInit = std::chrono::high_resolution_clock::now();

		// Example Triangles
		datastructure.fillTriangleNumbers(0, objManager.triangles.size() - 1);
		datastructure.initDatastructure({objManager.triangles});

		auto endDatastructureInit = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsedDatastructureInit = endDatastructureInit - startDatastructureInit;
		std::cout << "Time taken for Datastrcture Initialization: " << elapsedDatastructureInit.count() << " seconds " << std::endl;
		
		auto start = std::chrono::high_resolution_clock::now();

		ImageData points = sendRaysAndIntersectPointsColors(imageSize, lightPos, objManager, datastructure);
		// End the timer
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = end - start;
		std::cout << "Time taken for Intersection: " << elapsed.count() << " seconds " << std::endl;

		// Draw Image based on found Points
		// drawImage(imageSize, points.imagePoints, points.imageColors, angleDegree, true, false);
		Graphics::drawImage(imageSize, points.imagePoints, points.imageColors, angleDegree, true, false);
		
	}
}
