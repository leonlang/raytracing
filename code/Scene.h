#ifndef SCENE_H
#define SCENE_H

#include "Transformation.h"
#include "Object.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Scene
{
    void hairball(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree,
                  glm::vec2 &imageSize, glm::vec4 &lightPos, glm::vec3 &backgroundColor);
    void forest(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree,
                glm::vec2 &imageSize, glm::vec4 &lightPos, glm::vec3 &backgroundColor);
    void sphere(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree,
                glm::vec2 &imageSize, glm::vec4 &lightPos, glm::vec3 &backgroundColor);
    void dragon(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree,
                glm::vec2 &imageSize, glm::vec4 &lightPos, glm::vec3 &backgroundColor);
    void ambientOcclusionSphere(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree,
                                glm::vec2 &imageSize, glm::vec4 &lightPos, glm::vec3 &backgroundColor);
    void bistroInterior(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree,
                        glm::vec2 &imageSize, glm::vec4 &lightPos, glm::vec3 &backgroundColor);
    void bistroInterior1(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree,
                         glm::vec2 &imageSize, glm::vec4 &lightPos, glm::vec3 &backgroundColor);
    void bistroExterior(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree,
                        glm::vec2 &imageSize, glm::vec4 &lightPos, glm::vec3 &backgroundColor);
}

#endif
