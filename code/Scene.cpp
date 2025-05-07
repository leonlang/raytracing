#include "Scene.h"

namespace Scene {
    void scene1(ObjectManager& objManager, glm::mat4& viewMatrix, const float& angleDegree, 
                glm::vec2& imageSize, glm::vec4& lightPos) {
        
        float radius = 100.0f; // Radius of the circle on which the camera moves
        float radians = glm::radians(angleDegree); // Convert angle from degrees to radians
        float circleX = radius * std::cos(radians); // Calculate x coordinate on the circle
        float circleZ = radius * std::sin(radians); // Calculate z coordinate on the circle

        viewMatrix = Transformation::createViewMatrix(glm::vec3(circleX, 0.f, circleZ), 
                    glm::vec3(glm::radians(0.f), glm::radians(angleDegree + 90), glm::radians(0.f)));

        imageSize = glm::vec2(600, 400); // Image Size
        lightPos = glm::vec4(500.0f, -300.0f, -200.f, 1.0f); // Light Position

        // Load Cube Triangles and scale them
        objManager.loadObjFile("cube", "./obj/cube.obj");
        objManager.transformTriangles("cube", Transformation::scaleObj(10.0f, 10.0f, 10.0f));
        objManager.transformTriangles("cube", Transformation::changeObjPosition(glm::vec3(0.f, 15.f, -15.f)));

        // Uncomment and use these lines if needed
        // objManager.loadObjFile("cube2", "./obj/cube2.obj");
        // objManager.transformTriangles("cube2", Transformation::scaleObj(10.0f, 10.0f, 10.0f));
        // objManager.transformTriangles("cube2", Transformation::changeObjPosition(glm::vec3(0.f, 15.f, 15.f)));

        // objManager.loadObjFile("sphere", "./obj/sphere.obj");

        // objManager.loadObjFile("cat2", "./obj/chair/chair.obj");
        // objManager.transformTriangles("cat2", Transformation::scaleObj(20.f, 20.f, 20.f));
        // objManager.loadObjFile("cat2", "./obj/cat/cat.obj");

        // objManager.objObjects.insert({"cat3", objManager.objObjects["cat2"]}); 
        // objManager.loadObjFile("cat5", "./obj/cat/cat.obj");
    }
    void scene2(ObjectManager& objManager, glm::mat4& viewMatrix, const float& angleDegree, glm::vec2& imageSize, glm::vec4& lightPos){

        // Initiate Scene with Camera and View Matrix
        float radius = 50.0f; // Radius of the circle on which the camera moves
        float radians = glm::radians(angleDegree); // Convert angle from degrees to radians
        float circleX = radius * std::cos(radians); // Calculate x coordinate on the circle
        float circleZ = radius * std::sin(radians); // Calculate z coordinate on the circle
        // Modelview Transform
        viewMatrix = Transformation::createViewMatrix(glm::vec3(circleX, -50.f, circleZ), glm::vec3(glm::radians(30.f), glm::radians(angleDegree + 90), glm::radians(0.f)));
        imageSize = glm::vec2(600, 400); // Image Size
        lightPos = glm::vec4(500.0f, -300.0f, -200.f, 1.0f); // Light Position
    
        // 3 Trees
        objManager.loadObjFile("tree","./obj/tree/tree.obj");
        // Copy Tree onto Tree1 and 2
        // Place Tree 0
        objManager.transformTriangles("tree", Transformation::scaleObj(0.03f, 0.03f, 0.03f));
        objManager.transformTriangles("tree", Transformation::rotateObjX(glm::radians(-90.f)));
        objManager.transformTriangles("tree", Transformation::changeObjPosition(glm::vec3(-6.f, -25.f, -25.f)));
        // objManager.transformTriangles("tree", glm::inverse(viewMatrix));
        // Place Tree 1
    }
}
