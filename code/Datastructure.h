#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include <vector>
#include <glm/glm.hpp>
#include "Object.h"

class Datastructure
{
public:
    std::vector<int> triangleNumbers;
    glm::vec3 minBox;
    glm::vec3 maxBox;
    void initDatastructure(const std::vector<Triangle> &triangles);
    std::vector<int> checkIntersection(const Ray &ray);
    void createBoundingBox(const std::vector<Triangle> &triangles);
    void fillTriangleNumbers(int a, int b);
    bool intersectRayAabb(const Ray &ray, const glm::vec3 &minBox, const glm::vec3 &maxBox);
};
class Lbvh
{
public:
    glm::vec3 centralCoordinates(Triangle &triangle);
    std::vector<bool> coordinateToBits(glm::vec3 &coordinate);
    glm::vec3 avgTriangleSize(const std::vector<Triangle> &triangles);
    int gridSize(const std::vector<Triangle> &triangles);
};

#endif // DATASTRUCTURE_H
