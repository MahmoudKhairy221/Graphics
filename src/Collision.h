#ifndef COLLISION_H
#define COLLISION_H

#include <glm/glm.hpp>
#include <vector>

struct AABB {
    glm::vec3 min;
    glm::vec3 max;
    
    AABB() : min(0.0f), max(0.0f) {}
    AABB(const glm::vec3& min, const glm::vec3& max) : min(min), max(max) {}
    
    bool Intersects(const AABB& other) const;
    bool Contains(const glm::vec3& point) const;
};

struct Sphere {
    glm::vec3 center;
    float radius;
    
    Sphere() : center(0.0f), radius(0.0f) {}
    Sphere(const glm::vec3& center, float radius) : center(center), radius(radius) {}
    
    bool Intersects(const Sphere& other) const;
    bool Intersects(const AABB& box) const;
    bool Contains(const glm::vec3& point) const;
};

struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;
    
    Ray(const glm::vec3& origin, const glm::vec3& direction) 
        : origin(origin), direction(glm::normalize(direction)) {}
    
    bool Intersects(const AABB& box, float& t) const;
    bool Intersects(const Sphere& sphere, float& t) const;
};

class Collision {
public:
    static bool CheckAABBAABB(const AABB& a, const AABB& b);
    static bool CheckSphereSphere(const Sphere& a, const Sphere& b);
    static bool CheckSphereAABB(const Sphere& sphere, const AABB& box);
    static bool CheckPointAABB(const glm::vec3& point, const AABB& box);
    static bool CheckPointSphere(const glm::vec3& point, const Sphere& sphere);
    static bool CheckRayAABB(const Ray& ray, const AABB& box, float& t);
    static bool CheckRaySphere(const Ray& ray, const Sphere& sphere, float& t);
    
    // Frustum culling for detection cones
    static bool IsPointInFrustum(const glm::vec3& point, const glm::vec3& origin, 
                                 const glm::vec3& direction, float fov, float maxDistance);
};

#endif // COLLISION_H

