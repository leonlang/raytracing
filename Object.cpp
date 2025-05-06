#include "Object.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" // Include an image loading library like stb_image


// Default Triangle constructor  
Triangle::Triangle() 
    : pointOne(0.0f), pointTwo(0.0f), pointThree(0.0f), normalOne(0.0f), normalTwo(0.0f), normalThree(0.0f) {}

// Parameterized Triangle constructor 
Triangle::Triangle(glm::vec4 p1, glm::vec4 p2, glm::vec4 p3, glm::vec3 n1, glm::vec3 n2, glm::vec3 n3)
    : pointOne(p1), pointTwo(p2), pointThree(p3), normalOne(n1), normalTwo(n2), normalThree(n3) {}


// Ray class implementation 
Ray::Ray(glm::vec3 d) 
    : origin(0.0f, 0.0f, 0.0f), direction(d) {}

// ObjObject::ObjObject() = default;

// Complex Triangle Structures with OBJ
// Uses Library tinyobjloader
// https://github.com/tinyobjloader/tinyobjloader
// Code is from readme File (Example code (New Object Oriented API)) and slightly modified to fit my implementation
void ObjectManager::loadObjFile(const std::string&objName ,const std::string& objFilename) {
    ObjObject obj;
    tinyobj::ObjReader reader;
    obj.triangleStartIndex = triangles.size();

    
    /*
    // default parameters for objects
    float ambientStrength = 0.2f;
    float specularStrength = 0.5f;
    float shininess = 15.0f;

    obj.objProperties = glm::vec3(ambientStrength, specularStrength, shininess);
    */

    if (!reader.ParseFromFile(objFilename)) {
        if (!reader.Error().empty()) {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
    } 
    
    if (!reader.Warning().empty()) {
         std::cout << "TinyObjReader: " << reader.Warning();
    }
    

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    // std::vector<Triangle> triangles;


    // New
    // std::unordered_map<std::string, unsigned char*> textureData;
    // std::unordered_map<std::string, glm::ivec2> textureDimensions;
    for (const auto& material : materials) {
        if (!material.diffuse_texname.empty()) {
            int width, height, channels;
            std::string texturePath = material.diffuse_texname;
            if (textureData.find(texturePath) == textureData.end()) {
                unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &channels, 3);
                // std::cout << material.diffuse_texname;
                if (data) {
                    textureData[texturePath] = data;
                    textureDimensions[texturePath] = glm::ivec2(width, height);
                }
                else {
                    std::cerr << "Failed to load texture: " << texturePath << std::endl;
                }
            }
        }
    }


    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
        // Loop over faces (polygons)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

            // Loop over vertices in the face.
            Triangle tria;
            for (size_t v = 0; v < fv; v++) {
                if (fv == 3) {
                    std::string texturePath;
                    glm::vec2 texCoordinate(0,0);
                    glm::vec4 vertex(0.0f, 0.0f, 0.0f, 1.0f);
                    glm::vec3 vnormal(0.0f, 0.0f, 0.0f);
                    // access to vertex
                    tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                    vertex.x = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                    vertex.y = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                    vertex.z = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

                    // Check if `normal_index` is zero or positive. negative = no normal data
                    if (idx.normal_index >= 0) {
                        vnormal.x = attrib.normals[3 * size_t(idx.normal_index) + 0];
                        vnormal.y = attrib.normals[3 * size_t(idx.normal_index) + 1];
                        vnormal.z = attrib.normals[3 * size_t(idx.normal_index) + 2];
                    }

                    // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                    if (idx.texcoord_index >= 0) {
                        tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                        tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];

                        // Use Texture for Color
                        int materialID = shapes[s].mesh.material_ids[f];
                        if (materialID >= 0 && materialID < int(materials.size())) {
                            texturePath = materials[materialID].diffuse_texname;
                            if (!texturePath.empty() && textureData.count(texturePath)) {
                                unsigned char* texData = textureData[texturePath];
                                glm::ivec2 texDim = textureDimensions[texturePath];

                                // Ensure valid texture dimensions
                                if (texDim.x > 0 && texDim.y > 0 && texData) {
                                    int u = static_cast<int>(std::floor(tx * texDim.x)) % texDim.x;
                                    int v = static_cast<int>(std::floor((1.0f - ty) * texDim.y)) % texDim.y;
                                    // Ensure u and v are non-negative
                                    u = (u + texDim.x) % texDim.x;
                                    v = (v + texDim.y) % texDim.y;

                                    texCoordinate = glm::vec2(u, v);

                                    size_t texIndex = (v * texDim.x + u) * 3;
                                }
                            }
                        }
                    }
                    /*
                    // Optional: vertex colors
                    vcolor.x = attrib.colors[3 * size_t(idx.vertex_index) + 0];
                    vcolor.y = attrib.colors[3 * size_t(idx.vertex_index) + 1];
                    vcolor.z = attrib.colors[3 * size_t(idx.vertex_index) + 2];
                    */
                    int materialID = shapes[s].mesh.material_ids[f];
                    if (materialID >= 0 && materialID < int(materials.size())) {
                        tria.ambient = glm::vec3(materials[materialID].ambient[0], 
                            materials[materialID].ambient[1], 
                            materials[materialID].ambient[2]);
   
                        tria.diffuse = glm::vec3(materials[materialID].diffuse[0], 
                            materials[materialID].diffuse[1], 
                            materials[materialID].diffuse[2]);
   
                        tria.specular = glm::vec3(materials[materialID].specular[0], 
                             materials[materialID].specular[1], 
                             materials[materialID].specular[2]);
   
                        tria.shininess = materials[materialID].shininess;
   
                        // std::cout << "Diffuse Material: " << materials[materialID].diffuse[0] << " " << materials[materialID].diffuse[1] << " " << materials[materialID].diffuse[2] << std::endl;

                    }
                    
                    // std::cout << "Red" << vcolor.x << "Green" << vcolor.y << "Blue" << vcolor.z << std::endl;
                    if (v == 0) {
                        tria.pointOne = vertex;
                        tria.normalOne = vnormal;
                        tria.colorOneCoordinate = texCoordinate;
                        // texture Name is identical for the whole Triangle so I initialize it only once
                        tria.textureName = texturePath;

                    }
                    if (v == 1) {
                        tria.pointTwo = vertex;
                        tria.normalTwo = vnormal;
                        tria.colorTwoCoordinate = texCoordinate;
                    }
                    if (v == 2) {
                        tria.pointThree = vertex;
                        tria.normalThree = vnormal;
                        tria.colorThreeCoordinate = texCoordinate;
                    }
                }
            }
            /*
            // Print out the triangle information
            std::cout << "Triangle:\n";
            std::cout << " Point One: (" << tria.pointOne.x << ", " << tria.pointOne.y << ", " << tria.pointOne.z << ")\n";
            std::cout << " Point Two: (" << tria.pointTwo.x << ", " << tria.pointTwo.y << ", " << tria.pointTwo.z << ")\n";
            std::cout << " Point Three: (" << tria.pointThree.x << ", " << tria.pointThree.y << ", " << tria.pointThree.z << ")\n";
            
            std::cout << " Normal One: (" << tria.normalOne.x << ", " << tria.normalOne.y << ", " << tria.normalOne.z << ")\n";
            std::cout << " Normal Two: (" << tria.normalTwo.x << ", " << tria.normalTwo.y << ", " << tria.normalTwo.z << ")\n";
            std::cout << " Normal Three: (" << tria.normalThree.x << ", " << tria.normalThree.y << ", " << tria.normalThree.z << ")\n";
        
            std::cout << " Color Coordinates: (" << tria.colorOneCoordinate.x << ", " << tria.colorOneCoordinate.y << ") | ";
            std::cout << "(" << tria.colorTwoCoordinate.x << ", " << tria.colorTwoCoordinate.y << ") | ";
            std::cout << "(" << tria.colorThreeCoordinate.x << ", " << tria.colorThreeCoordinate.y << ")\n";            
            std::cout << " Ambient: (" << tria.ambient.x << ", " << tria.ambient.y << ", " << tria.ambient.z << ")\n";
            std::cout << " Diffuse: (" << tria.diffuse.x << ", " << tria.diffuse.y << ", " << tria.diffuse.z << ")\n";
            std::cout << " Specular: (" << tria.specular.x << ", " << tria.specular.y << ", " << tria.specular.z << ")\n";
            
            std::cout << " Shininess: " << tria.shininess << "\n";
            std::cout << " Texture Name: " << tria.textureName << "\n";          
            */  
            triangles.push_back(tria);
            index_offset += fv;
        }
    }
    obj.triangleEndIndex = triangles.size();
    objObjects.insert({objName, obj});
    // std::cout << "Triangles: " << obj.triangleStartIndex << " to " << obj.triangleEndIndex << std::endl;
}


// multiplies all triangles from specific obj with a matrix
void ObjectManager::transformTriangles(const std::string& objFilename, const glm::mat4& matrix) {

    // std::vector<Triangle>& triangles = objObjects[objFilename].triangles;
    // std::cout << "Transforming triangles for object: " << objFilename << std::endl;
    // std::cout << "Triangles: " << objObjects.at(objFilename).triangleStartIndex << " to " << objObjects.at(objFilename).triangleEndIndex << std::endl;
    for (int i = objObjects.at(objFilename).triangleStartIndex; i < objObjects.at(objFilename).triangleEndIndex; ++i) {
        triangles.at(i).pointOne = matrix * triangles.at(i).pointOne;
        triangles.at(i).pointTwo = matrix * triangles.at(i).pointTwo;
        triangles.at(i).pointThree = matrix * triangles.at(i).pointThree;
    }
} 
/*
// check first Triangle point for which coordinate is bigger
bool compareXPointsOfTriangle(const Triangle& a, const Triangle& b) {
    return a.pointOne.x < b.pointOne.x;
}
bool compareYPointsOfTriangle(const Triangle& a, const Triangle& b) {
    return a.pointOne.y < b.pointOne.y;
}
bool compareZPointsOfTriangle(const Triangle& a, const Triangle& b) {
    return a.pointOne.z < b.pointOne.z;
}

// Hierarchical Bounding around Objects
// Creates a Box around a triangulated object
std::pair<glm::vec3,glm::vec3> calculateBoundingBoxes(const std::vector<Triangle>& triangles) {

    glm::vec3 minBox = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);;
    glm::vec3 maxBox = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    for (const Triangle& t : triangles) {
        minBox = glm::min(minBox, glm::vec3(t.pointOne));
        minBox = glm::min(minBox, glm::vec3(t.pointTwo));
        minBox = glm::min(minBox, glm::vec3(t.pointThree));

        maxBox = glm::max(maxBox, glm::vec3(t.pointOne));
        maxBox = glm::max(maxBox, glm::vec3(t.pointTwo));
        maxBox = glm::max(maxBox, glm::vec3(t.pointThree));
    }

    return { minBox, maxBox };
}

// Hierarchical Bounding around Objects
// Triangle Object gets split based on the longest side
void ObjectManager::splitTrianglesForBox(Node* root) {
    // Sort triangles: The largest Side of the Box is sorted so I can cut this side of the box in Half
    // (Triangles get put into right or left side of Tree)
    float boxXSize = fabs(root->maxBox.x - root->minBox.x);
    float boxYSize = fabs(root->maxBox.y - root->minBox.y);
    float boxZSize = fabs(root->maxBox.z - root->minBox.z);

    glm::vec3 minBoxLeft;
    glm::vec3 maxBoxLeft;
    glm::vec3 minBoxRight;
    glm::vec3 maxBoxRight;
    std::vector<Triangle> trianglesLeftSide;
    std::vector<Triangle> trianglesRightSide;
    std::size_t triangleSize = root->triangles.size();

    if (boxXSize > boxYSize && boxXSize > boxZSize) {
        std::sort(root->triangles.begin(), root->triangles.end(), compareXPointsOfTriangle);
    }
    else if (boxYSize > boxXSize && boxYSize > boxZSize) {
        std::sort(root->triangles.begin(), root->triangles.end(), compareYPointsOfTriangle);
    }
    else{
        std::sort(root->triangles.begin(), root->triangles.end(), compareZPointsOfTriangle);
    }
    */
    /* for (Triangle& t : root->triangles) {
        std::cout << "tX" << t.pointOne.x << std::endl;
    }*/
/*
    trianglesLeftSide.insert(trianglesLeftSide.end(), root->triangles.begin(), root->triangles.begin() + triangleSize/2);
    trianglesRightSide.insert(trianglesRightSide.end(), root->triangles.begin() + triangleSize / 2, root->triangles.end());
    std::tie(minBoxLeft, maxBoxLeft) = calculateBoundingBoxes(trianglesLeftSide);
    std::tie(minBoxRight, maxBoxRight) = calculateBoundingBoxes(trianglesRightSide);
    root->left = new Node(trianglesLeftSide, minBoxLeft, maxBoxLeft);
    root->right = new Node(trianglesRightSide, minBoxRight, maxBoxRight);
    // Build Binary Tree Recursively until size is <= triangleSizeStop
    int triangleSizeStop = 8;
    if (trianglesLeftSide.size() > triangleSizeStop) {
        splitTrianglesForBox(root->left);
    }
    if (trianglesRightSide.size() > triangleSizeStop) {
        splitTrianglesForBox(root->right);
    }


    // std::cout << "Triangles" << triangleSize << "Left" << trianglesLeftSide.size() << "Right" << trianglesRightSide.size() << std::endl;

}
// Hierarchical Bounding around Objects
// Concept: https://cg.informatik.uni-freiburg.de/course\_notes/graphics\_01\_raycasting.pdf
void  ObjectManager::createBoundingHierarchy(const std::string& objFilename) {

    std::vector<Triangle>& triangles = objTriangles[objFilename];

    // Assign First Min and Max Box to Triangles
    std::tie(minBox[objFilename], maxBox[objFilename]) = calculateBoundingBoxes(triangles);
    Node* root = new Node(triangles, minBox[objFilename], maxBox[objFilename]);
    splitTrianglesForBox(root);
    boundingVolumeHierarchy[objFilename] = root;
}
*/