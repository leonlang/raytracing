#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <unordered_map>
#include "tiny_obj_loader.h"
#include <algorithm>

class Triangle
{
public:
    glm::vec4 pointOne;
    glm::vec4 pointTwo;
    glm::vec4 pointThree;
    glm::vec3 normalOne;   // Normal at pointOne
    glm::vec3 normalTwo;   // Normal at pointTwo
    glm::vec3 normalThree; // Normal at pointThree
    glm::vec2 colorOneCoordinate;
    glm::vec2 colorTwoCoordinate;
    glm::vec2 colorThreeCoordinate;
    // glm::vec3 color;
    // maybe Vertex color is needed, but I believe it is not needed
    glm::vec3 ambient;
    glm::vec3 specular;
    glm::vec3 diffuse;
    float shininess;
    std::string textureName;
    // Default constructor
    Triangle();

    // Parameterized constructor
    Triangle(glm::vec4 p1, glm::vec4 p2, glm::vec4 p3, glm::vec3 n1, glm::vec3 n2, glm::vec3 n3);

    // Method to calculate the normal of the triangle
    glm::vec3 calculateNormal() const;
};

class Ray
{
public:
    glm::vec3 origin;
    glm::vec3 direction;
    // Constructor
    Ray(glm::vec3 d);
};

struct Node
{
    glm::vec3 minBox;
    glm::vec3 maxBox;
    std::vector<Triangle> triangles;
    Node *left;
    Node *right;

    // Constructor to initialize the node with a value
    Node(const std::vector<Triangle> &triangleD, const glm::vec3 &minBoxD, const glm::vec3 &maxBoxD)
        : triangles(triangleD), minBox(minBoxD), maxBox(maxBoxD), left(nullptr), right(nullptr)
    {
    }
};

class ObjObject
{
public:
    // std::vector<Triangle> triangles;
    int triangleStartIndex;
    int triangleEndIndex;
    glm::vec3 minBox;
    glm::vec3 maxBox;
    // ambientStrength, specularStrength and shininess
    // glm::vec3 objProperties;
    // loaded textures
};
class ObjectManager
{
public:
    std::vector<Triangle> triangles;
    std::unordered_map<std::string, unsigned char *> textureData;
    std::unordered_map<std::string, glm::ivec2> textureDimensions;

    std::unordered_map<std::string, ObjObject> objObjects;
    // std::vector<ObjObject> objObjects;

    // Method to load triangles from an OBJ file
    void loadObjFile(const std::string &objName, const std::string &objFilename);

    // Method to transform triangles for a specific OBJ file
    void applyViewTransformation(const glm::mat4 &matrix);
    void transformTriangles(const std::string &objFilename, const glm::mat4 &matrix);
    void splitTrianglesForBox(Node *root);
    void createBoundingHierarchy(const std::string &objFilename);
};

#endif // OBJECT_H
