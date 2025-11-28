#include "Portal.h"
#include "Camera.h"
#include "Collision.h"
#include "Audio.h"
#include <cmath>

Portal::Portal(const glm::vec3& position, bool requiresGateCondition)
    : m_position(position)
    , m_unlocked(false)
    , m_requiresGateCondition(requiresGateCondition)
    , m_gateConditionMet(false)
    , m_pulseTime(0.0f)
    , m_scale(1.0f)
    , m_emissiveIntensity(1.0f)
{
}

void Portal::Update(float deltaTime) {
    if (m_unlocked && CanActivate()) {
        UpdateAnimation(deltaTime);
    }
}

void Portal::UpdateAnimation(float deltaTime) {
    m_pulseTime += deltaTime;
    
    // Pulsing scale animation
    m_scale = 1.0f + std::sin(m_pulseTime * 2.0f) * 0.1f;
    
    // Pulsing emissive intensity
    m_emissiveIntensity = 0.8f + std::sin(m_pulseTime * 3.0f) * 0.2f;
}

void Portal::Render(Camera* camera) {
    if (!camera || !m_unlocked) return;
    
    RenderMesh();
}

void Portal::RenderMesh() {
    // Render portal with emissive shader
    // Scaling animation and emissive pulse
    // Placeholder rendering
}

bool Portal::CheckPlayerReach(const glm::vec3& playerPos) const {
    float distance = glm::length(playerPos - m_position);
    if (distance < 1.5f && m_unlocked && CanActivate()) {
        Audio::PlayPortalSound();
        return true;
    }
    return false;
}

