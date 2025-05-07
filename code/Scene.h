#ifndef SCENE_H
#define SCENE_H

#include "Transformation.h"
#include "Object.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Scene {
    void scene1(ObjectManager& objManager, glm::mat4& viewMatrix, const float& angleDegree, 
                glm::vec2& imageSize, glm::vec4& lightPos);
    void scene2(ObjectManager& objManager, glm::mat4& viewMatrix, const float& angleDegree, 
                glm::vec2& imageSize, glm::vec4& lightPos);
}

#endif // SCENE1_H
