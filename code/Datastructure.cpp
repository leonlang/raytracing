#include "Datastructure.h"
#include <glm/gtx/component_wise.hpp>


void traverseAndPrint(Node* node) {
    if (!node) return;

    std::cout << "Node ";
    if (node->hasTriangles) {
        std::cout << "[Triangle Index: " << node->triangleIndex << "]\n";
    } else {
        std::cout << "[Bounding Box] Min: (" 
                  << node->minBox.x << ", " << node->minBox.y << ", " << node->minBox.z 
                  << ") Max: (" << node->maxBox.x << ", " << node->maxBox.y << ", " << node->maxBox.z 
                  << ")\n";
    }

    // Recursively traverse left and right children
    traverseAndPrint(node->left);
    traverseAndPrint(node->right);
}

void Datastructure::initDatastructure(const std::vector<Triangle> &triangles)
{
    // Datastructure 1: Simple intersection with one box which contains all triangles
    createBoundingBox(triangles);
    Lbvh lbvh;
    // glm::vec3 avgSize = lbvh.avgTriangleSize(triangles);
    // int gridSize = lbvh.gridSize(triangles);
    // std::cout << "Grid Size: " << gridSize << std::endl;
    rootNode = lbvh.createTree(triangles);
    
}

std::vector<int> Datastructure::checkIntersection(const Ray &ray)
{
    // Datastructure 1: Simple intersection with one box which contains all triangles

    // traverseAndPrint(rootNode);
    // Traverse tree for Lbvh and ...

    std::vector<int> collectedIndices;
    nodeBoundingBoxIntersection(rootNode, ray, collectedIndices);
    return collectedIndices;


    // Simple Check with one bounding box
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

std::bitset<60> Lbvh::coordinateToMorton(glm::vec3 &coordinate)
{
    glm::ivec3 coordinateInt = glm::ivec3(coordinate);
    // std::cout << "Coordinate: " << coordinateInt.x << ", " << coordinateInt.y << ", " << coordinateInt.z << std::endl;
    // Store in 16 bits (bool true = 1, false = 0)
    const int bitSize = 20; // Each coordinate will be stored in 24 bits
    const int bitNumber = bitSize * 3; // Total bits for x, y, z coordinates 
    // Create a bitset with 72 bits. Convert each coordinate to bits and then use them to create 3x bit morton code
    std::bitset<bitNumber> bits;
    for (int i = 0; i < 3; ++i) // For each coordinate x, y, z
    {
        std::bitset<bitSize> bits2(coordinateInt[i]); // Create a bitset for each coordinate
        for (int j = 0; j < bitSize; ++j) // For each bit in the coordinate
        {
            if (bits2[j]) // If the bit is set
            {
                bits.set(3 * j + i); // Set the corresponding bit in the  morton code
            }
            else
            {
                bits.reset(3 * j + i); // Reset the corresponding bit in the morton code
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

std::vector<Lbvh::mortonTriangle> Lbvh::mortonCodes(const std::vector<Triangle> &triangles,float changeGridAmount)
{
    std::vector<mortonTriangle> mortonTriangles;
    std::pair<float, glm::vec3> gridPair = gridConstruction(triangles);
    // divides by changeGridAmount to multiply the amount of grid cells by the changeGridAmount
    // A higher changeGridAmount means that the grid cells are smaller, so less triangles are in one grid cell
    float avgTSize = avgTriangleSize(triangles) / changeGridAmount;
    int gridSize = static_cast<int>(std::ceil(gridPair.first / avgTSize));

    std::cout << "Grid Size: " << gridSize << std::endl;
   // mortonTriangle mTriangle;
    // mTriangle.bits =  bits2;// Initialize with 24 bits
    for (int i = 0; i < triangles.size(); ++i)
    {
        int gridNumber;

        // int gridNumber = static_cast<int>()
        // centered coordinates of the triangle and made positive should there be negative coordinates
        glm::vec3 centerPositive = centralCoordinates(triangles[i]) - gridPair.second;
        glm::vec3 triangleGridPosition = centerPositive / avgTSize; // Calculate the grid position of the triangle
        
        mortonTriangles.push_back({coordinateToMorton(triangleGridPosition),i});
        // print coordinate to bits
        // std::cout << "Bits " << coordinateToMorton(triangleGridPosition) << " End" << std::endl;
    }


    // Sort the morton triangles based on their bits

    // String Sort, for when you need more then 64 bits
    /* std::sort(mortonTriangles.begin(), mortonTriangles.end(), [](const mortonTriangle & firstMorton, const mortonTriangle & secondMorton) {
        return firstMorton.bits.to_string() < secondMorton.bits.to_string();
    }); */
    std::sort(mortonTriangles.begin(), mortonTriangles.end(), [](const mortonTriangle& firstMorton, const mortonTriangle& secondMorton) {
        return firstMorton.bits.to_ulong() < secondMorton.bits.to_ulong();
    });


    /* for (const auto& mt : mortonTriangles) {
        std::cout << mt.bits << " - Index: " << mt.index << std::endl;
        // Print center coordinates of the triangle with same index
        glm::vec3 center = centralCoordinates(triangles[mt.index]) - gridPair.second; // Adjust center coordinates by subtracting the negative offset
        std::cout << "Center Coordinates: " << (int)center.x << ", " << (int)center.y << ", " << (int)center.z << std::endl;
        glm::vec3 triangleGridPosition = center / avgTSize; // Calculate the grid position of the triangle
        std::cout << "Triangle Grid Position: " << (int)triangleGridPosition.x << ", " << (int)triangleGridPosition.y << ", " << (int)triangleGridPosition.z << std::endl;
        std::cout << avgTSize << std::endl;
    } */

    return mortonTriangles;
}





Node* Lbvh::createTree(const std::vector<Triangle> &triangles)
{
    // std::pair<int, float> gridSizePair = gridSize(triangles);
    // Calculate the number of bits needed to represent the grid size
    // const size_t bits_needed = static_cast<size_t>(log2(gridSizePair.first)) + 1;
    std::vector<mortonTriangle> mortonCodeTriangles  =  mortonCodes(triangles,100.f); // Get the morton code for the triangles
    std::vector<Node*> nodes; // Vector to hold the nodes of the tree
    for (const auto& mt : mortonCodeTriangles) {
        nodes.push_back(new Node(mt.index)); // Create a new node for each triangle and add it to the vector
    }

    while (nodes.size() > 1) {
        std::vector<Node*> newNodes;

        for (size_t i = 0; i < nodes.size(); i += 2) {
            if (i + 1 < nodes.size()) {
                glm::vec3 minBox;
                glm::vec3 maxBox;
                // Compute min and max bounding box
                if (!nodes[i]->hasTriangles && !nodes[i + 1]->hasTriangles) {
                    minBox = glm::min(nodes[i]->minBox, nodes[i + 1]->minBox);
                    maxBox = glm::max(nodes[i]->maxBox, nodes[i + 1]->maxBox);
                } 
                // if one node has triangles and the other is a bounding box
                else if (nodes[i]->hasTriangles && !nodes[i + 1]->hasTriangles) {
                    Datastructure datastructure;
                    std::vector<Triangle> trianglesForBox;
                    trianglesForBox.push_back(triangles[nodes[i]->triangleIndex]);
                    trianglesForBox.push_back(triangles[nodes[i + 1]->left->triangleIndex]); 
                    trianglesForBox.push_back(triangles[nodes[i + 1]->right->triangleIndex]);
                    datastructure.createBoundingBox(trianglesForBox);
                    minBox = datastructure.minBox;
                    maxBox = datastructure.maxBox;
                } 
                // if the other node has triangles and the first is a bounding box
                else if (!nodes[i]->hasTriangles && nodes[i + 1]->hasTriangles) {
                    Datastructure datastructure;
                    std::vector<Triangle> trianglesForBox;
                    trianglesForBox.push_back(triangles[nodes[i + 1]->triangleIndex]);
                    trianglesForBox.push_back(triangles[nodes[i]->left->triangleIndex]);
                    trianglesForBox.push_back(triangles[nodes[i]->right->triangleIndex]);
                    datastructure.createBoundingBox(trianglesForBox);
                    minBox = datastructure.minBox;
                    maxBox = datastructure.maxBox;
                }
                // if both nodes have triangles, create a new bounding box
                else {
                    Datastructure datastructure;
                    std::vector<Triangle> trianglesForBox;
                    trianglesForBox.push_back(triangles[nodes[i]->triangleIndex]);
                    trianglesForBox.push_back(triangles[nodes[i + 1]->triangleIndex]);
                    datastructure.createBoundingBox(trianglesForBox);
                    minBox = datastructure.minBox;
                    maxBox = datastructure.maxBox;
                    // If both nodes are bounding boxes, create a new bounding box
                }

                // Create parent node
                Node* parent = new Node(minBox, maxBox);
                parent->left = nodes[i];
                parent->right = nodes[i+1];

                newNodes.push_back(parent);
            } else {
                // If there's an odd number of nodes, carry the last one forward
                newNodes.push_back(nodes[i]);
            }
        }
        nodes = newNodes;
    }

    std::cout << "Creating tree with " << triangles.size() << " triangles." << std::endl;


    Node* root = nodes[0]; // The last remaining node is the root of the tree
    return root; // Return the root node of the tree
    // traverseAndPrint(root); // Traverse and print the tree structure

}


void Datastructure::nodeBoundingBoxIntersection(Node* node, const Ray& ray, std::vector<int>& collectedIndices) {
    if (!node) return; // Ensure node is valid

    // If it's a leaf node containing a triangle, collect its index
    if (node->hasTriangles) {
        collectedIndices.push_back(node->triangleIndex);
        return;
    }

    // If there's no intersection, return early
    if (!intersectRayAabb(ray, node->minBox, node->maxBox)) {
        return;
    }

    // Recurse for left and right children
    nodeBoundingBoxIntersection(node->left, ray, collectedIndices);
    nodeBoundingBoxIntersection(node->right, ray, collectedIndices);
}