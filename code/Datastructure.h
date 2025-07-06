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
    bool oneTriangleLeft; // Flag to indicate if this node contains a triangle or a bounding box
    // Constructor for lbvh and sah bounding box
    Node(const glm::vec3& minBoxD, const glm::vec3& maxBoxD)
        : minBox(minBoxD), maxBox(maxBoxD), triangleIndex(-1), left(nullptr), right(nullptr), oneTriangleLeft(false) {
    }

    // Constructor for end of tree where only one triangle is left
    Node(int index)
        : triangleIndex(index), left(nullptr), right(nullptr), oneTriangleLeft(true) {
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
    Node* rootNode; // Root node of the BVH tree
    void initDatastructure(const std::vector<Triangle> &triangles);
    std::vector<int> checkIntersection(const Ray &ray, int& boxCount);
    void createBoundingBox(const std::vector<Triangle> &triangles);
    void createBoundingBoxWithNumbers(const std::vector<Triangle> &triangles, const std::vector<int> &triangleNumbers);
    void fillTriangleNumbers(int a, int b);
    bool intersectRayAabb(const Ray &ray, const glm::vec3 &minBox, const glm::vec3 &maxBox);
    void nodeBoundingBoxIntersection(Node* node, const Ray& ray, std::vector<int>& collectedIndices, int& boxCount);

};
class Lbvh
{
public:
    struct mortonTriangle {
        std::bitset<60> bits; // Adjust size as needed
        int index;
    };
    float avgTriangleSize(const std::vector<Triangle> &triangles);
    std::pair<float,glm::vec3> gridConstruction(const std::vector<Triangle> &triangles);
    glm::vec3 centralCoordinates(const Triangle &triangle);
    std::bitset<60> coordinateToMorton(glm::vec3 &coordinate);
    std::vector<mortonTriangle> mortonCodes(const std::vector<Triangle> &triangles, float &changeGridAmount);
    Node* createTree(const std::vector<Triangle> &triangles,float &changeGridAmount);
    
};

class Sah
{
public:
    const float sahBucketCost(const std::vector<Triangle> &triangles,std::vector<int>& triangleNumbers); // Cost of SAH Bucket
    std::vector<int> getSortedTriangleNumbers(const std::vector<Triangle>& triangles, std::vector<int>& triangleNumbers, glm::vec3& minBox, glm::vec3& maxBox);
    std::pair<std::vector<int>, std::vector<int>> findBestBucketSplit(const std::vector<Triangle>& triangles, std::vector<int>& sortedTriangleNumbers, int& bucketCount);
    Node* createTree(const std::vector<Triangle> &triangles,std::vector<int>& triangleNumbers, int& bucketCount);
};
#endif // DATASTRUCTURE_H
