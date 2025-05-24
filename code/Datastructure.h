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
    struct mortonTriangle {
        std::bitset<72> bits; // Adjust size as needed
        int index;
    };
    float avgTriangleSize(const std::vector<Triangle> &triangles);
    std::pair<float,glm::vec3> gridConstruction(const std::vector<Triangle> &triangles);
    glm::vec3 centralCoordinates(const Triangle &triangle);
    std::bitset<72> coordinateToMorton(glm::vec3 &coordinate);
    std::vector<mortonTriangle> mortonCodes(const std::vector<Triangle> &triangles);
    void createTree(const std::vector<Triangle> &triangles);
    
};

#endif // DATASTRUCTURE_H
