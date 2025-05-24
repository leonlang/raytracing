#include "Datastructure.h"
#include <glm/gtx/component_wise.hpp>

void Datastructure::initDatastructure(const std::vector<Triangle> &triangles)
{
    // Datastructure 1: Simple intersection with one box which contains all triangles
    createBoundingBox(triangles);
    Lbvh lbvh;
    // glm::vec3 avgSize = lbvh.avgTriangleSize(triangles);
    // int gridSize = lbvh.gridSize(triangles);
    // std::cout << "Grid Size: " << gridSize << std::endl;
    lbvh.createTree(triangles);
    
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

std::bitset<72> Lbvh::coordinateToMorton(glm::vec3 &coordinate)
{
    glm::ivec3 coordinateInt = glm::ivec3(coordinate);
    std::cout << "Coordinate: " << coordinateInt.x << ", " << coordinateInt.y << ", " << coordinateInt.z << std::endl;
    // Store in 16 bits (bool true = 1, false = 0)
    const int bitSize = 24; // Each coordinate will be stored in 24 bits
    const int bitNumber = bitSize * 3; // Total bits for x, y, z coordinates is 72 bits (24 bits each)
    // Create a bitset with 72 bits. Convert each coordinate to 24 bits and then use them to create 72 bit morton code
    std::bitset<bitNumber> bits;
    for (int i = 0; i < 3; ++i) // For each coordinate x, y, z
    {
        std::bitset<24> bits2(coordinateInt[i]); // Create a bitset with 24 bits for each coordinate
        for (int j = 0; j < bitSize; ++j) // For each bit in the coordinate
        {
            if (bits2[j]) // If the bit is set
            {
                bits.set(3 * j + i); // Set the corresponding bit in the 72 bit morton code
            }
            else
            {
                bits.reset(3 * j + i); // Reset the corresponding bit in the 72 bit morton code
            }
        }
    }
    return bits;
}

glm::vec3 Lbvh::centralCoordinates(const Triangle &triangle)
{
    glm::vec3 center = (triangle.pointOne + triangle.pointTwo + triangle.pointThree) / 3.0f;
    return center;
}

float Lbvh::avgTriangleSize(const std::vector<Triangle> &triangles)
{
    glm::vec3 avgSize(0.0f);
    glm::vec3 minBox(0.0f);
    glm::vec3 maxBox(0.0f);
    for (const Triangle &t : triangles)
    {
        // Get Bounding Box and use it to calculate the length in each direction
        minBox = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
        maxBox = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
        minBox = glm::min(minBox, glm::vec3(t.pointOne) / t.pointOne.w);
        minBox = glm::min(minBox, glm::vec3(t.pointTwo) / t.pointTwo.w);
        minBox = glm::min(minBox, glm::vec3(t.pointThree) / t.pointThree.w);
        maxBox = glm::max(maxBox, glm::vec3(t.pointOne) / t.pointOne.w);
        maxBox = glm::max(maxBox, glm::vec3(t.pointTwo) / t.pointTwo.w);
        maxBox = glm::max(maxBox, glm::vec3(t.pointThree)  / t.pointThree.w);       
        glm::vec3 size = maxBox - minBox; // Calculate the size of the triangle
        avgSize += size;
    }
    avgSize /= static_cast<float>(triangles.size());
    return glm::compMax(avgSize);
}

std::pair<float,glm::vec3> Lbvh::gridConstruction(const std::vector<Triangle> &triangles)
{
    Datastructure datastructure;
    datastructure.createBoundingBox(triangles);
    float maxObjectSize = glm::compMax(datastructure.maxBox - datastructure.minBox);
    glm::vec3 minmaxvec = datastructure.maxBox - datastructure.minBox;
    glm::vec3 negativeCoordinates = datastructure.minBox; // Check if bounding box has negative coordinates
    return std::pair<float, glm::vec3>(maxObjectSize, negativeCoordinates);

}

std::vector<Lbvh::mortonTriangle> Lbvh::mortonCodes(const std::vector<Triangle> &triangles)
{
    std::vector<mortonTriangle> mortonTriangles;
    std::pair<float, glm::vec3> gridPair = gridConstruction(triangles);
    float avgTSize = avgTriangleSize(triangles);
    int gridSize = static_cast<int>(std::ceil(gridPair.first / avgTSize));
        // std::cout << "Grid Size: " << gridSizePair.first << ", Average Size: " << gridSizePair.second << std::endl;
   // mortonTriangle mTriangle;
    // mTriangle.bits =  bits2;// Initialize with 24 bits
    for (int i = 0; i < triangles.size(); ++i)
    {
        int gridNumber;

        // int gridNumber = static_cast<int>()
        // centered coordinates of the triangle and made positive should there be negative coordinates
        glm::vec3 centerPositive = centralCoordinates(triangles[i]) - gridPair.second;
        glm::vec3 triangleGridPosition = centerPositive / avgTSize; // Calculate the grid position of the triangle
        std::cout << "Center Positive: " << centerPositive.x << ", " << centerPositive.y << ", " << centerPositive.z << std::endl;
        
        mortonTriangles.push_back({coordinateToMorton(triangleGridPosition),i});
        // print coordinate to bits
        std::cout << "Bits " << coordinateToMorton(triangleGridPosition) << " End" << std::endl;
    }
    return mortonTriangles;
}
void Lbvh::createTree(const std::vector<Triangle> &triangles)
{
    // std::pair<int, float> gridSizePair = gridSize(triangles);
    // Calculate the number of bits needed to represent the grid size
    // const size_t bits_needed = static_cast<size_t>(log2(gridSizePair.first)) + 1;
    mortonCodes(triangles); // Get the morton code for the triangles
    // Create a tree structure based on the triangles
    std::cout << "Creating tree with " << triangles.size() << " triangles." << std::endl;
    

}