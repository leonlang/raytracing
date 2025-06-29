#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <vector>
#include <string>
#include <glm/glm.hpp>
#define cimg_display 0
#include "CImg.h"
#include "Object.h"

using namespace cimg_library;

struct ImageData
{
    std::vector<glm::vec2> imagePoints;
    std::vector<glm::vec3> imageColors;
};

namespace Graphics
{
    glm::vec3 phongIllumination(ObjectManager &objManager, const Triangle &triangle,
                                const Ray &ray, const glm::vec3 &lightPos, const float &distance);
    glm::vec2 getTextureCoordinate(const glm::vec3 &barycentricCoords,
                                   const glm::vec2 &texCoordA,
                                   const glm::vec2 &texCoordB,
                                   const glm::vec2 &texCoordC);
    void reinhardtToneMapping(glm::vec3 &color, float exposure, float gamma);
    std::vector<glm::vec3> generateRandomCoordinates(int amount, float range);
    std::vector<glm::vec3> convertTriangleToShadowPoints(const ObjectManager &objManager);
    std::vector<glm::vec3> ambientOcclusionShadowPoints();
    void drawImage(const glm::vec2 &imgSize, const std::vector<glm::vec2> &imagePoints,
                   const std::vector<glm::vec3> &imageColors, const std::string &angleDegree,
                   const bool &saveImage, const bool &displayImage);
    glm::vec3 getHeatmapColor(int value, int maxVal);
    std::vector<glm::vec3> convertToHeatmap(const std::vector<int> &boxCounts, int maxVal);

}

#endif // IMAGE_GENERATION_H
