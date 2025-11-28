#ifndef LIGHTING_H
#define LIGHTING_H

#include <glm/glm.hpp>
#include <vector>

enum class LightType {
    DIRECTIONAL,
    POINT,
    SPOT
};

struct Light {
    LightType type;
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
    float range; // For point/spot lights
    float innerCone; // For spot lights
    float outerCone; // For spot lights
};

class Lighting {
public:
    Lighting();
    
    void Update(float deltaTime);
    
    // Key Light (Directional) - oscillating intensity
    void SetKeyLight(const glm::vec3& direction, const glm::vec3& color, float minIntensity, float maxIntensity);
    Light GetKeyLight() const { return m_keyLight; }
    
    // Sentinel Light (Point/Spot) - moving along path
    void SetSentinelLightPath(const std::vector<glm::vec3>& path, float speed, const glm::vec3& color, float intensity);
    Light GetSentinelLight() const { return m_sentinelLight; }
    
    void ApplyLights() const; // Apply to OpenGL
    
private:
    Light m_keyLight;
    Light m_sentinelLight;
    
    // Key Light animation
    float m_keyLightTime;
    float m_keyLightMinIntensity;
    float m_keyLightMaxIntensity;
    
    // Sentinel Light animation
    std::vector<glm::vec3> m_sentinelPath;
    int m_currentPathIndex;
    float m_sentinelSpeed;
    float m_pathProgress;
    
    void UpdateKeyLight(float deltaTime);
    void UpdateSentinelLight(float deltaTime);
};

#endif // LIGHTING_H

