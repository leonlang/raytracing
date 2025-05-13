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

#endif // DATASTRUCTURE_H
