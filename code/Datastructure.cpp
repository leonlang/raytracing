#include "Datastructure.h"
#include <glm/gtx/component_wise.hpp>

void Datastructure::initDatastructure(const std::vector<Triangle> &triangles)
{
    // Datastructure 1: Simple intersection with one box which contains all triangles
    createBoundingBox(triangles);
}

std::vector<int> Datastructure::checkIntersection(const Ray &ray)
{
    // Datastructure 1: Simple intersection with one box which contains all triangles
    if (intersectRayAabb(ray, minBox, maxBox))
    {
        return triangleNumbers;
    }
    return std::vector<int>();
}

void Datastructure::createBoundingBox(const std::vector<Triangle> &triangles)
{
    for (const Triangle &t : triangles)
    {
        minBox = glm::min(minBox, glm::vec3(t.pointOne));
        minBox = glm::min(minBox, glm::vec3(t.pointTwo));
        minBox = glm::min(minBox, glm::vec3(t.pointThree));

        maxBox = glm::max(maxBox, glm::vec3(t.pointOne));
        maxBox = glm::max(maxBox, glm::vec3(t.pointTwo));
        maxBox = glm::max(maxBox, glm::vec3(t.pointThree));
    }
    fillTriangleNumbers(0, triangles.size() - 1); // Fill triangle numbers from 0 to size-1
}

// Test Function, if no bounding box is used
void Datastructure::fillTriangleNumbers(int a, int b)
{
    triangleNumbers.clear(); // Clear existing numbers

    for (int i = a; i <= b; ++i)
    {
        triangleNumbers.push_back(i); // Fill sequentially from a to b
    }
}

bool Datastructure::intersectRayAabb(const Ray &ray, const glm::vec3 &minBox, const glm::vec3 &maxBox)
{
    // Calculate the t values for each pair of planes

    // x-slab intersection t
    float minXT = (minBox.x - ray.origin.x) / ray.direction.x;
    float maxXT = (maxBox.x - ray.origin.x) / ray.direction.x;
    if (minXT > maxXT)
    {
        std::swap(minXT, maxXT);
    }

    float minYT = (minBox.y - ray.origin.y) / ray.direction.y;
    float maxYT = (maxBox.y - ray.origin.y) / ray.direction.y;
    if (minYT > maxYT)
    {
        std::swap(minYT, maxYT);
    }

    // Check if the intervals overlap
    if (maxXT < minYT || maxYT < minXT)
    {
        return false;
    }

    // Now do it for z axis
    if (minYT > minXT)
    {
        minXT = minYT;
    }
    if (maxYT < maxXT)
    {
        maxXT = maxYT;
    }

    float minZT = (minBox.z - ray.origin.z) / ray.direction.z;
    float maxZT = (maxBox.z - ray.origin.z) / ray.direction.z;

    if (minZT > maxZT)
    {
        std::swap(minZT, maxZT);
    }

    if ((minXT > maxZT) || (minZT > maxXT))
    {
        return false;
    }

    return true;
}

std::vector<bool> Lbvh::coordinateToBits(glm::vec3 &coordinate)
{
    glm::ivec3 coordinateInt = glm::ivec3(coordinate);
    std::cout << "Coordinate: " << coordinateInt.x << ", " << coordinateInt.y << ", " << coordinateInt.z << std::endl;
    // Store in 16 bits (bool true = 1, false = 0)
    int bitNumber = 16;
    int bitValue = std::pow(2, bitNumber - 1);
    std::cout << bitValue << "BitVaule:" << std::endl;
    std::vector<bool> bits;
    int bitVala = bitValue % 40000;
    for (int i = 0; i < bitNumber; ++i)
    {
        for (int j = 0; j < 1; ++j)
        {
            if (coordinateInt[j] >= bitValue)
            {
                bits.push_back(true);
                coordinateInt[j] = coordinateInt[j] % bitValue;
            }
            else
            {
                bits.push_back(false);
            }
        }
        bitValue = bitValue / 2;
    }
    // Print all bits as 0 or 1
    std::cout << "Bits: ";
    for (bool bit : bits)
    {
        std::cout << (bit ? 1 : 0) << " ";
    }

    return bits;
}

glm::vec3 Lbvh::centralCoordinates(Triangle &triangle)
{
    glm::vec3 center = (triangle.pointOne + triangle.pointTwo + triangle.pointThree) / 3.0f;
    return center;
}