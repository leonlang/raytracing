#include "Datastructure.h"
#include <glm/gtx/component_wise.hpp>

std::pair<glm::vec3, glm::vec3> Datastructure::createBoundingBox(const std::vector<Triangle>& triangles) {
    glm::vec3 minBox(FLT_MAX, FLT_MAX, FLT_MAX);
    glm::vec3 maxBox(-FLT_MAX, -FLT_MAX, -FLT_MAX);

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
