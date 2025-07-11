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
#include <future>
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
glm::vec3 computeColorPoint(const Ray &ray, ObjectManager &objManager, Datastructure &datastructure, const glm::vec3 &lightPos, std::vector<glm::vec3> &randomCoordinates, int &boxCount, glm::vec3 &backgroundColor)
{
	glm::vec3 colorPoint(0, 0, 0);
	float distanceComparison = INFINITY;
	bool isIntersection = false;
	glm::vec3 color(0.f, 0.f, 0.f);
	int lastIntersectionNumber = 0;
	// Added data hierarchies here. They should result a vector
	// which contains the indexes of the triangles which are left
	// The for looop goes through those triangles
	for (int k : datastructure.checkIntersection(ray, boxCount))
	{
		float fDistance = Intersection::rayTriangleIntersection(ray, objManager.triangles.at(k));

		// Ray has hit the triangle
		if (fDistance != -INFINITY && fDistance < distanceComparison)
		{
			lastIntersectionNumber = k;
			isIntersection = true;
			distanceComparison = fDistance;
		}
	}
	// If no intersection was found, set the colorPoint to -1 so we can color it as background
	if (!isIntersection)
	{
		colorPoint = backgroundColor; // No intersection found
	}
	else
	{
		color = Graphics::phongIllumination(objManager, objManager.triangles.at(lastIntersectionNumber), ray, lightPos, distanceComparison);

		// Check for Shadows
		bool isShadow = false;
		int shadowAmount = 0;
		if (isShadow)
		{
			for (const glm::vec3 &point : randomCoordinates)
			{
				glm::vec3 lightPosChanged = lightPos + point;
				// Added data hierarchies here. They should result a vector
				// which contains the indexes of the triangles which are left
				// The for looop goes through those triangles
				if (Intersection::shadowIntersection(objManager, datastructure, lightPosChanged, distanceComparison, ray))

				// if (Intersection::shadowIntersection(objManager, datastructure, point, fDistance, ray))
				{
					shadowAmount++;
				};
			}
			color = color * (float(randomCoordinates.size() - shadowAmount)) / float(randomCoordinates.size()); //+ color * float(shadowAmount) * 0.5f / float(randomCoordinates.size());
																												// color = color * float(randomCoordinates.size() - shadowAmount) / float(randomCoordinates.size()) + color * float(shadowAmount) * 8.f / float(randomCoordinates.size());
																												// color = color * float(randomCoordinates.size() - shadowAmount) / float(randomCoordinates.size());
		}
		Graphics::reinhardtToneMapping(color, 0.25f, 1.f);
		// color = color * float(randomCoordinates.size() - shadowAmount) + color * float(shadowAmount) * 0.2f;
		colorPoint = glm::vec3(glm::ceil(color * 255.0f)); // Check if any point is over 255 and cout it
	}
	return colorPoint;
}

// Sending out Rays
// Concept: https://cg.informatik.uni-freiburg.de/course\_notes/graphics\_01\_raycasting.pdf
// Sends out Rays and returns the corresponding color for each pixel
ImageData sendRaysAndIntersectPointsColors(
    const glm::vec2 &imageSize,
    const glm::vec4 &lightPos,
    ObjectManager &objManager,
    Datastructure &datastructure,
    glm::vec3 &backgroundColor,
    std::vector<int> &boxCounts,
    std::vector<glm::vec3> &shadowPoints)
{
    const glm::vec3 lightPosition(lightPos.x, lightPos.y, lightPos.z);

    ImageData imageData;
	// specify how many threads the hardware should use.
	// If you specify more threads than available cores, the hardware will run at max and possibly everything else will run slower or crash.
	// I would recommend half the number of threads as available cores.
    const int numThreads = 1; //std::thread::hardware_concurrency() / 2;
    std::vector<std::thread> threads;
	// store the results I get from each thread in a vector
    std::vector<std::vector<std::tuple<glm::vec2, glm::vec3, int>>> threadResults(numThreads);

	// go through number of specified threads
    for (int t = 0; t < numThreads; ++t)
    {
        threads.emplace_back([=, &objManager, &datastructure, &backgroundColor, &shadowPoints, &threadResults]() {
            int start = t * imageSize.x / numThreads;
            int end = (t + 1) * imageSize.x / numThreads;

			// Each thread can store its own results in his own vector. This has the advantage that we don't have to use a mutex for each pixel, which would reduce performance.
            std::vector<std::tuple<glm::vec2, glm::vec3, int>> localResults;

            for (int i = start; i < end; ++i)
            {
                for (int j = 0; j < imageSize.y; ++j)
                {
                    Ray ray(glm::vec3(0.0f, 0.0f, 100000.0f));
                    ray.direction.x = i - imageSize.x / 2.0f;
                    ray.direction.y = j - imageSize.y / 2.0f;

                    int boxCount = 0;
                    glm::vec3 color = computeColorPoint(ray, objManager, datastructure, lightPosition, shadowPoints, boxCount, backgroundColor);

                    localResults.emplace_back(glm::vec2(i, j), color, boxCount);
                }
            }

            threadResults[t] = std::move(localResults);
        });
    }

    for (auto &t : threads)
        t.join();

    for (const auto &threadVec : threadResults)
    {
        for (const auto &[point, color, count] : threadVec)
        {
            imageData.imagePoints.push_back(point);
            imageData.imageColors.push_back(color);
            boxCounts.push_back(count);
        }
    }

    return imageData;
}



int main()
{
	// Choose Szene

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
		glm::vec3 backgroundColor(0.f, 0.f, 0.f);
		std::vector<glm::vec3> randomCoordinates = Graphics::generateRandomCoordinates(2, 500.0f);
		std::vector<glm::vec3> shadowPointsAO = Graphics::ambientOcclusionShadowPoints(); // Get the shadow points for ambient occlusion

		// Choose Szene
		Scene::hairball(objManager, viewMatrix, angleDegree, imageSize, lightPos, backgroundColor);

		// Transform the view matrix to the object space
		objManager.applyViewTransformation(glm::inverse(viewMatrix));
		lightPos = glm::inverse(viewMatrix) * lightPos;

		// End the timer
		auto endInit = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsedInit = endInit - startInit;

		// Print the loaded objects
		for (const auto &[name, obj] : objManager.objObjects)
		{
			std::cout << "Filename: " << name << "\n";
		}

		std::cout << "Time taken for OBJ Loading: " << elapsedInit.count() << " seconds " << std::endl;

		// Start the timer for RaymIntersection
		auto startDatastructureInit = std::chrono::high_resolution_clock::now();

		// Example Triangles
		// datastructure.fillTriangleNumbers(0, objManager.triangles.size() - 1);
		datastructure.initDatastructure({objManager.triangles});

		auto endDatastructureInit = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsedDatastructureInit = endDatastructureInit - startDatastructureInit;
		std::cout << "Time taken for Datastructure Initialization: " << elapsedDatastructureInit.count() << " seconds " << std::endl;

		auto start = std::chrono::high_resolution_clock::now();
		std::vector<int> boxCounts; // Vector to store the number of boxes checked during intersection
		// max value of boxCounts
		ImageData points = sendRaysAndIntersectPointsColors(imageSize, lightPos, objManager, datastructure, backgroundColor, boxCounts, shadowPointsAO);
		int maxBoxes = *std::max_element(boxCounts.begin(), boxCounts.end());
		std::cout << "Max Box Count: " << maxBoxes << std::endl;

		// End the timer
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = end - start;
		std::cout << "Time taken for Intersection: " << elapsed.count() << " seconds " << std::endl;

		// Draw Image based on found Points
		// drawImage(imageSize, points.imagePoints, points.imageColors, angleDegree, true, false);
		// Graphics::drawImage(imageSize, points.imagePoints, points.imageColors, angleDegree, true, false);
		// glm::vec3 heatmapColor = Graphics::getHeatmapColor(1000, 10000);
		// std::cout << "Heatmap Color: " << heatmapColor.x << ", " << heatmapColor.y << ", " << heatmapColor.z << std::endl;
		// std::vector<glm::vec3> heatmapColors = Graphics::convertToHeatmap(boxCounts, 10000);
		// Store Image
		Graphics::drawImage(imageSize, points.imagePoints, points.imageColors, std::to_string(int(angleDegree)), true, false);
		// Store Heatmap Image
		Graphics::drawImage(imageSize, points.imagePoints, Graphics::convertToHeatmap(boxCounts, 3000), "_heatmap", true, false);
	}
}
