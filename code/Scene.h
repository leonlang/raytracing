#ifndef SCENE_H
#define SCENE_H

#include "Transformation.h"
#include "Object.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Scene
{
    void scene1(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree,
                glm::vec2 &imageSize, glm::vec4 &lightPos, glm::vec3 &backgroundColor);
    void scene2(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree,
                glm::vec2 &imageSize, glm::vec4 &lightPos, glm::vec3 &backgroundColor);
    void sceneChair(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree,
                    glm::vec2 &imageSize, glm::vec4 &lightPos, glm::vec3 &backgroundColor);
    void sceneComplex(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree,
               glm::vec2 &imageSize, glm::vec4 &lightPos, glm::vec3 &backgroundColor);
    void sceneBMW(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree,
                glm::vec2 &imageSize, glm::vec4 &lightPos, glm::vec3 &backgroundColor);
    void hairball(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree,
        glm::vec2 &imageSize, glm::vec4 &lightPos, glm::vec3 &backgroundColor);
    void house(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree,
        glm::vec2 &imageSize, glm::vec4 &lightPos, glm::vec3 &backgroundColor);
    void forest(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree,
        glm::vec2 &imageSize, glm::vec4 &lightPos, glm::vec3 &backgroundColor);
    void sphere(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree,
        glm::vec2 &imageSize, glm::vec4 &lightPos, glm::vec3 &backgroundColor);
    void dragon(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree,
        glm::vec2 &imageSize, glm::vec4 &lightPos, glm::vec3 &backgroundColor);
    void ambientOcclusionSphere(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree,
        glm::vec2 &imageSize, glm::vec4 &lightPos, glm::vec3 &backgroundColor);
}

#endif // SCENE1_H
