#include "Lighting.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <cmath>
#include <algorithm>

Lighting::Lighting()
    : m_keyLightTime(0.0f)
    , m_keyLightMinIntensity(0.3f)
    , m_keyLightMaxIntensity(1.0f)
    , m_currentPathIndex(0)
    , m_sentinelSpeed(2.0f)
    , m_pathProgress(0.0f)
{
    // Initialize Key Light (Directional)
    m_keyLight.type = LightType::DIRECTIONAL;
    m_keyLight.direction = glm::normalize(glm::vec3(-0.5f, -1.0f, -0.3f));
    m_keyLight.color = glm::vec3(1.0f, 0.95f, 0.9f); // Warm white
    m_keyLight.intensity = 0.7f;
    
    // Initialize Sentinel Light (Point)
    m_sentinelLight.type = LightType::POINT;
    m_sentinelLight.position = glm::vec3(0.0f, 3.0f, 0.0f);
    m_sentinelLight.color = glm::vec3(0.8f, 0.9f, 1.0f); // Cool blue-white
    m_sentinelLight.intensity = 1.0f;
    m_sentinelLight.range = 10.0f;
}

void Lighting::Update(float deltaTime) {
    UpdateKeyLight(deltaTime);
    UpdateSentinelLight(deltaTime);
}

void Lighting::SetKeyLight(const glm::vec3& direction, const glm::vec3& color, float minIntensity, float maxIntensity) {
    m_keyLight.direction = glm::normalize(direction);
    m_keyLight.color = color;
    m_keyLightMinIntensity = minIntensity;
    m_keyLightMaxIntensity = maxIntensity;
}

void Lighting::SetSentinelLightPath(const std::vector<glm::vec3>& path, float speed, const glm::vec3& color, float intensity) {
    m_sentinelPath = path;
    m_sentinelSpeed = speed;
    m_sentinelLight.color = color;
    m_sentinelLight.intensity = intensity;
    
    if (!path.empty()) {
        m_sentinelLight.position = path[0];
        m_currentPathIndex = 0;
        m_pathProgress = 0.0f;
    }
}

void Lighting::UpdateKeyLight(float deltaTime) {
    m_keyLightTime += deltaTime;
    
    // Oscillate intensity using sinusoidal curve
    float t = (std::sin(m_keyLightTime * 0.5f) + 1.0f) * 0.5f; // 0 to 1
    m_keyLight.intensity = m_keyLightMinIntensity + (m_keyLightMaxIntensity - m_keyLightMinIntensity) * t;
}

void Lighting::UpdateSentinelLight(float deltaTime) {
    if (m_sentinelPath.size() < 2) return;
    
    // Move along path
    int nextIndex = (m_currentPathIndex + 1) % m_sentinelPath.size();
    glm::vec3 current = m_sentinelPath[m_currentPathIndex];
    glm::vec3 next = m_sentinelPath[nextIndex];
    
    glm::vec3 direction = next - current;
    float segmentLength = glm::length(direction);
    
    if (segmentLength > 0.001f) {
        direction = glm::normalize(direction);
        m_pathProgress += m_sentinelSpeed * deltaTime;
        
        if (m_pathProgress >= segmentLength) {
            m_pathProgress = 0.0f;
            m_currentPathIndex = nextIndex;
            m_sentinelLight.position = m_sentinelPath[m_currentPathIndex];
        } else {
            m_sentinelLight.position = current + direction * m_pathProgress;
        }
    }
}

void Lighting::ApplyLights() const {
    // Apply lights to OpenGL
    // In modern OpenGL, this would be done via shader uniforms
    // For fixed pipeline (if needed), use glLight functions
    
    // Key Light (Directional)
    GLfloat keyLightDir[] = { m_keyLight.direction.x, m_keyLight.direction.y, m_keyLight.direction.z, 0.0f };
    GLfloat keyLightColor[] = { 
        m_keyLight.color.x * m_keyLight.intensity,
        m_keyLight.color.y * m_keyLight.intensity,
        m_keyLight.color.z * m_keyLight.intensity,
        1.0f 
    };
    
    // Sentinel Light (Point)
    GLfloat sentinelPos[] = { m_sentinelLight.position.x, m_sentinelLight.position.y, m_sentinelLight.position.z, 1.0f };
    GLfloat sentinelColor[] = { 
        m_sentinelLight.color.x * m_sentinelLight.intensity,
        m_sentinelLight.color.y * m_sentinelLight.intensity,
        m_sentinelLight.color.z * m_sentinelLight.intensity,
        1.0f 
    };
    
    // These would be set as shader uniforms in modern OpenGL
    // For now, placeholder for shader integration
}

