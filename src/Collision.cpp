#include "Collision.h"
#include <algorithm>
#include <cmath>

bool AABB::Intersects(const AABB& other) const {
    return (min.x <= other.max.x && max.x >= other.min.x) &&
           (min.y <= other.max.y && max.y >= other.min.y) &&
           (min.z <= other.max.z && max.z >= other.min.z);
}

bool AABB::Contains(const glm::vec3& point) const {
    return point.x >= min.x && point.x <= max.x &&
           point.y >= min.y && point.y <= max.y &&
           point.z >= min.z && point.z <= max.z;
}

bool Sphere::Intersects(const Sphere& other) const {
    float distance = glm::length(center - other.center);
    return distance < (radius + other.radius);
}

bool Sphere::Intersects(const AABB& box) const {
    glm::vec3 closestPoint;
    closestPoint.x = std::max(box.min.x, std::min(center.x, box.max.x));
    closestPoint.y = std::max(box.min.y, std::min(center.y, box.max.y));
    closestPoint.z = std::max(box.min.z, std::min(center.z, box.max.z));
    
    float distance = glm::length(center - closestPoint);
    return distance < radius;
}

bool Sphere::Contains(const glm::vec3& point) const {
    return glm::length(center - point) <= radius;
}

bool Ray::Intersects(const AABB& box, float& t) const {
    float tmin = 0.0f;
    float tmax = 1000000.0f; // Large value instead of numeric_limits
    
    for (int i = 0; i < 3; i++) {
        if (std::abs(direction[i]) < 0.0001f) {
            if (origin[i] < box.min[i] || origin[i] > box.max[i]) {
                return false;
            }
        } else {
            float ood = 1.0f / direction[i];
            float t1 = (box.min[i] - origin[i]) * ood;
            float t2 = (box.max[i] - origin[i]) * ood;
            
            if (t1 > t2) std::swap(t1, t2);
            tmin = std::max(tmin, t1);
            tmax = std::min(tmax, t2);
            
            if (tmin > tmax) return false;
        }
    }
    
    t = tmin;
    return true;
}

bool Ray::Intersects(const Sphere& sphere, float& t) const {
    glm::vec3 oc = origin - sphere.center;
    float a = glm::dot(direction, direction);
    float b = 2.0f * glm::dot(oc, direction);
    float c = glm::dot(oc, oc) - sphere.radius * sphere.radius;
    float discriminant = b * b - 4 * a * c;
    
    if (discriminant < 0) return false;
    
    float sqrtd = std::sqrt(discriminant);
    float t0 = (-b - sqrtd) / (2.0f * a);
    float t1 = (-b + sqrtd) / (2.0f * a);
    
    if (t0 < 0 && t1 < 0) return false;
    
    t = (t0 < 0) ? t1 : t0;
    return true;
}

bool Collision::CheckAABBAABB(const AABB& a, const AABB& b) {
    return a.Intersects(b);
}

bool Collision::CheckSphereSphere(const Sphere& a, const Sphere& b) {
    return a.Intersects(b);
}

bool Collision::CheckSphereAABB(const Sphere& sphere, const AABB& box) {
    return sphere.Intersects(box);
}

bool Collision::CheckPointAABB(const glm::vec3& point, const AABB& box) {
    return box.Contains(point);
}

bool Collision::CheckPointSphere(const glm::vec3& point, const Sphere& sphere) {
    return sphere.Contains(point);
}

bool Collision::CheckRayAABB(const Ray& ray, const AABB& box, float& t) {
    return ray.Intersects(box, t);
}

bool Collision::CheckRaySphere(const Ray& ray, const Sphere& sphere, float& t) {
    return ray.Intersects(sphere, t);
}

bool Collision::IsPointInFrustum(const glm::vec3& point, const glm::vec3& origin,
                                 const glm::vec3& direction, float fov, float maxDistance) {
    glm::vec3 toPoint = point - origin;
    float distance = glm::length(toPoint);
    
    if (distance > maxDistance) return false;
    
    toPoint = glm::normalize(toPoint);
    float angle = std::acos(glm::dot(direction, toPoint));
    float halfFov = fov * 3.14159265359f / 180.0f * 0.5f;
    
    return angle <= halfFov;
}

