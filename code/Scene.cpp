#include "Scene.h"
#include <random>

// Here the scenes are defined.
// Each scene can be selected in main.cpp and results in a different image.
// There are multiple parameters which can be definded for each scene, like the image size and more.

// 3D Models used for the scenes:

// Sphere
// Source: William, Blender (3D OBJ Model)

// Chinese Dragon
// Source: Georgia Tech, 2011 (3D OBJ Model)

// Hairball
// Source: Nvidia Research, Samuli Laine & Tero Karras, 2011 (3D OBJ Model)

// Amazon Lumberyard Bistro
// Source: Nvidia Research, A. Lumberyard, 2017 (3D OBJ Model)

// Horse Chestnut Tree
// Source: Xfrog, 2017 (3D OBJ Model)

// Rectangular Grass Patch
// Source: printable_models, 2019 (3D OBJ Model)

// Computer Graphics Archive
// Source: Morgan McGuire, July 2017

namespace Scene
{
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

        // generate trees with random positions to make it look more natural.
        // Each tree has 316880 triangles, so 3 trees are around 1 million triangles.
        int i_end = 39;
        int j_end = 15;
        int x_start = -900;
        int y_start = -620;
        int y_end = 900;
        std::random_device rd;
        std::mt19937 gen(rd());

        for (int i = 0; i <= i_end; i++)
        {
            for (int j = 0; j <= j_end; j++)
            {
                // Example: random float between -5.0 and 5.0
                std::uniform_real_distribution<float> dist1(.21f, 0.36f);
                float scatter = 50.f;
                std::uniform_real_distribution<float> dist2(-scatter, scatter);
                std::uniform_real_distribution<float> dist3(-scatter, scatter);
                float scaleX = dist1(gen);
                float x_offset = dist2(gen);
                float y_offset = dist3(gen);
                std::string treeName = std::to_string(j) + "tree" + std::to_string(i);
                std::cout << "Loading " << treeName << std::endl;
                objManager.loadObjFile(treeName, "./obj/chestnut/chestnut.obj");
                float scale = 1.5f;
                objManager.transformTriangles(treeName, Transformation::scaleObj(scaleX, scaleX, scaleX));
                objManager.transformTriangles(treeName, Transformation::rotateObjX(glm::radians(-90.f)));
                objManager.transformTriangles(treeName, Transformation::rotateObjY(glm::radians(-270.f)));
                objManager.transformTriangles(treeName, Transformation::changeObjPosition(glm::vec3(x_start + i * (-x_start * 2) / i_end + x_offset, -80.f, y_start + j * (y_end - y_start) / j_end + y_offset)));
            }
        }

        /*
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

        */
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

        imageSize = glm::vec2(1920, 1080);                // Image Size
        lightPos = glm::vec4(0.0f, -15000.0f, 0.f, 1.0f); // Light Position
        // create 10 more trees in a loop for a forest-like scene

        objManager.loadObjFile("cube", "./obj/cube.obj");
        // objManager.transformTriangles("grass", Transformation::rotateObjX(glm::radians(-90.f)));
        objManager.transformTriangles("cube", Transformation::scaleObj(1500.f, 50.f, 1000.f));
        // objManager.transformTriangles("grass", Transformation::changeObjPosition(glm::vec3(0.f, -90.f, -400.f)));

        objManager.loadObjFile("sphere", "./obj/sphere.obj");
        objManager.transformTriangles("sphere", Transformation::scaleObj(50.f, 50.f, 50.f));
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
    }

    void ambientOcclusionSphere(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree,
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
        // Choose how many light sources are used for ambient occlusion based on sphere size
        // objManager.loadObjFile("sphere", "./obj/sphere.obj");
        objManager.loadObjFile("sphere", "./obj/simplify_sphere_160.obj");
        objManager.transformTriangles("sphere", Transformation::scaleObj(10000000.f, 10000000.f, 10000000.f));
    }

    void bistroInterior(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree,
                        glm::vec2 &imageSize, glm::vec4 &lightPos, glm::vec3 &backgroundColor)
    {

        float radius = 100000.0f;                         // Radius of the circle on which the camera moves
        float radians = glm::radians(angleDegree);        // Convert angle from degrees to radians
        float circleX = radius * std::sin(radians);       // Calculate x coordinate on the circle
        float circleZ = radius * std::cos(radians);       // Calculate z coordinate on the circle
        backgroundColor = glm::vec3(400.f, 200.f, 200.f); // Background Color
        viewMatrix = Transformation::createViewMatrix(glm::vec3(circleX, 0.0f, -circleZ), glm::vec3(glm::radians(0.0f), glm::radians(angleDegree), glm::radians(0.f)));

        imageSize = glm::vec2(1920, 1080); // Image Size
        // lightPos = glm::vec4(-5000.0f, -15000.0f, -20000.f, 1.0f); // Light Position
        lightPos = glm::vec4(5000.0f, -15000.0f, 0000.f, 1.0f); // Light Position
        objManager.loadObjFile("bistro", "./obj/bistro/Interior/interior_a.obj");
        objManager.transformTriangles("bistro", Transformation::scaleObj(250.f, 250.f, 250.f));
        objManager.transformTriangles("bistro", Transformation::rotateObjX(glm::radians(-180.f)));
        objManager.transformTriangles("bistro", Transformation::rotateObjY(glm::radians(25.f)));
        objManager.transformTriangles("bistro", Transformation::rotateObjX(glm::radians(-20.f)));

        objManager.transformTriangles("bistro", Transformation::changeObjPosition(glm::vec3(-1300, 700, 60.f)));
    }

    void bistroInterior1(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree,
                         glm::vec2 &imageSize, glm::vec4 &lightPos, glm::vec3 &backgroundColor)
    {

        float radius = 100000.0f;                         // Radius of the circle on which the camera moves
        float radians = glm::radians(angleDegree);        // Convert angle from degrees to radians
        float circleX = radius * std::sin(radians);       // Calculate x coordinate on the circle
        float circleZ = radius * std::cos(radians);       // Calculate z coordinate on the circle
        backgroundColor = glm::vec3(400.f, 200.f, 200.f); // Background Color
        viewMatrix = Transformation::createViewMatrix(glm::vec3(circleX, 0.0f, -circleZ), glm::vec3(glm::radians(0.0f), glm::radians(angleDegree), glm::radians(0.f)));

        imageSize = glm::vec2(1920, 1080); // Image Size
        // lightPos = glm::vec4(-5000.0f, -15000.0f, -20000.f, 1.0f); // Light Position
        lightPos = glm::vec4(5000.0f, -15000.0f, 0000.f, 1.0f); // Light Position
        objManager.loadObjFile("bistro", "./obj/bistro/Interior/interior_a.obj");
        objManager.transformTriangles("bistro", Transformation::scaleObj(270.f, 270.f, 270.f));
        objManager.transformTriangles("bistro", Transformation::rotateObjX(glm::radians(-180.f)));
        objManager.transformTriangles("bistro", Transformation::rotateObjY(glm::radians(65.f)));
        objManager.transformTriangles("bistro", Transformation::rotateObjX(glm::radians(-15.f)));

        objManager.transformTriangles("bistro", Transformation::changeObjPosition(glm::vec3(-100, 800, -10000.f)));
    }

    void bistroExterior(ObjectManager &objManager, glm::mat4 &viewMatrix, const float &angleDegree,
                        glm::vec2 &imageSize, glm::vec4 &lightPos, glm::vec3 &backgroundColor)
    {

        float radius = 100000.0f;                         // Radius of the circle on which the camera moves
        float radians = glm::radians(angleDegree);        // Convert angle from degrees to radians
        float circleX = radius * std::sin(radians);       // Calculate x coordinate on the circle
        float circleZ = radius * std::cos(radians);       // Calculate z coordinate on the circle
        backgroundColor = glm::vec3(200.f, 200.f, 200.f); // Background Color
        // for each degree low down increase lookDownY by 2000
        float lookDownAngle = 12.0f; // 1.9f; // Angle to look down
        float lookDownY = -21000.f;  // -1800.f;  // Y position to look down from
        viewMatrix = Transformation::createViewMatrix(glm::vec3(circleX, lookDownY, -circleZ), glm::vec3(glm::radians(lookDownAngle), glm::radians(angleDegree), glm::radians(0.f)));
        imageSize = glm::vec2(1920, 1080);                      // Image Size
        lightPos = glm::vec4(5000.0f, -15000.0f, 0000.f, 1.0f); // Light Position
        objManager.loadObjFile("bistro", "./obj/bistro/Exterior/exterior_a.obj");
        objManager.transformTriangles("bistro", Transformation::scaleObj(100.f, 100.f, 100.f));
        objManager.transformTriangles("bistro", Transformation::rotateObjX(glm::radians(-180.f)));
        objManager.transformTriangles("bistro", Transformation::rotateObjY(glm::radians(75.f)));
        objManager.transformTriangles("bistro", Transformation::changeObjPosition(glm::vec3(00, 600, 0.f)));
    }
}
