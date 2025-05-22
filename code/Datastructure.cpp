#include "Datastructure.h"
#include <glm/gtx/component_wise.hpp>

void Datastructure::initDatastructure(const std::vector<Triangle> &triangles)
{
    // Datastructure 1: Simple intersection with one box which contains all triangles
    createBoundingBox(triangles);
    Lbvh lbvh;
    // glm::vec3 avgSize = lbvh.avgTriangleSize(triangles);
    int gridSize = lbvh.gridSize(triangles);
    std::cout << "Grid Size: " << gridSize << std::endl;
    
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
    minBox = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
    maxBox = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    for (const Triangle &t : triangles)
    {
        minBox = glm::min(minBox, glm::vec3(t.pointOne) / t.pointOne.w);
        minBox = glm::min(minBox, glm::vec3(t.pointTwo) / t.pointTwo.w);
        minBox = glm::min(minBox, glm::vec3(t.pointThree) / t.pointThree.w);

        maxBox = glm::max(maxBox, glm::vec3(t.pointOne) / t.pointOne.w);
        maxBox = glm::max(maxBox, glm::vec3(t.pointTwo) / t.pointTwo.w);
        maxBox = glm::max(maxBox, glm::vec3(t.pointThree) / t.pointThree.w);
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

glm::vec3 Lbvh::avgTriangleSize(const std::vector<Triangle> &triangles)
{
    glm::vec3 avgSize(0.0f);
    glm::vec3 minBox(0.0f);
    glm::vec3 maxBox(0.0f);
    for (const Triangle &t : triangles)
    {
        minBox = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
        maxBox = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
        // Get Bounding Box and use it to calculate the length in each direction
        minBox = glm::min(minBox, glm::vec3(t.pointOne) / t.pointOne.w);
        minBox = glm::min(minBox, glm::vec3(t.pointTwo) / t.pointTwo.w);
        minBox = glm::min(minBox, glm::vec3(t.pointThree) / t.pointThree.w);
        maxBox = glm::max(maxBox, glm::vec3(t.pointOne) / t.pointOne.w);
        maxBox = glm::max(maxBox, glm::vec3(t.pointTwo) / t.pointTwo.w);
        maxBox = glm::max(maxBox, glm::vec3(t.pointThree)  / t.pointThree.w);       
        glm::vec3 size = maxBox - minBox; // Calculate the size of the triangle
        // std::cout << "Size: " << size.x << ", " << size.y << ", " << size.z << std::endl;
        avgSize += size;
    }
    avgSize /= static_cast<float>(triangles.size());
    return avgSize;
}

int Lbvh::gridSize(const std::vector<Triangle> &triangles)
{
    glm::vec3 avgSize = avgTriangleSize(triangles);
    std::cout << "AvgSize: " << avgSize.x << ", " << avgSize.y << ", " << avgSize.z << std::endl;
    Datastructure datastructure;
    datastructure.createBoundingBox(triangles);



    glm::vec3 minmaxvec = datastructure.maxBox - datastructure.minBox;
    std::cout << "MinMaxVec: " << minmaxvec.x << ", " << minmaxvec.y << ", " << minmaxvec.z << std::endl;

    glm::vec3 gridSizeVec = minmaxvec / avgSize;
    float maxGridValue = std::max({gridSizeVec.x, gridSizeVec.y, gridSizeVec.z});
    std::cout << "MaxGridValue: " << maxGridValue << std::endl;
    return static_cast<int>(std::ceil(maxGridValue));

}