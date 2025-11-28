#ifndef GLM_VEC3_HPP
#define GLM_VEC3_HPP

#include <cmath>

namespace glm {
    struct vec3 {
        float x, y, z;
        
        vec3() : x(0), y(0), z(0) {}
        vec3(float x, float y, float z) : x(x), y(y), z(z) {}
        
        vec3 operator+(const vec3& v) const { return vec3(x + v.x, y + v.y, z + v.z); }
        vec3 operator-(const vec3& v) const { return vec3(x - v.x, y - v.y, z - v.z); }
        vec3 operator*(float s) const { return vec3(x * s, y * s, z * s); }
        vec3 operator/(float s) const { return vec3(x / s, y / s, z / s); }
        
        float length() const { return std::sqrt(x*x + y*y + z*z); }
        vec3 normalize() const { float l = length(); return l > 0 ? (*this) / l : vec3(); }
    };
    
    inline float dot(const vec3& a, const vec3& b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
    inline vec3 cross(const vec3& a, const vec3& b) {
        return vec3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
    }
    inline float length(const vec3& v) { return v.length(); }
    inline vec3 normalize(const vec3& v) { return v.normalize(); }
    inline vec3 mix(const vec3& a, const vec3& b, float t) {
        return vec3(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t);
    }
}

#endif

