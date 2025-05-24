#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include <vector>
#include <glm/glm.hpp>
#include "Object.h"


struct Node {
    glm::vec3 minBox;
    glm::vec3 maxBox;
    int triangleIndex; // -1 if this node is a bounding box, otherwise stores a valid index
    Node* left;
    Node* right;
    bool hasTriangles; // Flag to indicate if this node contains a triangle or a bounding box

    // Constructor for bounding box
    Node(const glm::vec3& minBoxD, const glm::vec3& maxBoxD)
        : minBox(minBoxD), maxBox(maxBoxD), triangleIndex(-1), left(nullptr), right(nullptr), hasTriangles(false) {
    }

    // Constructor for triangle index
    Node(int index)
        : triangleIndex(index), left(nullptr), right(nullptr), hasTriangles(true) {
        minBox = glm::vec3(0); // Default values to avoid uninitialized data
        maxBox = glm::vec3(0);
    }
};

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
