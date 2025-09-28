#include "Intersection.h"

namespace Intersection
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

    // Triangle Intersection
    // Reference: Möller, T. & Trumbore, B. (1997).
    // "Fast, Minimum Storage Ray-Triangle Intersection"
    // Journal of Graphics Tools.
    float rayTriangleIntersection(const Ray &ray, const Triangle &triangle)
    {
        // Intersection of a ray with a triangle
        // Triangle Point1 in Cartesian Form
        glm::vec3 tP1Cartesian = glm::vec3(triangle.pointOne) / triangle.pointOne.w;
        glm::vec3 tP2Cartesian = glm::vec3(triangle.pointTwo) / triangle.pointTwo.w;
        glm::vec3 tP3Cartesian = glm::vec3(triangle.pointThree) / triangle.pointThree.w;
        // edge vectors
        glm::vec3 p1p2 = tP2Cartesian - tP1Cartesian;
        glm::vec3 p1p3 = tP3Cartesian - tP1Cartesian;

        // determinant
        glm::vec3 pvec = glm::cross(ray.direction, p1p3);
        float det = glm::dot(p1p2, pvec);

        if (fabs(det) < 1e-12f)
            return -INFINITY;

        // inverse Determinant
        float invDet = 1.0f / det;
        // distance vector
        glm::vec3 tvec = ray.origin - tP1Cartesian;
        // u and v paramter
        float u = glm::dot(tvec, pvec) * invDet;
        if (u < 0.0f || u > 1.0f)
            return -INFINITY;
        glm::vec3 qvec = glm::cross(tvec, p1p2);
        float v = glm::dot(ray.direction, qvec) * invDet;
        if (v < 0.0f || u + v > 1.0f)
            return -INFINITY;
        // intersection distance
        float t = glm::dot(p1p3, qvec) * invDet;

        // Check if triangle is behind ray
        if (t < 0.0f)
            return -INFINITY;
        return t;
    }

    // Shadow Intersection
    // Sends out Ray from intersection to light source. If object is in between, there is shadow
    bool shadowIntersection(ObjectManager &objManager, Datastructure &datastructure, const glm::vec3 &lightPos, const float &fDistance, const Ray &ray)
    {
        const std::vector<Triangle> &triangles = objManager.triangles;
        // const std::vector<Triangle>& trianglesBox = pairShadow.second;
        Ray shadowRay(lightPos - ray.direction * fDistance);
        shadowRay.origin = ray.direction * fDistance; // - lightPos * 0.001f; // bias to avoid self-hit
        auto end = std::chrono::high_resolution_clock::now();

        // shadowRay.origin += ray.direction * 0.001f; // add small value to prevent shadowAcne
        // Prevents intersection between same object
        // if (shadowObjFilename != currentObjFilename) {
        // for (int i = 0; i < triangles.size(); i++)
        // {
        int boxCount = 0;
        for (int k : datastructure.checkIntersection(shadowRay, boxCount))
        {
            float shadowDistance = Intersection::rayTriangleIntersection(shadowRay, triangles[k]);
            if (shadowDistance != -INFINITY && shadowDistance > 0.001f) // 0.001f is a small bias to avoid self-hit with shadows
            // if (shadowDistance != -INFINITY && shadowDistance > 0.00000007f && shadowDistance < 0.00000175f) // Adds the range where AmbientOcclusion is applied.
            // if (shadowDistance != -INFINITY && shadowDistance > 0.00000007f && shadowDistance < 0.00000375f) // Adds the range where AmbientOcclusion is applied.

            // For further away objects, the AO is not applied
            {
                return true;
            }
        }
        return false;
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

    // Phong Shading
    // Concept: https://cg.informatik.uni-freiburg.de/course\_notes/graphics\_02\_shading.pdf
    glm::vec3 interpolateNormal(const Triangle &triangle, const glm::vec3 &barycentricCoords)
    {
        // By multiplying each vertex normal by its corresponding barycentric coordinate,
        // I am weighting each normal by how much influence that vertex has at the point of interest.
        return glm::normalize(
            barycentricCoords.x * triangle.normalOne +
            barycentricCoords.y * triangle.normalTwo +
            barycentricCoords.z * triangle.normalThree);
    }
} // namespace Intersection
