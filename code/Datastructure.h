#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include <vector>
#include <glm/glm.hpp>
#include "Object.h"



class Datastructure {
public:
    std::vector<int> triangleNumbers;
    
    std::pair<glm::vec3, glm::vec3> createBoundingBox(const std::vector<Triangle>& triangles);
};

#endif // DATASTRUCTURE_H
