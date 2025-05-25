#include "Scene.h"

namespace Scene
{
    void scene1(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree,
                glm::vec2 &imageSize, glm::vec4 &lightPos)
    {

        float radius = 100.0f;                      // Radius of the circle on which the camera moves
        float radians = glm::radians(angleDegree);  // Convert angle from degrees to radians
        float circleX = radius * std::cos(radians); // Calculate x coordinate on the circle
        float circleZ = radius * std::sin(radians); // Calculate z coordinate on the circle

        viewMatrix = Transformation::createViewMatrix(glm::vec3(circleX, 0.f, circleZ),
                                                      glm::vec3(glm::radians(0.f), glm::radians(angleDegree + 90), glm::radians(0.f)));

        imageSize = glm::vec2(600, 400);                     // Image Size
        lightPos = glm::vec4(500.0f, -300.0f, -200.f, 1.0f); // Light Position

        // Load Cube Triangles and scale them
        objManager.loadObjFile("cube", "./obj/cube.obj");
        objManager.transformTriangles("cube", Transformation::scaleObj(10.0f, 10.0f, 10.0f));
        objManager.transformTriangles("cube", Transformation::changeObjPosition(glm::vec3(0.f, 15.f, -15.f)));

        // Uncomment and use these lines if needed
        objManager.loadObjFile("cube2", "./obj/cube.obj");
        objManager.transformTriangles("cube2", Transformation::scaleObj(10.0f, 10.0f, 10.0f));
        objManager.transformTriangles("cube2", Transformation::changeObjPosition(glm::vec3(0.f, 15.f, 15.f)));
        // objManager.loadObjFile("sphere", "./obj/sphere.obj");
        // objManager.loadObjFile("cat2", "./obj/cat/cat.obj");

        // objManager.objObjects.insert({"cat3", objManager.objObjects["cat2"]});
        // objManager.loadObjFile("cat5", "./obj/cat/cat.obj");
    }
    void sceneChair(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree,
                    glm::vec2 &imageSize, glm::vec4 &lightPos)
    {

        float radius = 100.0f;                      // Radius of the circle on which the camera moves
        float radians = glm::radians(angleDegree);  // Convert angle from degrees to radians
        float circleX = radius * std::cos(radians); // Calculate x coordinate on the circle
        float circleZ = radius * std::sin(radians); // Calculate z coordinate on the circle

        viewMatrix = Transformation::createViewMatrix(glm::vec3(circleX, 0.f, circleZ),
                                                      glm::vec3(glm::radians(0.f), glm::radians(angleDegree + 90), glm::radians(0.f)));
        imageSize = glm::vec2(600, 400);                     // Image Size
        lightPos = glm::vec4(500.0f, -300.0f, -200.f, 1.0f); // Light Position
        objManager.loadObjFile("chair", "./obj/chair/chair.obj");
        objManager.transformTriangles("chair", Transformation::rotateObjX(glm::radians(-90.f)));
        objManager.transformTriangles("chair", Transformation::rotateObjY(glm::radians(180.f)));
        objManager.transformTriangles("chair", Transformation::scaleObj(100.f, 100.f, 100.f));

    }
    void scene2(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree, glm::vec2 &imageSize, glm::vec4 &lightPos)
    {

        // Initiate Scene with Camera and View Matrix
        float radius = 50.0f;                       // Radius of the circle on which the camera moves
        float radians = glm::radians(angleDegree);  // Convert angle from degrees to radians
        float circleX = radius * std::cos(radians); // Calculate x coordinate on the circle
        float circleZ = radius * std::sin(radians); // Calculate z coordinate on the circle
        // Modelview Transform
        viewMatrix = Transformation::createViewMatrix(glm::vec3(circleX, -50.f, circleZ), glm::vec3(glm::radians(30.f), glm::radians(angleDegree + 90), glm::radians(0.f)));
        imageSize = glm::vec2(600, 400);                     // Image Size
        lightPos = glm::vec4(500.0f, -300.0f, -200.f, 1.0f); // Light Position

        // 3 Trees
        objManager.loadObjFile("tree", "./obj/tree/tree.obj");
        // Copy Tree onto Tree1 and 2
        // Place Tree 0
        objManager.transformTriangles("tree", Transformation::scaleObj(0.03f, 0.03f, 0.03f));
        objManager.transformTriangles("tree", Transformation::rotateObjX(glm::radians(-90.f)));
        objManager.transformTriangles("tree", Transformation::changeObjPosition(glm::vec3(-6.f, -25.f, -25.f)));
        // Place Tree 1
    }

    void sceneComplex(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree, glm::vec2 &imageSize, glm::vec4 &lightPos)
    {

        // Initiate Scene with Camera and View Matrix
        float radius = 50.0f;                       // Radius of the circle on which the camera moves
        float radians = glm::radians(angleDegree);  // Convert angle from degrees to radians
        float circleX = radius * std::cos(radians); // Calculate x coordinate on the circle
        float circleZ = radius * std::sin(radians); // Calculate z coordinate on the circle
        // Modelview Transform
        viewMatrix = Transformation::createViewMatrix(glm::vec3(circleX, -50.f, circleZ), glm::vec3(glm::radians(30.f), glm::radians(angleDegree + 90), glm::radians(0.f)));
        imageSize = glm::vec2(600, 400);                     // Image Size
        lightPos = glm::vec4(500.0f, -300.0f, -200.f, 1.0f); // Light Position

		// 1 Cube Ground
		objManager.loadObjFile("cube","./obj/cube.obj");
		objManager.transformTriangles("cube", Transformation::scaleObj(35.0f, 35.0f, 35.0f));
		objManager.transformTriangles("cube", Transformation::changeObjPosition(glm::vec3(0.f, 10.f, 0.f)));

        // 2 Cats
		objManager.loadObjFile("cat","./obj/cat/cat.obj");
		objManager.transformTriangles("cat", Transformation::scaleObj(0.35f, 0.35f, 0.35f));
		objManager.transformTriangles("cat", Transformation::rotateObjX(glm::radians(-90.f)));
		objManager.transformTriangles("cat", Transformation::rotateObjY(glm::radians(125.f)));
		objManager.transformTriangles("cat", Transformation::changeObjPosition(glm::vec3(25.f, -25.f, -14.f)));

        objManager.loadObjFile("cat1","./obj/cat/cat.obj");
        objManager.transformTriangles("cat1", Transformation::scaleObj(0.35f, 0.35f, 0.35f));
		objManager.transformTriangles("cat1", Transformation::rotateObjX(glm::radians(-90.f)));
		objManager.transformTriangles("cat1", Transformation::rotateObjY(glm::radians(70.f)));
		objManager.transformTriangles("cat1", Transformation::changeObjPosition(glm::vec3(25.f, -25.f, 8.f)));

        // 1 Stanford Bunny
		objManager.loadObjFile("bunny","./obj/stanford-bunny.obj");
		objManager.transformTriangles("bunny", Transformation::scaleObj(50.f, 50.0f, 50.0f));
		objManager.transformTriangles("bunny", Transformation::rotateObjX(glm::radians(181.f)));
		objManager.transformTriangles("bunny", Transformation::rotateObjY(glm::radians(90.f)));
		objManager.transformTriangles("bunny", Transformation::changeObjPosition(glm::vec3(25.f, -23.f, 0.f)));


        // 3 Trees
		objManager.loadObjFile("tree","./obj/tree/tree.obj");
		objManager.transformTriangles("tree", Transformation::scaleObj(0.03f, 0.03f, 0.03f));
		objManager.transformTriangles("tree", Transformation::rotateObjX(glm::radians(-90.f)));
		objManager.transformTriangles("tree", Transformation::changeObjPosition(glm::vec3(-6.f, -25.f, -25.f)));

        objManager.loadObjFile("tree1","./obj/tree/tree.obj");
		objManager.transformTriangles("tree1", Transformation::scaleObj(0.035f, 0.035f, 0.035f));
		objManager.transformTriangles("tree1", Transformation::rotateObjX(glm::radians(-90.f)));
		objManager.transformTriangles("tree1", Transformation::changeObjPosition(glm::vec3(-6.f, -25.f, 0.f)));

        objManager.loadObjFile("tree2","./obj/tree/tree.obj");
		objManager.transformTriangles("tree2", Transformation::scaleObj(0.03f, 0.03f, 0.03f));
		objManager.transformTriangles("tree2", Transformation::rotateObjX(glm::radians(-90.f)));
		objManager.transformTriangles("tree2", Transformation::changeObjPosition(glm::vec3(-6.f, -25.f, 25.f)));

    } 
}
