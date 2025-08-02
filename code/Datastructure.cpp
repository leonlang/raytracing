#include "Datastructure.h"
#include <glm/gtx/component_wise.hpp>
#include <thread>
#include <chrono>

void traverseAndPrint(Node *node)
{
    if (!node)
        return;

    std::cout << "Node ";
    if (node->oneTriangleLeft)
    {
        std::cout << "[Triangle Index: " << node->triangleIndex << "]\n";
    }
    else
    {
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
    std::cout << "Triangles size " << triangles.size() << std::endl;

    // Datastructure 1: Simple intersection with one box which contains all triangles
    createBoundingBox(triangles);
    fillTriangleNumbers(0, triangles.size() - 1); // Fill triangle numbers from 0 to size-1

    // Sah Datastructure
    Sah sah;
    int bucketCount = 10; // Number of buckets for SAH
    // rootNode = sah.createTree(triangles, triangleNumbers, bucketCount); // Create the SAH tree with 10 buckets
    // std::cout << "SAH: Creating tree with " << triangles.size() << " triangles" << std::endl;

    // Sah Datastructure
    Hlbvh hlbvh;
    int bucketCountHlbvh = 10;
    int sahDepth = 17;
    float changeGridAmountHlbvh = 1.f; // Number of buckets for SAH
    // rootNode = hlbvh.createTree(triangles, triangleNumbers, bucketCount, sahDepth, changeGridAmountHlbvh, 0); // Create the SAH tree with 10 buckets
    std::cout << "SAH: Creating tree with " << triangles.size() << " triangles" << std::endl;

    /* std::cout << sah.sahBucketCost(triangles,triangleNumbers) << std::endl; // Print the cost of the SAH bucket
    // cout all sorted triangle numbers
    std::vector<int> sortedTriangleNumbers = sah.getSortedTriangleNumbers(triangles, triangleNumbers);
    std::cout << std::endl;
    std::pair<std::vector<int>, std::vector<int>> bucketSplit = sah.findBestBucketSplit(triangles, sortedTriangleNumbers, 10); // Find the best bucket split for 10 buckets
    // Print amount of triangles in each bucket:
    std::cout << "Bucket Split: ";
    std::cout << "Left Bucket: " << bucketSplit.first.size() << " triangles, Right Bucket: " << bucketSplit.second.size() << " triangles" << std::endl; */
    // Lbvh Datastructure
    /**/
    Lbvh lbvh;
    // glm::vec3 avgSize = lbvh.avgTriangleSize(triangles);
    // int gridSize = lbvh.gridSize(triangles);
    // std::cout << "Grid Size: " << gridSize << std::endl;
    float changeGridAmount = 1.f;
    rootNode = lbvh.createTree(triangles, changeGridAmount);
    // std::cout << "LBVH: Creating tree with " << triangles.size() << " triangles" << std::endl;

    UniformGrid uniformGrid;
    avgTriangleSize = lbvh.avgTriangleSize(triangles) * 2;
    triangleGridCells = std::move(uniformGrid.trianglesToGridCells(triangles, avgTriangleSize));
    gridCellsIndex = std::move(uniformGrid.gridCellsIndex(triangleGridCells)); // Generate the grid cells index for quick access
    gridBorderMin = uniformGrid.gridBorderMin;
    gridBorderMax = uniformGrid.gridBorderMax;
    gridBorder = uniformGrid.gridBorder;
}

std::vector<int> Datastructure::checkIntersection(const Ray &ray, int &boxCount)
{
    UniformGrid uniformGrid;
    // auto startInit = std::chrono::high_resolution_clock::now();

    std::vector<int> uniformGridCollected = uniformGrid.traverseAndCollectTriangles(triangleGridCells, gridCellsIndex, ray, gridBorderMin, gridBorderMax, gridBorder, avgTriangleSize);
    // return uniformGridCollected;
    // End the timer
    /* auto endInit = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsedInit = endInit - startInit;

    std::cout << "Time taken for OBJ Loading: " << elapsedInit.count() << " seconds " << std::endl; */

    std::vector<int> collectedIndices;

    nodeBoundingBoxIntersection(rootNode, ray, collectedIndices, boxCount);
    /*
    if (!uniformGridCollected.empty())
    {
    std::cout << "show all collected Indices:";
    for (const int &index : collectedIndices)
    {
        std::cout << index << " ";
    }
    std::cout << std::endl;
    std::cout << "show all Uniform Grid collected Indices:";
    for (const int &index : uniformGridCollected)
    {
        std::cout << index << " ";
    }
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    } */

    return uniformGridCollected; // Return the collected indices from the uniform grid traversal
    return collectedIndices;

    /*
    // Simple Check with one bounding box
    if (intersectRayAabb(ray, minBox, maxBox))
    {
        return triangleNumbers;
    }
    return std::vector<int>(); */
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
}

void Datastructure::createBoundingBoxWithNumbers(const std::vector<Triangle> &triangles, const std::vector<int> &triangleNumbers)
{
    minBox = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
    maxBox = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    for (const int &tNumber : triangleNumbers)
    {
        const Triangle &t = triangles[tNumber]; // Get the triangle by its number
        minBox = glm::min(minBox, glm::vec3(t.pointOne) / t.pointOne.w);
        minBox = glm::min(minBox, glm::vec3(t.pointTwo) / t.pointTwo.w);
        minBox = glm::min(minBox, glm::vec3(t.pointThree) / t.pointThree.w);

        maxBox = glm::max(maxBox, glm::vec3(t.pointOne) / t.pointOne.w);
        maxBox = glm::max(maxBox, glm::vec3(t.pointTwo) / t.pointTwo.w);
        maxBox = glm::max(maxBox, glm::vec3(t.pointThree) / t.pointThree.w);
    }
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

std::pair<glm::vec3, glm::vec3> Datastructure::combineBoundingBoxes(std::vector<std::pair<glm::vec3, glm::vec3>> boundingBoxes)
{
    glm::vec3 minBox(FLT_MAX, FLT_MAX, FLT_MAX);
    glm::vec3 maxBox(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    for (const auto &box : boundingBoxes)
    {
        minBox = glm::min(minBox, box.first);
        maxBox = glm::max(maxBox, box.second);
    }

    return std::make_pair(minBox, maxBox);
}

std::bitset<60> Lbvh::coordinateToMorton(glm::vec3 &coordinate)
{
    glm::ivec3 coordinateInt = glm::ivec3(coordinate);
    // std::cout << "Coordinate: " << coordinateInt.x << ", " << coordinateInt.y << ", " << coordinateInt.z << std::endl;
    // Store in 16 bits (bool true = 1, false = 0)
    const int bitSize = 20;            // Each coordinate will be stored in 24 bits
    const int bitNumber = bitSize * 3; // Total bits for x, y, z coordinates
    // Create a bitset with 72 bits. Convert each coordinate to bits and then use them to create 3x bit morton code
    std::bitset<bitNumber> bits;
    for (int i = 0; i < 3; ++i) // For each coordinate x, y, z
    {
        std::bitset<bitSize> bits2(coordinateInt[i]); // Create a bitset for each coordinate
        for (int j = 0; j < bitSize; ++j)             // For each bit in the coordinate
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
        maxBox = glm::max(maxBox, glm::vec3(t.pointThree) / t.pointThree.w);
        glm::vec3 size = maxBox - minBox; // Calculate the size of the triangle
        avgSize += size;
    }
    avgSize /= static_cast<float>(triangles.size());
    // std::cout << "Average Triangle Size: " << avgSize.x << ", " << avgSize.y << ", " << avgSize.z << std::endl;
    return glm::compMax(avgSize);
}

std::pair<float, glm::vec3> Lbvh::gridConstruction(const std::vector<Triangle> &triangles)
{
    Datastructure datastructure;
    datastructure.createBoundingBox(triangles);
    float maxObjectSize = glm::compMax(datastructure.maxBox - datastructure.minBox);
    glm::vec3 minmaxvec = datastructure.maxBox - datastructure.minBox;
    glm::vec3 negativeCoordinates = datastructure.minBox; // Check if bounding box has negative coordinates
    return std::pair<float, glm::vec3>(maxObjectSize, negativeCoordinates);
}

std::vector<Lbvh::mortonTriangle> Lbvh::mortonCodes(const std::vector<Triangle> &triangles, float &changeGridAmount)
{
    std::vector<mortonTriangle> mortonTriangles;
    std::pair<float, glm::vec3> gridPair = gridConstruction(triangles);
    // divides by changeGridAmount to multiply the amount of grid cells by the changeGridAmount
    // A higher changeGridAmount means that the grid cells are smaller, so less triangles are in one grid cell
    float avgTSize = avgTriangleSize(triangles) / changeGridAmount;
    int gridSize = static_cast<int>(std::ceil(gridPair.first / avgTSize));

    // std::cout << "Grid size: " << gridSize << std::endl;
    // mortonTriangle mTriangle;
    // mTriangle.bits =  bits2;// Initialize with 24 bits
    for (int i = 0; i < triangles.size(); ++i)
    {
        int gridNumber;

        // int gridNumber = static_cast<int>()
        // centered coordinates of the triangle and made positive should there be negative coordinates
        glm::vec3 centerPositive = centralCoordinates(triangles[i]) - gridPair.second;
        glm::vec3 triangleGridPosition = centerPositive / avgTSize; // Calculate the grid position of the triangle

        mortonTriangles.push_back({coordinateToMorton(triangleGridPosition), i});
        // print coordinate to bits
        // std::cout << "Bits " << coordinateToMorton(triangleGridPosition) << " End" << std::endl;
    }

    // Sort the morton triangles based on their bits

    // String Sort, for when you need more then 64 bits
    /* std::sort(mortonTriangles.begin(), mortonTriangles.end(), [](const mortonTriangle & firstMorton, const mortonTriangle & secondMorton) {
        return firstMorton.bits.to_string() < secondMorton.bits.to_string();
    }); */
    std::sort(mortonTriangles.begin(), mortonTriangles.end(), [](const mortonTriangle &firstMorton, const mortonTriangle &secondMorton)
              { return firstMorton.bits.to_ulong() < secondMorton.bits.to_ulong(); });

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

Node *Lbvh::createTree(const std::vector<Triangle> &triangles, float &changeGridAmount)
{
    // std::pair<int, float> gridSizePair = gridSize(triangles);
    // Calculate the number of bits needed to represent the grid size
    // const size_t bits_needed = static_cast<size_t>(log2(gridSizePair.first)) + 1;
    std::vector<mortonTriangle> mortonCodeTriangles = mortonCodes(triangles, changeGridAmount); // Get the morton code for the triangles
    std::vector<Node *> nodes;                                                                  // Vector to hold the nodes of the tree
    for (const auto &mt : mortonCodeTriangles)
    {
        nodes.push_back(new Node(mt.index)); // Create a new node for each triangle and add it to the vector
    }
    while (nodes.size() > 1)
    {
        std::vector<Node *> newNodes;

        for (size_t i = 0; i < nodes.size(); i += 2)
        {
            if (i + 1 < nodes.size())
            {
                glm::vec3 minBox;
                glm::vec3 maxBox;
                // Compute min and max bounding box
                if (!nodes[i]->oneTriangleLeft && !nodes[i + 1]->oneTriangleLeft)
                {
                    minBox = glm::min(nodes[i]->minBox, nodes[i + 1]->minBox);
                    maxBox = glm::max(nodes[i]->maxBox, nodes[i + 1]->maxBox);
                }
                // if one node has triangles and the other is a bounding box
                /* else if (nodes[i]->oneTriangleLeft && !nodes[i + 1]->oneTriangleLeft)
                {
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
                else if (!nodes[i]->oneTriangleLeft && nodes[i + 1]->oneTriangleLeft)
                {
                    Datastructure datastructure;
                    std::vector<Triangle> trianglesForBox;

                    trianglesForBox.push_back(triangles[nodes[i + 1]->triangleIndex]);
                    std::cout << "Working" << std::endl;

                    std::cout << "Error: triangleIndex " << nodes[i]->left->triangleIndex << " is out of bounds.\n";
                    std::cout << "Is there another bounding box?" << nodes[i]->left->oneTriangleLeft << std::endl;
                    trianglesForBox.push_back(triangles[nodes[i]->left->triangleIndex]);
                    std::cout << "Not Working" << std::endl;

                    trianglesForBox.push_back(triangles[nodes[i]->right->triangleIndex]);
                    datastructure.createBoundingBox(trianglesForBox);
                    minBox = datastructure.minBox;
                    maxBox = datastructure.maxBox;
                } */
                // if both nodes have triangles, create a new bounding box
                else
                {
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
                Node *parent = new Node(minBox, maxBox);
                parent->left = nodes[i];
                parent->right = nodes[i + 1];

                newNodes.push_back(parent);
            }
            // If there's an odd number of nodes, carry the last one forward
            else
            {
                // If the last node is a triangle, create a new bounding box for it
                // This ensures that the second layer only contains bounding boxes
                if (nodes[i]->oneTriangleLeft)
                {
                    Datastructure datastructure;
                    glm::vec3 minBox;
                    glm::vec3 maxBox;
                    std::vector<Triangle> trianglesForBox;
                    trianglesForBox.push_back(triangles[nodes[i]->triangleIndex]);
                    datastructure.createBoundingBox(trianglesForBox);
                    minBox = datastructure.minBox;
                    maxBox = datastructure.maxBox;
                    Node *parent = new Node(minBox, maxBox);
                    parent->left = nodes[i];
                    parent->right = nodes[i];

                    newNodes.push_back(parent);
                }
                // If the last node is a bounding box, just carry it forward
                else
                {
                    newNodes.push_back(nodes[i]);
                }
            }
        }
        nodes = newNodes;
    }

    std::cout << "LBVH: Creating tree with " << triangles.size() << " triangles" << std::endl;

    Node *root = nodes[0]; // The last remaining node is the root of the tree
    return root;           // Return the root node of the tree
    // traverseAndPrint(root); // Traverse and print the tree structure
}

void Datastructure::nodeBoundingBoxIntersection(Node *node, const Ray &ray, std::vector<int> &collectedIndices, int &boxCount)
{
    if (!node)
        return; // Ensure node is valid

    // Count this bounding box as traversed
    ++boxCount;

    // If it's a leaf node containing a triangle, collect its index
    if (node->oneTriangleLeft)
    {
        collectedIndices.push_back(node->triangleIndex);
        return;
    }

    // If there's no intersection, return early
    if (!intersectRayAabb(ray, node->minBox, node->maxBox))
    {
        return;
    }

    // Recurse for left and right children
    nodeBoundingBoxIntersection(node->left, ray, collectedIndices, boxCount);
    nodeBoundingBoxIntersection(node->right, ray, collectedIndices, boxCount);
}
const float Sah::sahBucketCost(const std::vector<Triangle> &triangles, std::vector<int> &triangleNumbers)
{
    Datastructure datastructure;
    datastructure.createBoundingBoxWithNumbers(triangles, triangleNumbers);
    // Cost of Box is the volume of the bounding box
    float costBox = (datastructure.maxBox.x - datastructure.minBox.x) * (datastructure.maxBox.y - datastructure.minBox.y) * (datastructure.maxBox.z - datastructure.minBox.z);
    float cost = costBox * triangleNumbers.size(); // Cost of the bounding box is the volume times the number of triangles in it
    return cost;
}
const float Sah::sahBucketCostOptimized(std::pair<glm::vec3, glm::vec3> &boundingBox, int triangleCount)
{
    // Cost of Box is the volume of the bounding box
    float costBox = (boundingBox.second.x - boundingBox.first.x) * (boundingBox.second.y - boundingBox.first.y) * (boundingBox.second.z - boundingBox.first.z);
    float cost = costBox * triangleCount; // Cost of the bounding box is the volume times the number of triangles in it
    return cost;
}

std::vector<int> Sah::getSortedTriangleNumbers(const std::vector<Triangle> &triangles, std::vector<int> &triangleNumbers, glm::vec3 &minBox, glm::vec3 &maxBox)
{
    // Calculate the bounding box size in each dimension
    glm::vec3 boxSize = maxBox - minBox;

    if (boxSize.x > boxSize.y && boxSize.x > boxSize.z)
    {
        std::sort(triangleNumbers.begin(), triangleNumbers.end(), [&](int a, int b)
                  { return triangles[a].pointOne.x < triangles[b].pointOne.x; });
    }
    else if (boxSize.y > boxSize.x && boxSize.y > boxSize.z)
    {
        std::sort(triangleNumbers.begin(), triangleNumbers.end(), [&](int a, int b)
                  { return triangles[a].pointOne.y < triangles[b].pointOne.y; });
    }
    else
    {
        std::sort(triangleNumbers.begin(), triangleNumbers.end(), [&](int a, int b)
                  { return triangles[a].pointOne.z < triangles[b].pointOne.z; });
    }

    return triangleNumbers;
}

std::pair<std::vector<int>, std::vector<int>> Sah::findBestBucketSplit(const std::vector<Triangle> &triangles, std::vector<int> &sortedTriangleNumbers, int &bucketCount)
{
    std::vector<int> bucket;
    std::vector<int> leftBucket;
    std::vector<int> rightBucket;
    std::vector<int> bestLeftBucket;
    std::vector<int> bestRightBucket;

    // Initialize the best cost to a large value
    float bestCost = FLT_MAX;

    /*
    // Iterate through possible split points
    // Old option which creates a new bounding box for each split
    for (int i = 1; i < bucketCount; ++i)
    {

        int split = (i * sortedTriangleNumbers.size()) / bucketCount;
        leftBucket = std::vector<int>(sortedTriangleNumbers.begin(), sortedTriangleNumbers.begin() + split);
        rightBucket = std::vector<int>(sortedTriangleNumbers.begin() + split, sortedTriangleNumbers.end());
        // Calculate the cost of the split
        float leftCost = sahBucketCost(triangles, leftBucket);
        float rightCost = sahBucketCost(triangles, rightBucket);

        float totalCost = leftCost + rightCost;
        // If this is the best cost so far, update the best buckets
        if (totalCost < bestCost)
        {
            bestCost = totalCost;
            bestLeftBucket = leftBucket;
            bestRightBucket = rightBucket;
        }
    }
    */
    // Improved Option which first calculates all possible bounding boxes and then merges them for faster performance
    std::vector<std::pair<glm::vec3, glm::vec3>> boundingBoxes(bucketCount); // Vector to hold the bounding boxes for each split
    std::vector<std::vector<int>> bucketNumbers(bucketCount);
    int bestBucketSplit = -1;
    for (int i = 0; i < bucketCount; ++i)
    {
        Datastructure datastructure;
        int begin = (i * sortedTriangleNumbers.size()) / bucketCount;
        int split = ((i + 1) * sortedTriangleNumbers.size()) / bucketCount;
        bucketNumbers[i].assign(sortedTriangleNumbers.begin() + begin, sortedTriangleNumbers.begin() + split);
        datastructure.createBoundingBoxWithNumbers(triangles, bucketNumbers[i]);
        boundingBoxes[i] = {datastructure.minBox, datastructure.maxBox};
    }
    // Precompute bounding box combinations based on the bucket numbers
    std::vector<std::pair<glm::vec3, glm::vec3>> leftBBox(bucketCount);
    std::vector<std::pair<glm::vec3, glm::vec3>> rightBBox(bucketCount);
    leftBBox[0] = boundingBoxes[0];
    for (int i = 1; i < bucketCount; ++i)
        leftBBox[i] = Datastructure().combineBoundingBoxes({leftBBox[i - 1], boundingBoxes[i]});
    rightBBox[bucketCount - 1] = boundingBoxes[bucketCount - 1];
    for (int i = bucketCount - 2; i >= 0; --i)
        rightBBox[i] = Datastructure().combineBoundingBoxes({rightBBox[i + 1], boundingBoxes[i]});

    // Check each split and calculate the best option
    int leftSize = 0;
    for (int i = 1; i < bucketCount; ++i)
    {
        leftSize += bucketNumbers[i - 1].size(); // Increase the left triangle size by the size of the new bucket which is added
        int rightSize = sortedTriangleNumbers.size() - leftSize;

        float leftCost = sahBucketCostOptimized(leftBBox[i - 1], leftSize);
        float rightCost = sahBucketCostOptimized(rightBBox[i], rightSize);
        float totalCost = leftCost + rightCost;

        if (totalCost < bestCost)
        {
            bestCost = totalCost;
            bestBucketSplit = i;
        }
    }
    // Build triangles left and right for the best split and return them
    for (int i = 0; i < bestBucketSplit; ++i)
        bestLeftBucket.insert(bestLeftBucket.end(), bucketNumbers[i].begin(), bucketNumbers[i].end());
    for (int i = bestBucketSplit; i < bucketCount; ++i)
        bestRightBucket.insert(bestRightBucket.end(), bucketNumbers[i].begin(), bucketNumbers[i].end());

    return {bestLeftBucket, bestRightBucket};
}

Node *Sah::createTree(const std::vector<Triangle> &triangles, std::vector<int> &triangleNumbers, int &bucketCount)
{
    if (triangleNumbers.empty())
    {
        // If there are no triangles, return a null node
        std::cout << "No triangles to create a tree, something wrong?" << std::endl;
        return nullptr;
    }
    if (triangleNumbers.size() == 1)
    {
        // If there's only one triangle, create a leaf node with that triangle's index
        // std::cout << "Creating leaf node with triangle index: " << triangleNumbers[0] << std::endl;

        return new Node(triangleNumbers[0]);
    }

    Datastructure datastructure;
    datastructure.createBoundingBoxWithNumbers(triangles, triangleNumbers);
    // If there are only two triangles, create a node with both triangles
    // This improves the performance of the tree because no sorting is needed
    if (triangleNumbers.size() == 2)
    {
        Node *node = new Node(datastructure.minBox, datastructure.maxBox);
        node->left = new Node(triangleNumbers[0]);
        node->right = new Node(triangleNumbers[1]);
        return node;
    }
    // End the timer

    Node *node = new Node(datastructure.minBox, datastructure.maxBox);
    std::vector<int> sortedTriangleNumbers = getSortedTriangleNumbers(triangles, triangleNumbers, datastructure.minBox, datastructure.maxBox); // Get the sorted triangle numbers based on the largest axis
    std::pair<std::vector<int>, std::vector<int>> bucketSplit = findBestBucketSplit(triangles, sortedTriangleNumbers, bucketCount);            // Find the best bucket split for specified bucket count

    // std::cout << "Creating SAH tree with " << triangleNumbers.size() << " triangles" << std::endl;
    // std::cout << "Bucket Split: Left Bucket: " << bucketSplit.first.size() << " triangles, Right Bucket: " << bucketSplit.second.size() << " triangles" << std::endl;
    node->left = createTree(triangles, bucketSplit.first, bucketCount);   // Create the left subtree with the left bucket
    node->right = createTree(triangles, bucketSplit.second, bucketCount); // Create the right subtree with the right bucket

    return node;
}

std::vector<Hlbvh::mortonTriangle> Hlbvh::mortonCodes(const std::vector<Triangle> &triangles, std::vector<int> &triangleNumbers, float &changeGridAmount)
{
    // Mildly changed lbvh version. This Version can use triangle numbers, so not all triangles are needed but just the ones
    // in the current node.
    Lbvh lbvh;
    std::vector<Triangle> trianglesForLbvh;
    for (const int &tNumber : triangleNumbers)
    {
        // std::cout << "Triangles Size: " << triangles.size() << std::endl;
        // std::cout << "Triangle Number: " << tNumber << std::endl;
        trianglesForLbvh.push_back(triangles[tNumber]);
    }
    std::vector<mortonTriangle> mortonTriangles;
    std::pair<float, glm::vec3> gridPair = lbvh.gridConstruction(trianglesForLbvh);
    // divides by changeGridAmount to multiply the amount of grid cells by the changeGridAmount
    // A higher changeGridAmount means that the grid cells are smaller, so less triangles are in one grid cell
    float avgTSize = lbvh.avgTriangleSize(trianglesForLbvh) / changeGridAmount;
    int gridSize = static_cast<int>(std::ceil(gridPair.first / avgTSize));

    // for (int i = 0; i < triangles.size(); ++i)
    for (const int &tNumber : triangleNumbers)
    {
        int gridNumber;

        // int gridNumber = static_cast<int>()
        // centered coordinates of the triangle and made positive should there be negative coordinates
        glm::vec3 centerPositive = lbvh.centralCoordinates(triangles[tNumber]) - gridPair.second;
        glm::vec3 triangleGridPosition = centerPositive / avgTSize; // Calculate the grid position of the triangle

        mortonTriangles.push_back({lbvh.coordinateToMorton(triangleGridPosition), tNumber});
        // print coordinate to bits
        // std::cout << "Bits " << coordinateToMorton(triangleGridPosition) << " End" << std::endl;
    }

    // Sort the morton triangles based on their bits
    std::sort(mortonTriangles.begin(), mortonTriangles.end(), [](const mortonTriangle &firstMorton, const mortonTriangle &secondMorton)
              { return firstMorton.bits.to_ulong() < secondMorton.bits.to_ulong(); });

    return mortonTriangles;
}

Node *Hlbvh::createLbvhTree(const std::vector<Triangle> &triangles, std::vector<int> &triangleNumbers, float &changeGridAmount)
{
    // mildly changed lbvh version. This Version can use triangle numbers, so not all triangles are needed but just the ones
    // in the current node. This wasn't needed in the original LBVH implementation, as all triangles were used to create the full tree.
    // Here I create only part of the tree, after SAH depth is reached.

    // std::pair<int, float> gridSizePair = gridSize(triangles);
    // Calculate the number of bits needed to represent the grid size
    // const size_t bits_needed = static_cast<size_t>(log2(gridSizePair.first)) + 1;
    Lbvh lbvh;
    std::vector<Hlbvh::mortonTriangle> mortonCodeTriangles = mortonCodes(triangles, triangleNumbers, changeGridAmount); // Get the morton code for the triangles
    std::vector<Node *> nodes;                                                                                          // Vector to hold the nodes of the tree
    for (const auto &mt : mortonCodeTriangles)
    {
        nodes.push_back(new Node(mt.index)); // Create a new node for each triangle and add it to the vector
    }
    while (nodes.size() > 1)
    {
        std::vector<Node *> newNodes;

        for (size_t i = 0; i < nodes.size(); i += 2)
        {
            if (i + 1 < nodes.size())
            {
                glm::vec3 minBox;
                glm::vec3 maxBox;
                // Compute min and max bounding box
                if (!nodes[i]->oneTriangleLeft && !nodes[i + 1]->oneTriangleLeft)
                {
                    minBox = glm::min(nodes[i]->minBox, nodes[i + 1]->minBox);
                    maxBox = glm::max(nodes[i]->maxBox, nodes[i + 1]->maxBox);
                }
                // if both nodes have triangles, create a new bounding box
                else
                {
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
                Node *parent = new Node(minBox, maxBox);
                parent->left = nodes[i];
                parent->right = nodes[i + 1];

                newNodes.push_back(parent);
            }
            // If there's an odd number of nodes, carry the last one forward
            else
            {
                // If the last node is a triangle, create a new bounding box for it
                // This ensures that the second layer only contains bounding boxes
                if (nodes[i]->oneTriangleLeft)
                {
                    Datastructure datastructure;
                    glm::vec3 minBox;
                    glm::vec3 maxBox;
                    std::vector<Triangle> trianglesForBox;
                    trianglesForBox.push_back(triangles[nodes[i]->triangleIndex]);
                    datastructure.createBoundingBox(trianglesForBox);
                    minBox = datastructure.minBox;
                    maxBox = datastructure.maxBox;
                    Node *parent = new Node(minBox, maxBox);
                    parent->left = nodes[i];
                    parent->right = nodes[i];

                    newNodes.push_back(parent);
                }
                // If the last node is a bounding box, just carry it forward
                else
                {
                    newNodes.push_back(nodes[i]);
                }
            }
        }
        nodes = newNodes;
    }

    std::cout << "LBVH: Creating tree with " << triangles.size() << " triangles" << std::endl;

    Node *root = nodes[0]; // The last remaining node is the root of the tree
    return root;           // Return the root node of the tree
    // traverseAndPrint(root); // Traverse and print the tree structure
}
Node *Hlbvh::createTree(const std::vector<Triangle> &triangles, std::vector<int> &triangleNumbers, int &bucketCount, int &sahDepth, float &changeGridAmount, int sahCurrentDepth)
{
    if (triangleNumbers.empty())
    {
        // If there are no triangles, return a null node
        std::cout << "No triangles to create a tree, something wrong?" << std::endl;
        return nullptr;
    }
    if (triangleNumbers.size() == 1)
    {
        // If there's only one triangle, create a leaf node with that triangle's index
        // std::cout << "Creating leaf node with triangle index: " << triangleNumbers[0] << std::endl;

        return new Node(triangleNumbers[0]);
    }

    Datastructure datastructure;
    datastructure.createBoundingBoxWithNumbers(triangles, triangleNumbers);
    // If there are only two triangles, create a node with both triangles
    // This improves the performance of the tree because no sorting is needed
    if (triangleNumbers.size() == 2)
    {
        Node *node = new Node(datastructure.minBox, datastructure.maxBox);
        node->left = new Node(triangleNumbers[0]);
        node->right = new Node(triangleNumbers[1]);
        return node;
    }
    // End the timer

    Node *node = new Node(datastructure.minBox, datastructure.maxBox);
    // If the current depth is greater than the sah depth, create a LBVH tree
    if (sahCurrentDepth >= sahDepth)
    {
        return createLbvhTree(triangles, triangleNumbers, changeGridAmount);
    }
    Sah sah;
    std::vector<int> sortedTriangleNumbers = sah.getSortedTriangleNumbers(triangles, triangleNumbers, datastructure.minBox, datastructure.maxBox); // Get the sorted triangle numbers based on the largest axis
    std::pair<std::vector<int>, std::vector<int>> bucketSplit = sah.findBestBucketSplit(triangles, sortedTriangleNumbers, bucketCount);            // Find the best bucket split for specified bucket count

    // std::cout << "Creating SAH tree with " << triangleNumbers.size() << " triangles" << std::endl;
    // std::cout << "Bucket Split: Left Bucket: " << bucketSplit.first.size() << " triangles, Right Bucket: " << bucketSplit.second.size() << " triangles" << std::endl;
    node->left = createTree(triangles, bucketSplit.first, bucketCount, sahDepth, changeGridAmount, sahCurrentDepth + 1);   // Create the left subtree with the left bucket
    node->right = createTree(triangles, bucketSplit.second, bucketCount, sahDepth, changeGridAmount, sahCurrentDepth + 1); // Create the right subtree with the right bucket

    return node;
}
std::pair<glm::ivec3, glm::ivec3> UniformGrid::gridCellsFromTriangle(const Triangle &triangle, const float &avgTriangleSize)
{
    glm::vec3 minBox(0.0f);
    glm::vec3 maxBox(0.0f);
    // Get Bounding Box of Triangle and use it to calculate in which grid cell it is located
    minBox = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
    maxBox = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    minBox = glm::min(minBox, glm::vec3(triangle.pointOne) / triangle.pointOne.w);
    minBox = glm::min(minBox, glm::vec3(triangle.pointTwo) / triangle.pointTwo.w);
    minBox = glm::min(minBox, glm::vec3(triangle.pointThree) / triangle.pointThree.w);
    maxBox = glm::max(maxBox, glm::vec3(triangle.pointOne) / triangle.pointOne.w);
    maxBox = glm::max(maxBox, glm::vec3(triangle.pointTwo) / triangle.pointTwo.w);
    maxBox = glm::max(maxBox, glm::vec3(triangle.pointThree) / triangle.pointThree.w);

    glm::ivec3 cellBeginning = glm::floor(minBox / avgTriangleSize);
    // it works with floor with the examples I used for now. If weird artifacts appear, try ceil
    // but ceil increases time complexity by 4
    glm::ivec3 cellEnd = glm::floor(maxBox / avgTriangleSize);
    return std::make_pair(cellBeginning, cellEnd);
}
int UniformGrid::trianglesCellsCount(const std::vector<Triangle> &triangles, const float &avgTriangleSize)
{

    Lbvh lbvh;
    int cellCount = 0;
    // go through all triangles and count the number of grid cells they occupy
    for (const Triangle &triangle : triangles)
    {
        std::pair<glm::ivec3, glm::ivec3> cells = gridCellsFromTriangle(triangle, avgTriangleSize);
        // Example: Cell 5 to 7 are occupied, this would be 5,6,7 = 3 cells = 7 + 1 - 5
        cellCount += (cells.second.x + 1 - cells.first.x) * (cells.second.y + 1 - cells.first.y) * (cells.second.z + 1 - cells.first.z);
        gridBorderMin = glm::min(gridBorderMin, cells.first);
        gridBorderMax = glm::max(gridBorderMax, cells.second);
    }
    // Calculate the grid border and use the highest value in each direction
    // so the grid border stretches the same in both negative and positive direction
    /* gridBorder.x = std::max(abs(gridBorderMin.x), abs(gridBorderMax.x));
    gridBorder.y = std::max(abs(gridBorderMin.y), abs(gridBorderMax.y));
    gridBorder.z = std::max(abs(gridBorderMin.z), abs(gridBorderMax.z)); */
    gridBorder = gridBorderMax - gridBorderMin + 1;
    std::cout << "Grid Border: " << gridBorder.x << ", " << gridBorder.y << ", " << gridBorder.z << std::endl;
    return cellCount;
}

std::vector<int> UniformGrid::gridCellsIndex(std::vector<std::pair<int, int>> &triangleGridCells)
{
    int counter = 0;
    int cellIndexCounter = 0;
    int lastNumber = -1;
    std::vector<int> cellIndex(gridBorder.x + gridBorder.x * gridBorder.y + gridBorder.x * gridBorder.y * gridBorder.z + 1, -1);
    for (const auto &cell : triangleGridCells)
    {
        // std::cout << "Cell: " << cell.first << ", Index: " << cell.second << std::endl;

        if (cell.first != lastNumber)
        {
            // store where a new cellNumber starts in the cellIndex vector
            // This is used to quickly find the start of a cell in the triangleGridCells vector
            cellIndex[cell.first] = counter;
            // cellIndexCounter += 1;
            lastNumber = cell.first; // Update the last number to the current cell number
        }
        counter += 1;
    }
    return cellIndex;
}
std::vector<std::pair<int, int>> UniformGrid::trianglesToGridCells(const std::vector<Triangle> &triangles, const float &avgTriangleSize)
{
    // Initialize vector and allocate space for the grid cells
    std::vector<std::pair<int, int>> gridCells(trianglesCellsCount(triangles, avgTriangleSize));
    int counter = 0;
    int normalized_x = gridBorderMin.x;
    int normalized_y = gridBorderMin.y;
    int normalized_z = gridBorderMin.z;
    int counterCells = 0;
    for (const Triangle &triangle : triangles)
    {
        std::pair<glm::ivec3, glm::ivec3> cells = gridCellsFromTriangle(triangle, avgTriangleSize);
        for (int x = cells.first.x; x <= cells.second.x; ++x)
        {
            for (int y = cells.first.y; y <= cells.second.y; ++y)
            {
                for (int z = cells.first.z; z <= cells.second.z; ++z)
                {
                    // Normalize the grid Cells to start from 0,0,0
                    int dx = x - normalized_x;
                    int dy = y - normalized_y;
                    int dz = z - normalized_z;
                    // Calculate the index in the gridCells vector
                    int index = dx + dy * gridBorder.x + gridBorder.y * gridBorder.x * dz;
                    gridCells[counterCells] = std::make_pair(index, counter);
                    counterCells += 1; // Increment the counter for each cell
                }
            }
        }
        counter += 1; // Increment the counter for each triangle
    }
    // Sorting the grid cells
    std::sort(gridCells.begin(), gridCells.end(), [](const std::pair<int, int> &a, const std::pair<int, int> &b)
              { return a.first < b.first; });
    return gridCells;
}

std::vector<int> UniformGrid::traverseAndCollectTriangles(std::vector<std::pair<int, int>> &trianglesToGridCells, std::vector<int> &gridCellsIndex, const Ray &ray, glm::ivec3 &gridBorderMin, glm::ivec3 &gridBorderMax, glm::ivec3 &gridBorder, float &avgTriangleSize)
{
    // auto startInit1 = std::chrono::high_resolution_clock::now();

    std::vector<int> collectedTriangles; // Vector to hold the collected triangle indices
    // Initialize all needed variables
    glm::ivec3 currentCell = glm::floor(ray.origin / avgTriangleSize); // values i,j and k which show current cell
    currentCell -= gridBorderMin;                                      // Normalize the current cell to the grid border
    glm::dvec3 normalizedRayDirection = glm::normalize(ray.direction); // Normalize the ray direction
    glm::dvec3 delta; // values deltaX deltaY and deltaZ
    delta.x = normalizedRayDirection.x != 0.0 ? static_cast<double>(avgTriangleSize) / std::abs(normalizedRayDirection.x)
                                              : std::numeric_limits<double>::infinity();
    delta.y = normalizedRayDirection.y != 0.0 ? static_cast<double>(avgTriangleSize) / std::abs(normalizedRayDirection.y)
                                              : std::numeric_limits<double>::infinity();
    delta.z = normalizedRayDirection.z != 0.0 ? static_cast<double>(avgTriangleSize) / std::abs(normalizedRayDirection.z)
                                              : std::numeric_limits<double>::infinity();
    glm::ivec3 p = glm::sign(ray.direction); // values pX pY and pZ, sign returns -1, 0 or 1 depending on the sign of the value
    // Calculates the next boundary in each direction. when the direction is positive, it adds 1 to the current cell, when negative it adds nothing
    glm::dvec3 nextBoundary = (glm::dvec3(currentCell + gridBorderMin) + glm::dvec3(p.x > 0, p.y > 0, p.z > 0)) * static_cast<double>(avgTriangleSize);
    glm::dvec3 d; // values dX dY and dZ
    d.x = normalizedRayDirection.x != 0.0 ? (nextBoundary.x - ray.origin.x) / normalizedRayDirection.x
                                          : std::numeric_limits<double>::infinity();
    d.y = normalizedRayDirection.y != 0.0 ? (nextBoundary.y - ray.origin.y) / normalizedRayDirection.y
                                          : std::numeric_limits<double>::infinity();
    d.z = normalizedRayDirection.z != 0.0 ? (nextBoundary.z - ray.origin.z) / normalizedRayDirection.z
                                          : std::numeric_limits<double>::infinity();
    /* std::cout << "Grid Border Min: " << gridBorderMin.x << ", " << gridBorderMin.y << ", " << gridBorderMin.z << std::endl;
    std::cout << "Grid Border Max: " << gridBorderMax.x << ", " << gridBorderMax.y << ", " << gridBorderMax.z << std::endl;
    std::cout << "Grid Border: " << gridBorder.x << ", " << gridBorder.y << ", " << gridBorder.z << std::endl;
    std::cout << "Current Cell: " << currentCell.x << ", " << currentCell.y << ", " << currentCell.z << std::endl; */
    // Start traversing the grid cells
    // the currentCell is normalized, so it goes until the normalized grid border. After this there are no more grid cells to traverse

    // if p.x  is negative then currentCell.x should be checked against 0 and not against gridBorder.x (Same for y and z)
    int borderX1 = (p.x < 0) ? 0 : INT_MIN;
    int borderX2 = (p.x < 0) ? INT_MAX : gridBorder.x;

    int borderY1 = (p.y < 0) ? 0 : INT_MIN;
    int borderY2 = (p.y < 0) ? INT_MAX : gridBorder.y;

    int borderZ1 = (p.z < 0) ? 0 : INT_MIN;
    int borderZ2 = (p.z < 0) ? INT_MAX : gridBorder.z;

    /*
    std::cout << "Average Triangle Size: " << avgTriangleSize << std::endl;
    std::cout << "Normalized Ray Direction: " << normalizedRayDirection.x << ", " << normalizedRayDirection.y << ", " << normalizedRayDirection.z << std::endl;
    std::cout << "Delta: " << delta.x << ", " << delta.y << ", " << delta.z << std::endl;
    std::cout << "Next Boundary: " << nextBoundary.x << ", " << nextBoundary.y << ", " << nextBoundary.z << std::endl;
    std::cout << "d" << d.x << ", " << d.y << ", " << d.z << std::endl; */

    // auto endInit1 = std::chrono::high_resolution_clock::now();
    // std::chrono::duration<double> elapsedInit1 = endInit1 - startInit1;
    // std::cout << "Time taken for Start Loading: " << elapsedInit1.count() << " seconds " << std::endl;
    // auto startInit = std::chrono::high_resolution_clock::now();

    while (currentCell.x >= borderX1 && currentCell.x <= borderX2 && currentCell.y >= borderY1 && currentCell.y <= borderY2 && currentCell.z >= borderZ1 && currentCell.z <= borderZ2)
    {
        // std::cout << "Current Cell: " << currentCell.x << ", " << currentCell.y << ", " << currentCell.z << std::endl;

        if (d.x <= d.y && d.x <= d.z)
        {

            currentCell.x += p.x; // Move to the next cell in the x direction
            d.x += delta.x;       // Update the distance to the next boundary in the x direction
        }
        else if (d.y <= d.x && d.y <= d.z)
        {

            currentCell.y += p.y; // Move to the next cell in the y direction
            d.y += delta.y;       // Update the distance to the next boundary in the y direction
        }
        else if (d.z <= d.x && d.z <= d.y)
        {
            currentCell.z += p.z; // Move to the next cell in the z direction
            d.z += delta.z;       // Update the distance to the next boundary in the z direction
        }
        // Check if the normalized current cell is within the grid.
        // if not then I don't have to check for triangles
        // when it isn't within the grid this means that the ray was initialized outside of the grid
        // so now the algorithm continues until it is inside the grid
        if (currentCell.x < 0 || currentCell.y < 0 || currentCell.z < 0 || currentCell.x > gridBorder.x || currentCell.y > gridBorder.y || currentCell.z > gridBorder.z)
        {
            continue; // Skip to the next iteration if the cell is out of bounds
        }
        // std::cout << "Current Cell1: " << currentCell.x << ", " << currentCell.y << ", " << currentCell.z << std::endl;
        /* if (currentCell.x + currentCell.y * gridBorder.x + currentCell.z * gridBorder.x * gridBorder.y >= gridCellsIndex.size() || currentCell.x + currentCell.y * gridBorder.x + currentCell.z * gridBorder.x * gridBorder.y < 0)
        {
            std::cout << "Error: startI is out of bounds: " << currentCell.x + currentCell.y * gridBorder.x + currentCell.z * gridBorder.x * gridBorder.y << " for trianglesToGridCells size: " << trianglesToGridCells.size() << std::endl;
            std::cout << "Current Cell: " << currentCell.x << ", " << currentCell.y << ", " << currentCell.z << std::endl;
            std::cout << "Grid Border: " << gridBorder.x << ", " << gridBorder.y << ", " << gridBorder.z << std::endl;
            std::cout << "Grid Cells Index Size: " << gridCellsIndex.size() << std::endl;
            std::cout << "Triangles To Grid Cells Size: " << trianglesToGridCells.size() << std::endl;
            std::cout << "Current Cell Index: " << currentCell.x + currentCell.y * gridBorder.x + currentCell.z * gridBorder.x * gridBorder.y << std::endl;
            std::cout << "Current Cell Index: " << currentCell.x + currentCell.y * gridBorder.x + currentCell.z * gridBorder.x * gridBorder.y << std::endl;
            std::cout << "Current Cell: " << currentCell.x << ", " << currentCell.y << ", " << currentCell.z << std::endl;
            std::cout << "Grid Border: " << gridBorder.x << ", " << gridBorder.y << ", " << gridBorder.z << std::endl;
            continue; // Skip to the next iteration if the start index is out of bounds
        } */
        int startI = gridCellsIndex[currentCell.x + currentCell.y * gridBorder.x + currentCell.z * gridBorder.x * gridBorder.y]; // Get the start index of the current cell in the gridCellsIndex vector

        if (startI == -1)
        {
            continue; // If the grid cells Index is -1, skip to the next iteration because this means that there are no triangles in this grid cell
        }
        int gridValue = trianglesToGridCells[startI].first;

        while (gridValue == trianglesToGridCells[startI].first)
        {
            // Collect the triangle index from the grid cell
            collectedTriangles.emplace_back(trianglesToGridCells[startI].second); // Add the triangle index to the collected triangles

            startI += 1; // Increment the start index until the grid value changes
            if (startI >= trianglesToGridCells.size())
            {
                break; // End Search if the start index is out of bounds
            }
        }
    }
    // auto endInit = std::chrono::high_resolution_clock::now();
    // std::chrono::duration<double> elapsedInit = endInit - startInit;
    // std::cout << "Time taken for Next Loading: " << elapsedInit.count() << " seconds " << std::endl;


    // std::cout << "Skipping cell with no triangles: " << currentCell.x << ", " << currentCell.y << ", " << currentCell.z << std::endl;
    return collectedTriangles; // Return an empty vector for now, as this function is not yet implemented
}
