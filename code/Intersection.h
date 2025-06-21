#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include "Object.h"
#include "Datastructure.h"

namespace Intersection
{

    // Computes the normal of a given triangle
    glm::vec3 calculateTriangleNormal(const Triangle &triangle);

    // Computes the intersection of a ray with a triangle using Möller–Trumbore algorithm
    float rayTriangleIntersection(const Ray &ray, const Triangle &triangle);

    bool shadowIntersection(ObjectManager &objManager,Datastructure &datastructure, const glm::vec3 &lightPos, const float &fDistance, const Ray &ray);
    // int ambientOcclusion(ObjectManager &objManager,Datastructure &datastructure, const glm::vec3 &lightPos, const float &fDistance, const Ray &ray, float &occlusionDistance);

    // Computes the barycentric coordinates for a point within a triangle
    glm::vec3 calculateBarycentricCoords(const Triangle &triangle, const glm::vec3 &point);

    // Interpolates the normal at a given point using barycentric coordinates
    glm::vec3 interpolateNormal(const Triangle &triangle, const glm::vec3 &barycentricCoords);

} // namespace Intersection

#endif // INTERSECTION_H
