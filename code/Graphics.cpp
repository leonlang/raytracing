#include "Graphics.h"

namespace Graphics
{

    // Normal Interpolation
    // Concept for the Algorithm: https://www.khronos.org/opengl/wiki/Calculating_a_Surface_Normal
    glm::vec3 calculateTriangleNormal(const Triangle &triangle)
    {
        glm::vec3 v1 = triangle.pointTwo - triangle.pointOne;
        glm::vec3 v2 = triangle.pointThree - triangle.pointOne;
        glm::vec3 normal = glm::cross(v1, v2);
        return glm::normalize(normal);
    }

    // Phong Shading
    // Concept for the Algorithm: https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/barycentric-coordinates.html
    glm::vec3 calculateBarycentricCoords(const Triangle &triangle, const glm::vec3 &point)
    {
        // Triangle Point1 in Cartesian Form
        glm::vec3 tP1Cartesian = glm::vec3(triangle.pointOne) / triangle.pointOne.w;
        glm::vec3 tP2Cartesian = glm::vec3(triangle.pointTwo) / triangle.pointTwo.w;
        glm::vec3 tP3Cartesian = glm::vec3(triangle.pointThree) / triangle.pointThree.w;

        // Calculate the vectors from pointOne to the other two vertices of the triangle
        // These vectors represent the edges of the triangle and the vector from the
        // first vertex to the point of interest. They are used for determining the relative
        // position of the point within the triangle.
        glm::vec3 v0 = tP2Cartesian - tP1Cartesian;
        glm::vec3 v1 = tP3Cartesian - tP1Cartesian;
        glm::vec3 v2 = point - tP1Cartesian;

        // The dot products are used to calculate the areas and angles between the vectors.
        // These values are crucial for the barycentric coordinates formula, which determines
        // how much of each vertex's influence is present at the point.
        float d00 = glm::dot(v0, v0);
        float d01 = glm::dot(v0, v1);
        float d11 = glm::dot(v1, v1);
        float d20 = glm::dot(v2, v0);
        float d21 = glm::dot(v2, v1);

        // Compute the denominator of the barycentric coordinates formula
        // It normalizes the coordinates, ensuring they sum to 1.
        // This step ensures that the point lies within the triangle
        float denom = d00 * d11 - d01 * d01;

        // Compute the barycentric coordinates (v, w, u)
        // v and w are calculated using the dot products and the denominator
        float v = (d11 * d20 - d01 * d21) / denom;
        float w = (d00 * d21 - d01 * d20) / denom;
        // u is calculated to ensure the sum of the barycentric coordinates is 1
        float u = 1.0f - v - w;

        // Return the barycentric coordinates as a vec3
        // std::cout << "1:" << u << "2:" << v << "3:" << w << std::endl;
        return glm::vec3(u, v, w);
    }

    // Lighting with Phong Illumination Model
    // Concept is found here: https://cg.informatik.uni-freiburg.de/course_notes/graphics_02_shading.pdf
    glm::vec3 phongIllumination(ObjectManager &objManager, const Triangle &triangle, const Ray &ray, const glm::vec3 &lightPos, const float &distance)
    {

        glm::vec3 objColor(1, 1, 1);
        if (!triangle.textureName.empty())
        {
            glm::vec3 intersectionPoint = ray.origin + distance * ray.direction;
            glm::vec2 interpolatedTexCoordinate = getTextureCoordinate(calculateBarycentricCoords(triangle, intersectionPoint), triangle.colorOneCoordinate, triangle.colorTwoCoordinate, triangle.colorThreeCoordinate);
            // std::cout << "Interpolated X:" << interpolatedTexCoordinate.x << "Xone:" << triangle->colorOneCoordinate.x << "Xtwo:" << triangle->colorTwoCoordinate.x << "XThree:" << triangle->colorThreeCoordinate.x << std::endl;
            unsigned char *texData = objManager.textureData[triangle.textureName];
            glm::ivec2 texDim = objManager.textureDimensions[triangle.textureName];

            size_t texIndex = (static_cast<int>(interpolatedTexCoordinate.y) * texDim.x + static_cast<int>(interpolatedTexCoordinate.x)) * 3;
            objColor.x = texData[texIndex + 0] / 255.0f;
            objColor.y = texData[texIndex + 1] / 255.0f;
            objColor.z = texData[texIndex + 2] / 255.0f;
        }

        // Phong illumination model
        // std::cout << "Triangle Diffuse: " << triangle.diffuse.x << " " << triangle.diffuse.y << " " << triangle.diffuse.z << std::endl;
        const glm::vec3 lightColor(1.0f, 1.0f, 1.0f); // White light
        // rView is a constant factor for the reflection model, representing the light reflected to the view position
        // smaller number = less light reflected to view position = diffuse plays a smaller role
        constexpr float rView = 1.0f / glm::pi<float>();

        // Calculate the intersection point of the ray with the triangle
        glm::vec3 intersectionPoint = ray.origin + distance * ray.direction;

        // Calculate barycentric coordinates for the intersection point within the triangle
        glm::vec3 barycentricCoords = calculateBarycentricCoords(triangle, intersectionPoint);

        // Interpolate the normal at the intersection point using barycentric coordinates
        // glm::vec3 n = interpolateNormal(*triangle, barycentricCoords); // normal
        // test with normal
        glm::vec3 n = calculateTriangleNormal(triangle);
        // Calculate the direction vector from the intersection point to the light source
        glm::vec3 l = glm::normalize(lightPos - intersectionPoint); // lightDirection

        // Calculate Diffuse Reflection
        // Diffuse reflection is based on Lambert's cosine law, which states that the intensity of light is proportional to the
        // cosine of the angle between the light direction and the surface normal
        // Means: intensity of light is is higher if the angle is sharper
        // The dot product n * l represents this cosine value, and I use max to ensure it is non-negative
        // objectColor * lightColor represents the final color of object with light
        float dotProduct = glm::dot(n, l);
        if (dotProduct < 0.0f)
        {
            dotProduct = -dotProduct;
        }
        // glm::vec3 diffuse = rView * objectColor * lightColor * glm::max(dotProduct, 0.00f);
        glm::vec3 diffuse = rView * triangle.diffuse * objColor * lightColor * glm::max(dotProduct, 0.00f);

        // Calculate Ambient Reflection
        // Ambient reflection represents the constant illumination of the object by the environment
        // It is usually a small constant value added to ensure that objects are visible even when not directly lit
        // Higher AmbientStrenght = All of the Object brightens up more by the same amount
        glm::vec3 ambient = (1 / glm::pi<float>()) * triangle.ambient * objColor * lightColor;
   
        // Calculate Specular Reflection
        // Specular reflection represents the mirror-like reflection of light sources on shiny surfaces
        // It does not use the object color (objectColor) because specular highlights are typically the color of the light source
        // Higher shininess means a smaller specular highlight
        glm::vec3 v = glm::normalize(-ray.direction); // View Direction
        glm::vec3 r = glm::reflect(-l, n);            // Reflect Direction

        // The specular term is calculated using the Phong reflection model
        // It is based on the dot product between the view direction and the reflection direction, raised to the power of the shininess factor (shininess)
        // specularStrength = specular strength. Smaller specular strength means less intensity
        glm::vec3 specular = lightColor * triangle.specular * glm::max(dotProduct, 0.00f) * glm::pow(glm::max(glm::dot(r, v), 0.0f), triangle.shininess);

        // Combine the three components (diffuse, specular, and ambient) to get the final color
        return diffuse + specular + ambient; 
    }

    glm::vec2 getTextureCoordinate(const glm::vec3 &barycentricCoords, const glm::vec2 &texCoordA, const glm::vec2 &texCoordB, const glm::vec2 &texCoordC)
    {
        // Interpolate the texture coordinates using the barycentric coordinates
        glm::vec2 texCoord = barycentricCoords.x * texCoordA + barycentricCoords.y * texCoordB + barycentricCoords.z * texCoordC;

        return texCoord;
    }

    void reinhardtToneMapping(glm::vec3 &color, float exposure, float gamma)
    {
        color = color / (color + exposure);
        // color = color / (color + 0.5f);
        // color = color / (color + 0.1f);
        // color = color / (color + 4.0f);

        // Add Gamma
        // glm::vec3 gamma(1.1f, 1.1f, 1.1f);
        // glm::vec3 gamma (2.2f, 2.2f, 2.2f);
        color = glm::pow(color, glm::vec3(gamma));
    }

    std::vector<glm::vec3> generateRandomCoordinates(int amount, float range)
    {
        std::vector<glm::vec3> coordinates;
        glm::vec3 coordinate(0, 0, 0);
        for (int i = 0; i < amount; i++)
        {
            switch (i % 3)
            {
            case 0:
                coordinate.x += range;
                break;
            case 1:
                coordinate.y += range;
                break;
            case 2:
                coordinate.z += range;
                break;
            }
            coordinates.push_back(coordinate);
        }
        return coordinates;
    }

    void drawImage(const glm::vec2 &imgSize, const std::vector<glm::vec2> &imagePoints,
                   const std::vector<glm::vec3> &imageColors, const int &angleDegree,
                   const bool &saveImage, const bool &displayImage)
    {

        // Create image
        CImg<unsigned char> img(imgSize.x, imgSize.y, 1, 3);
        img.fill(0);

        // Draw pixels found in ray intersection
        for (size_t i = 0; i < imagePoints.size(); i++)
        {
            unsigned char color[] = {0, 0, 0};
            color[0] = static_cast<unsigned char>(imageColors[i].x);
            color[1] = static_cast<unsigned char>(imageColors[i].y);
            color[2] = static_cast<unsigned char>(imageColors[i].z);
            img.draw_point(imagePoints[i].x, imagePoints[i].y, color);
        }
        // Generate image name based on angle
        std::string imgName = "images/generation/output" + std::to_string(angleDegree) + ".bmp";

        if (saveImage)
        {
            img.save_bmp(imgName.c_str());
        }
        if (displayImage)
        {
            img.display("Simple Raytracer by Leon Lang");
        }
    }

}
