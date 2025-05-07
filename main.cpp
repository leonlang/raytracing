// simple_raytracer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include "code/Transformation.h"
#include "code/Object.h"
#include "code/Scene.h"
#include "code/Graphics.h"


#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#define TINYOBJLOADER_USE_MAPBOX_EARCUT
#include "tiny_obj_loader.h"

// Notes:
// Each covered Function presents an introduction. For example: Triangle Intersection corresponds to the same section in the report
// For Vec Math the library glm is used: https://github.com/g-truc/glm
// For OBJ Objects the library tinyObjLoader is used: https://github.com/tinyobjloader/tinyobjloader
// For Image Reading and Output the libraries stb: https://github.com/nothings/stb
// and CImg: https://cimg.eu/ are used.




// Normal Interpolation
// Concept for the Algorithm: https://www.khronos.org/opengl/wiki/Calculating_a_Surface_Normal
glm::vec3 calculateTriangleNormal(const Triangle& triangle) {
	glm::vec3 v1 = triangle.pointTwo - triangle.pointOne;
	glm::vec3 v2 = triangle.pointThree - triangle.pointOne;
	glm::vec3 normal = glm::cross(v1, v2);
	return glm::normalize(normal);
}

// Triangle Intersection
// This implementation uses the Möller–Trumbore intersection algorithm
// Concept for the Algorithm: https://www.graphics.cornell.edu/pubs/1997/MT97.pdf
inline float rayTriangleIntersection(const Ray& ray, const Triangle& triangle) {
	// Intersection of a ray with a triangle
	// Triangle Point1 in Cartesian Form
	glm::vec3 tP1Cartesian = glm::vec3(triangle.pointOne) / triangle.pointOne.w;
	glm::vec3 tP2Cartesian = glm::vec3(triangle.pointTwo) / triangle.pointTwo.w;
	glm::vec3 tP3Cartesian = glm::vec3(triangle.pointThree) / triangle.pointThree.w;
	// edge vectors
	glm::vec3 p1p2 = tP2Cartesian - tP1Cartesian;
	glm::vec3 p1p3 = tP3Cartesian - tP1Cartesian;

	// determinant
	glm::vec3 pvec = glm::cross(ray.direction, p1p3);
	float det = glm::dot(p1p2, pvec);

	if (fabs(det) < 1e-12f) return -INFINITY;

	// inverse Determinant
	float invDet = 1.0f / det;
	// distance vector
	glm::vec3 tvec = ray.origin - tP1Cartesian;
	// u and v paramter
	float u = glm::dot(tvec, pvec) * invDet;
	if (u < 0.0f || u > 1.0f) return -INFINITY;
	glm::vec3 qvec = glm::cross(tvec, p1p2);
	float v = glm::dot(ray.direction, qvec) * invDet;
	if (v < 0.0f || u + v > 1.0f) return -INFINITY;
	// intersection distance
	float t = glm::dot(p1p3, qvec) * invDet;

	// Check if triangle is behind ray
	if (t < 0.0f) return -INFINITY;
	return t;
}

// Phong Shading
// Concept for the Algorithm: https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/barycentric-coordinates.html
glm::vec3 calculateBarycentricCoords(const Triangle& triangle, const glm::vec3& point) {
	// Triangle Point1 in Cartesian Form
	glm::vec3 tP1Cartesian = glm::vec3(triangle.pointOne) / triangle.pointOne.w;
	glm::vec3 tP2Cartesian = glm::vec3(triangle.pointTwo) / triangle.pointTwo.w;
	glm::vec3 tP3Cartesian = glm::vec3(triangle.pointThree) / triangle.pointThree.w;

	// Calculate the vectors from pointOne to the other two vertices of the triangle
	// These vectors represent the edges of the triangle and the vector from the
	// first vertex to the point of interest. They are used for determining the relative
	// position of the point within the triangle.
	glm::vec3 v0 = tP2Cartesian - tP1Cartesian;
	glm::vec3 v1 = tP3Cartesian - tP1Cartesian;
	glm::vec3 v2 = point - tP1Cartesian;

	// The dot products are used to calculate the areas and angles between the vectors.
	// These values are crucial for the barycentric coordinates formula, which determines
	// how much of each vertex's influence is present at the point.
	float d00 = glm::dot(v0, v0);
	float d01 = glm::dot(v0, v1);
	float d11 = glm::dot(v1, v1);
	float d20 = glm::dot(v2, v0);
	float d21 = glm::dot(v2, v1);

	// Compute the denominator of the barycentric coordinates formula
	// It normalizes the coordinates, ensuring they sum to 1. 
	// This step ensures that the point lies within the triangle
	float denom = d00 * d11 - d01 * d01;

	// Compute the barycentric coordinates (v, w, u)
	// v and w are calculated using the dot products and the denominator
	float v = (d11 * d20 - d01 * d21) / denom;
	float w = (d00 * d21 - d01 * d20) / denom;
	// u is calculated to ensure the sum of the barycentric coordinates is 1
	float u = 1.0f - v - w;

	// Return the barycentric coordinates as a vec3
	// std::cout << "1:" << u << "2:" << v << "3:" << w << std::endl;
	return glm::vec3(u, v, w);
}
// Texture Mapping
// Concept: https://codeplea.com/triangular-interpolation
// Function to compute the interpolated texture coordinate
glm::vec2 getTextureCoordinate(const glm::vec3& barycentricCoords, const glm::vec2& texCoordA, const glm::vec2& texCoordB, const glm::vec2& texCoordC) {
	// Interpolate the texture coordinates using the barycentric coordinates
	glm::vec2 texCoord = barycentricCoords.x * texCoordA
		+ barycentricCoords.y * texCoordB
		+ barycentricCoords.z * texCoordC;

	return texCoord;
}

// Phong Shading
// Concept: https://cg.informatik.uni-freiburg.de/course\_notes/graphics\_02\_shading.pdf
glm::vec3 interpolateNormal(const Triangle& triangle, const glm::vec3& barycentricCoords) {
	// By multiplying each vertex normal by its corresponding barycentric coordinate, 
	// I am weighting each normal by how much influence that vertex has at the point of interest.
	return glm::normalize(
		barycentricCoords.x * triangle.normalOne +
		barycentricCoords.y * triangle.normalTwo +
		barycentricCoords.z * triangle.normalThree
	);
}


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
		float shadowDistance = rayTriangleIntersection(shadowRay, triangles[j]);
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
		glm::vec2 interpolatedTexCoordinate = getTextureCoordinate(calculateBarycentricCoords(triangle, intersectionPoint), triangle.colorOneCoordinate, triangle.colorTwoCoordinate, triangle.colorThreeCoordinate);
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

		float fDistance = rayTriangleIntersection(ray, objManager.triangles.at(k));
		
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
