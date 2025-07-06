#include "Scene.h"

namespace Scene
{
    void scene1(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree,
                glm::vec2 &imageSize, glm::vec4 &lightPos, glm::vec3 &backgroundColor)
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
                    glm::vec2 &imageSize, glm::vec4 &lightPos, glm::vec3 &backgroundColor)
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
        objManager.transformTriangles("chair", Transformation::changeObjPosition(glm::vec3(-10.f, 50.f, -100.f)));
    }
    void scene2(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree, glm::vec2 &imageSize, glm::vec4 &lightPos, glm::vec3 &backgroundColor)
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

        objManager.loadObjFile("tree", "./obj/tree/tree.obj");
        objManager.transformTriangles("tree", Transformation::scaleObj(0.03f, 0.03f, 0.03f));
        objManager.transformTriangles("tree", Transformation::rotateObjX(glm::radians(-90.f)));
        objManager.transformTriangles("tree", Transformation::changeObjPosition(glm::vec3(-6.f, -25.f, -25.f)));
    }

    void sceneComplex(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree, glm::vec2 &imageSize, glm::vec4 &lightPos, glm::vec3 &backgroundColor)
    {
        // To use this scene go to main.cpp and and change Ray to this: Ray ray(glm::vec3(0.0f, 0.0f, 400.0f));
        // Initiate Scene with Camera and View Matrix
        float radius = 50.0f;                       // Radius of the circle on which the camera moves
        float radians = glm::radians(angleDegree);  // Convert angle from degrees to radians
        float circleX = radius * std::cos(radians); // Calculate x coordinate on the circle
        float circleZ = radius * std::sin(radians); // Calculate z coordinate on the circle
        backgroundColor = glm::vec3(173, 216, 230);
        // Modelview Transform
        viewMatrix = Transformation::createViewMatrix(glm::vec3(circleX, -50.f, circleZ), glm::vec3(glm::radians(30.f), glm::radians(angleDegree + 90), glm::radians(0.f)));
        // imageSize = glm::vec2(600, 400);                     // Image Size
        imageSize = glm::vec2(600, 400);                     // Image Size
        lightPos = glm::vec4(500.0f, -300.0f, -200.f, 1.0f); // Light Position

        // 1 Cube Ground
        objManager.loadObjFile("cube", "./obj/cube.obj");
        objManager.transformTriangles("cube", Transformation::scaleObj(35.0f, 35.0f, 35.0f));
        objManager.transformTriangles("cube", Transformation::changeObjPosition(glm::vec3(0.f, 10.f, 0.f)));

        // 2 Cats
        objManager.loadObjFile("cat", "./obj/cat/cat.obj");
        objManager.transformTriangles("cat", Transformation::scaleObj(0.35f, 0.35f, 0.35f));
        objManager.transformTriangles("cat", Transformation::rotateObjX(glm::radians(-90.f)));
        objManager.transformTriangles("cat", Transformation::rotateObjY(glm::radians(125.f)));
        objManager.transformTriangles("cat", Transformation::changeObjPosition(glm::vec3(25.f, -25.f, -14.f)));

        objManager.loadObjFile("cat1", "./obj/cat/cat.obj");
        objManager.transformTriangles("cat1", Transformation::scaleObj(0.35f, 0.35f, 0.35f));
        objManager.transformTriangles("cat1", Transformation::rotateObjX(glm::radians(-90.f)));
        objManager.transformTriangles("cat1", Transformation::rotateObjY(glm::radians(70.f)));
        objManager.transformTriangles("cat1", Transformation::changeObjPosition(glm::vec3(25.f, -25.f, 8.f)));

        // 1 Stanford Bunny
        objManager.loadObjFile("bunny", "./obj/stanford-bunny.obj");
        objManager.transformTriangles("bunny", Transformation::scaleObj(50.f, 50.0f, 50.0f));
        objManager.transformTriangles("bunny", Transformation::rotateObjX(glm::radians(181.f)));
        objManager.transformTriangles("bunny", Transformation::rotateObjY(glm::radians(90.f)));
        objManager.transformTriangles("bunny", Transformation::changeObjPosition(glm::vec3(25.f, -23.f, 0.f)));

        // 3 Trees
        objManager.loadObjFile("tree", "./obj/tree/tree.obj");
        objManager.transformTriangles("tree", Transformation::scaleObj(0.03f, 0.03f, 0.03f));
        objManager.transformTriangles("tree", Transformation::rotateObjX(glm::radians(-90.f)));
        objManager.transformTriangles("tree", Transformation::changeObjPosition(glm::vec3(-6.f, -25.f, -25.f)));

        objManager.loadObjFile("tree1", "./obj/tree/tree.obj");
        objManager.transformTriangles("tree1", Transformation::scaleObj(0.035f, 0.035f, 0.035f));
        objManager.transformTriangles("tree1", Transformation::rotateObjX(glm::radians(-90.f)));
        objManager.transformTriangles("tree1", Transformation::changeObjPosition(glm::vec3(-6.f, -25.f, 0.f)));

        objManager.loadObjFile("tree2", "./obj/tree/tree.obj");
        objManager.transformTriangles("tree2", Transformation::scaleObj(0.03f, 0.03f, 0.03f));
        objManager.transformTriangles("tree2", Transformation::rotateObjX(glm::radians(-90.f)));
        objManager.transformTriangles("tree2", Transformation::changeObjPosition(glm::vec3(-6.f, -25.f, 25.f)));
    }

    void sceneBMW(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree,
                  glm::vec2 &imageSize, glm::vec4 &lightPos, glm::vec3 &backgroundColor)
    {

        float radius = 6250.f;                         // Radius of the circle on which the camera moves
        float radians = glm::radians(angleDegree);     // Convert angle from degrees to radians
        float circleX = radius * std::sin(radians);    // Calculate x coordinate on the circle
        float circleZ = radius * std::cos(radians);    // Calculate z coordinate on the circle
        backgroundColor = glm::vec3(20.f, 20.f, 20.f); // Background Color

        // float lookDownAngle = 20.f; // Angle to look down
        // float lookDownY = -200.0f; // Y position to look down from
        // viewMatrix = Transformation::createViewMatrix(glm::vec3(circleX, lookDownY, -circleZ), glm::vec3(glm::radians(lookDownAngle), glm::radians(angleDegree), glm::radians(0.f)));
        viewMatrix = Transformation::createViewMatrix(glm::vec3(circleX, 0.0f, -circleZ), glm::vec3(glm::radians(0.0f), glm::radians(angleDegree), glm::radians(0.f)));

        // viewMatrix = Transformation::createViewMatrix(glm::vec3(0.f, 0.f, 0.f),
        //                                               glm::vec3(glm::radians(0.f), glm::radians(0.f), glm::radians(0.f)));
        imageSize = glm::vec2(1920, 1080);                   // Image Size
        lightPos = glm::vec4(500.0f, -300.0f, -200.f, 1.0f); // Light Position
        objManager.loadObjFile("bmw", "./obj/bmw/bmw.obj");
        objManager.transformTriangles("bmw", Transformation::rotateObjX(glm::radians(180.f)));
        objManager.transformTriangles("bmw", Transformation::rotateObjY(glm::radians(0.f)));
        // objManager.transformTriangles("bmw", Transformation::scaleObj(100.f, 100.f, 100.f));
        objManager.transformTriangles("bmw", Transformation::changeObjPosition(glm::vec3(0.f, 50.f, 0.f)));
    }

    void hairball(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree,
                  glm::vec2 &imageSize, glm::vec4 &lightPos, glm::vec3 &backgroundColor)
    {

        float radius = 100000.0f;                         // Radius of the circle on which the camera moves
        float radians = glm::radians(angleDegree);        // Convert angle from degrees to radians
        float circleX = radius * std::sin(radians);       // Calculate x coordinate on the circle
        float circleZ = radius * std::cos(radians);       // Calculate z coordinate on the circle
        backgroundColor = glm::vec3(200.f, 200.f, 200.f); // Background Color
        viewMatrix = Transformation::createViewMatrix(glm::vec3(circleX, 0.0f, -circleZ), glm::vec3(glm::radians(0.0f), glm::radians(angleDegree), glm::radians(0.f)));

        imageSize = glm::vec2(1920, 1080);                   // Image Size
        lightPos = glm::vec4(500.0f, -300.0f, -200.f, 1.0f); // Light Position
        objManager.loadObjFile("hairball", "./obj/hairball/hairball.obj");
        // objManager.transformTriangles("hairball", Transformation::rotateObjX(glm::radians(180.f)));
        // objManager.transformTriangles("hairball", Transformation::rotateObjY(glm::radians(0.f)));
        objManager.transformTriangles("hairball", Transformation::scaleObj(90.f, 90.f, 90.f));
        // objManager.transformTriangles("hairball", Transformation::changeObjPosition(glm::vec3(0.f, 50.f, 250.f)));
    }

    void house(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree,
               glm::vec2 &imageSize, glm::vec4 &lightPos, glm::vec3 &backgroundColor)
    {

        float radius = 500.0f;                            // Radius of the circle on which the camera moves
        float radians = glm::radians(angleDegree);        // Convert angle from degrees to radians
        float circleX = radius * std::sin(radians);       // Calculate x coordinate on the circle
        float circleZ = radius * std::cos(radians);       // Calculate z coordinate on the circle
        backgroundColor = glm::vec3(200.f, 200.f, 200.f); // Background Color
        // viewMatrix = Transformation::createViewMatrix(glm::vec3(circleX, 50.0f, -circleZ), glm::vec3(glm::radians(0.0f), glm::radians(angleDegree), glm::radians(0.f)));
        float lookDownAngle = 25.f; // Angle to look down
        float lookDownY = -200.0f;  // Y position to look down from

        viewMatrix = Transformation::createViewMatrix(glm::vec3(circleX, lookDownY, -circleZ), glm::vec3(glm::radians(lookDownAngle), glm::radians(angleDegree), glm::radians(0.f)));

        imageSize = glm::vec2(1920, 1080);                   // Image Size
        lightPos = glm::vec4(500.0f, -300.0f, -200.f, 1.0f); // Light Position
        objManager.loadObjFile("house", "./obj/house/house.obj");
        objManager.transformTriangles("house", Transformation::rotateObjX(glm::radians(180.f)));
        // objManager.transformTriangles("hairball", Transformation::rotateObjY(glm::radians(0.f)));
        objManager.transformTriangles("house", Transformation::scaleObj(0.6f, 0.6f, 0.6f));
        objManager.transformTriangles("house", Transformation::changeObjPosition(glm::vec3(0.f, -90.f, -400.f)));
    }

    void forest(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree,
                glm::vec2 &imageSize, glm::vec4 &lightPos, glm::vec3 &backgroundColor)
    {
        float radius = 100000.f;                          // Radius of the circle on which the camera moves
        float radians = glm::radians(angleDegree);        // Convert angle from degrees to radians
        float circleX = radius * std::sin(radians);       // Calculate x coordinate on the circle
        float circleZ = radius * std::cos(radians);       // Calculate z coordinate on the circle
        backgroundColor = glm::vec3(135.f, 206.f, 235.f); // Background Color

        // for each degree low down increase lookDownY by 2000
        float lookDownAngle = 25.f; // Angle to look down
        float lookDownY = -47000.f; // Y position to look down from
        viewMatrix = Transformation::createViewMatrix(glm::vec3(circleX, lookDownY, -circleZ), glm::vec3(glm::radians(lookDownAngle), glm::radians(angleDegree), glm::radians(0.f)));
        // viewMatrix = Transformation::createViewMatrix(glm::vec3(circleX, 0.0f, -circleZ), glm::vec3(glm::radians(0.0f), glm::radians(angleDegree), glm::radians(0.f)));

        imageSize = glm::vec2(1920, 1080);                         // Image Size
        lightPos = glm::vec4(-5000.0f, -15000.0f, -20000.f, 1.0f); // Light Position
        // create 10 more trees in a loop for a forest-like scene

        objManager.loadObjFile("grass", "./obj/grass_rec/grass_rec.obj");
        objManager.transformTriangles("grass", Transformation::rotateObjX(glm::radians(-90.f)));
        objManager.transformTriangles("grass", Transformation::scaleObj(10.f, 6.f, 7.f));
        // objManager.transformTriangles("grass", Transformation::changeObjPosition(glm::vec3(0.f, -90.f, -400.f)));

        // Back  Trees
        objManager.loadObjFile("tree1", "./obj/chestnut/chestnut.obj");
        objManager.transformTriangles("tree1", Transformation::scaleObj(1.7f, 1.7f, 1.7f));
        objManager.transformTriangles("tree1", Transformation::rotateObjX(glm::radians(-90.f)));
        objManager.transformTriangles("tree1", Transformation::rotateObjY(glm::radians(-270.f)));
        objManager.transformTriangles("tree1", Transformation::changeObjPosition(glm::vec3(800, -80, 900)));

        objManager.loadObjFile("tree2", "./obj/chestnut/chestnut.obj");
        objManager.transformTriangles("tree2", Transformation::scaleObj(1.7f, 1.7f, 1.7f));
        objManager.transformTriangles("tree2", Transformation::rotateObjX(glm::radians(-90.f)));
        objManager.transformTriangles("tree2", Transformation::rotateObjY(glm::radians(-270.f)));
        objManager.transformTriangles("tree2", Transformation::changeObjPosition(glm::vec3(300, -80, 900)));

        objManager.loadObjFile("tree3", "./obj/chestnut/chestnut.obj");
        objManager.transformTriangles("tree3", Transformation::scaleObj(1.7f, 1.7f, 1.7f));
        objManager.transformTriangles("tree3", Transformation::rotateObjX(glm::radians(-90.f)));
        objManager.transformTriangles("tree3", Transformation::rotateObjY(glm::radians(-270.f)));
        objManager.transformTriangles("tree3", Transformation::changeObjPosition(glm::vec3(-300, -80, 900)));

        objManager.loadObjFile("tree4", "./obj/chestnut/chestnut.obj");
        objManager.transformTriangles("tree4", Transformation::scaleObj(1.7f, 1.7f, 1.7f));
        objManager.transformTriangles("tree4", Transformation::rotateObjX(glm::radians(-90.f)));
        objManager.transformTriangles("tree4", Transformation::rotateObjY(glm::radians(-270.f)));
        objManager.transformTriangles("tree4", Transformation::changeObjPosition(glm::vec3(-800, -80, 900)));

        // Middle Row Trees
        objManager.loadObjFile("tree6", "./obj/chestnut/chestnut.obj");
        objManager.transformTriangles("tree6", Transformation::scaleObj(1.f, 1.f, 1.f));
        objManager.transformTriangles("tree6", Transformation::rotateObjX(glm::radians(-90.f)));
        objManager.transformTriangles("tree6", Transformation::rotateObjY(glm::radians(-270.f)));
        objManager.transformTriangles("tree6", Transformation::changeObjPosition(glm::vec3(400, -80, 100)));

        objManager.loadObjFile("tree7", "./obj/chestnut/chestnut.obj");
        objManager.transformTriangles("tree7", Transformation::scaleObj(1.f, 1.f, 1.f));
        objManager.transformTriangles("tree7", Transformation::rotateObjX(glm::radians(-90.f)));
        objManager.transformTriangles("tree7", Transformation::rotateObjY(glm::radians(-270.f)));
        objManager.transformTriangles("tree7", Transformation::changeObjPosition(glm::vec3(-400, -80, 100)));

        // Front Trees
        objManager.loadObjFile("tree5", "./obj/chestnut/chestnut.obj");
        objManager.transformTriangles("tree5", Transformation::scaleObj(1.5f, 1.5f, 1.5f));
        objManager.transformTriangles("tree5", Transformation::rotateObjX(glm::radians(-90.f)));
        objManager.transformTriangles("tree5", Transformation::rotateObjY(glm::radians(-270.f)));
        objManager.transformTriangles("tree5", Transformation::changeObjPosition(glm::vec3(810, -80, -420)));

        objManager.loadObjFile("tree8", "./obj/chestnut/chestnut.obj");
        objManager.transformTriangles("tree8", Transformation::scaleObj(1.5f, 1.5f, 1.5f));
        objManager.transformTriangles("tree8", Transformation::rotateObjX(glm::radians(-90.f)));
        objManager.transformTriangles("tree8", Transformation::rotateObjY(glm::radians(-270.f)));
        objManager.transformTriangles("tree8", Transformation::changeObjPosition(glm::vec3(-800, -80, -420)));

        // Tree in Center
        objManager.loadObjFile("tree9", "./obj/chestnut/chestnut.obj");
        objManager.transformTriangles("tree9", Transformation::scaleObj(1.9f, 1.9f, 1.9f));
        objManager.transformTriangles("tree9", Transformation::rotateObjX(glm::radians(-90.f)));
        objManager.transformTriangles("tree9", Transformation::rotateObjY(glm::radians(-270.f)));
        objManager.transformTriangles("tree9", Transformation::changeObjPosition(glm::vec3(0, -80, -250)));

        // objManager.transformTriangles(treeName, Transformation::changeObjPosition(glm::vec3(-6.f + i * 2.f, -25.f, -25.f + i * 2.f)));
        /* objManager.transformTriangles("hairball", Transformation::rotateObjX(glm::radians(180.f)));
        objManager.transformTriangles("hairball", Transformation::rotateObjY(glm::radians(0.f)));
        // objManager.transformTriangles("hairball", Transformation::scaleObj(100.f, 100.f, 100.f));
        objManager.transformTriangles("hairball", Transformation::changeObjPosition(glm::vec3(0.f, 50.f, 250.f)));  */
    }


        void sphere(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree,
                glm::vec2 &imageSize, glm::vec4 &lightPos, glm::vec3 &backgroundColor)
    {
        float radius = 100000.f;                          // Radius of the circle on which the camera moves
        float radians = glm::radians(angleDegree);        // Convert angle from degrees to radians
        float circleX = radius * std::sin(radians);       // Calculate x coordinate on the circle
        float circleZ = radius * std::cos(radians);       // Calculate z coordinate on the circle
        backgroundColor = glm::vec3(135.f, 206.f, 235.f); // Background Color

        // for each degree low down increase lookDownY by 2000
        float lookDownAngle = 25.f; // Angle to look down
        float lookDownY = -47000.f; // Y position to look down from
        viewMatrix = Transformation::createViewMatrix(glm::vec3(circleX, lookDownY, -circleZ), glm::vec3(glm::radians(lookDownAngle), glm::radians(angleDegree), glm::radians(0.f)));
        // viewMatrix = Transformation::createViewMatrix(glm::vec3(circleX, 0.0f, -circleZ), glm::vec3(glm::radians(0.0f), glm::radians(angleDegree), glm::radians(0.f)));

        imageSize = glm::vec2(1920, 1080);                         // Image Size
        lightPos = glm::vec4(-5000.0f, -15000.0f, -20000.f, 1.0f); // Light Position
        // create 10 more trees in a loop for a forest-like scene

        objManager.loadObjFile("cube", "./obj/cube.obj");
        // objManager.transformTriangles("grass", Transformation::rotateObjX(glm::radians(-90.f)));
        objManager.transformTriangles("cube", Transformation::scaleObj(1500.f, 50.f, 1000.f));
        // objManager.transformTriangles("grass", Transformation::changeObjPosition(glm::vec3(0.f, -90.f, -400.f)));


        /* objManager.loadObjFile("sphere", "./obj/sphere.obj");
        objManager.transformTriangles("sphere", Transformation::scaleObj(50.f, 50.f,50.f));
        objManager.transformTriangles("sphere", Transformation::changeObjPosition(glm::vec3(0, -170, -250))); */
        // half the triangles so only the first half of the sphere is rendered
        
        objManager.loadObjFile("sphere", "./obj/polo_ball/polo_ball.obj");
        objManager.transformTriangles("sphere", Transformation::scaleObj(25.f, 25.f,25.f));
        objManager.transformTriangles("sphere", Transformation::changeObjPosition(glm::vec3(0, -170, -250)));

    }

    void dragon(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree,
                glm::vec2 &imageSize, glm::vec4 &lightPos, glm::vec3 &backgroundColor)
    {

        float radius = 100000.0f;                         // Radius of the circle on which the camera moves
        float radians = glm::radians(angleDegree);        // Convert angle from degrees to radians
        float circleX = radius * std::sin(radians);       // Calculate x coordinate on the circle
        float circleZ = radius * std::cos(radians);       // Calculate z coordinate on the circle
        backgroundColor = glm::vec3(200.f, 200.f, 200.f); // Background Color
        viewMatrix = Transformation::createViewMatrix(glm::vec3(circleX, 0.0f, -circleZ), glm::vec3(glm::radians(0.0f), glm::radians(angleDegree), glm::radians(0.f)));

        imageSize = glm::vec2(1920, 1080);                         // Image Size
        lightPos = glm::vec4(-5000.0f, -15000.0f, -20000.f, 1.0f); // Light Position
        objManager.loadObjFile("dragon", "./obj/dragon/dragon.obj");
        objManager.transformTriangles("dragon", Transformation::scaleObj(1000.f, 1000.f, 1000.f));
        objManager.transformTriangles("dragon", Transformation::rotateObjX(glm::radians(-180.f)));
        objManager.transformTriangles("dragon", Transformation::rotateObjY(glm::radians(90.f)));


        // objManager.transformTriangles("hairball", Transformation::rotateObjX(glm::radians(180.f)));
        // objManager.transformTriangles("hairball", Transformation::rotateObjY(glm::radians(0.f)));
        // objManager.transformTriangles("hairball", Transformation::scaleObj(90.f, 90.f, 90.f));
        // objManager.transformTriangles("hairball", Transformation::changeObjPosition(glm::vec3(0.f, 50.f, 250.f)));
    }

    void ambientOcclusionSphere(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree,
                glm::vec2 &imageSize, glm::vec4 &lightPos, glm::vec3 &backgroundColor)
    {
        // To Implement for better Performance: go through all triangles and remove all which have a negative y value        



        float radius = 100000.0f;                         // Radius of the circle on which the camera moves
        float radians = glm::radians(angleDegree);        // Convert angle from degrees to radians
        float circleX = radius * std::sin(radians);       // Calculate x coordinate on the circle
        float circleZ = radius * std::cos(radians);       // Calculate z coordinate on the circle
        backgroundColor = glm::vec3(200.f, 200.f, 200.f); // Background Color
        viewMatrix = Transformation::createViewMatrix(glm::vec3(circleX, 0.0f, -circleZ), glm::vec3(glm::radians(0.0f), glm::radians(angleDegree), glm::radians(0.f)));

        imageSize = glm::vec2(1920, 1080);                         // Image Size
        lightPos = glm::vec4(-5000.0f, -15000.0f, -20000.f, 1.0f); // Light Position
        // objManager.loadObjFile("sphere", "./obj/sphere.obj");
        objManager.loadObjFile("sphere", "./obj/simplify_sphere_160.obj");
        objManager.transformTriangles("sphere", Transformation::scaleObj(10000000.f, 10000000.f, 10000000.f));

        // objManager.transformTriangles("hairball", Transformation::rotateObjX(glm::radians(180.f)));
        // objManager.transformTriangles("hairball", Transformation::rotateObjY(glm::radians(0.f)));
        // objManager.transformTriangles("hairball", Transformation::scaleObj(90.f, 90.f, 90.f));
        // objManager.transformTriangles("hairball", Transformation::changeObjPosition(glm::vec3(0.f, 50.f, 250.f)));
    }

        void bistroInterior(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree,
                glm::vec2 &imageSize, glm::vec4 &lightPos, glm::vec3 &backgroundColor)
    {

        float radius = 100000.0f;                         // Radius of the circle on which the camera moves
        float radians = glm::radians(angleDegree);        // Convert angle from degrees to radians
        float circleX = radius * std::sin(radians);       // Calculate x coordinate on the circle
        float circleZ = radius * std::cos(radians);       // Calculate z coordinate on the circle
        backgroundColor = glm::vec3(200.f, 200.f, 200.f); // Background Color
        viewMatrix = Transformation::createViewMatrix(glm::vec3(circleX, 0.0f, -circleZ), glm::vec3(glm::radians(0.0f), glm::radians(angleDegree), glm::radians(0.f)));

        imageSize = glm::vec2(1920, 1080);                         // Image Size
        lightPos = glm::vec4(-5000.0f, -15000.0f, -20000.f, 1.0f); // Light Position
        objManager.loadObjFile("bistro", "./obj/bistro/Interior/interior.obj");
        objManager.transformTriangles("bistro", Transformation::scaleObj(10.f, 10.f, 10.f));
        objManager.transformTriangles("bistro", Transformation::rotateObjX(glm::radians(-180.f)));
        objManager.transformTriangles("bistro", Transformation::rotateObjY(glm::radians(45.f)));

       objManager.transformTriangles("bistro", Transformation::changeObjPosition(glm::vec3(-2500, 1600, 6000.f)));
       // objManager.transformTriangles("bistro", Transformation::changeObjPosition(glm::vec3(-2500, 2000, 600000.f)));

       // objManager.transformTriangles("dragon", Transformation::rotateObjY(glm::radians(90.f)));

        // objManager.transformTriangles("hairball", Transformation::rotateObjX(glm::radians(180.f)));
        // objManager.transformTriangles("hairball", Transformation::rotateObjY(glm::radians(0.f)));
        // objManager.transformTriangles("hairball", Transformation::scaleObj(90.f, 90.f, 90.f));
        // objManager.transformTriangles("hairball", Transformation::changeObjPosition(glm::vec3(0.f, 50.f, 250.f)));
    }

}
