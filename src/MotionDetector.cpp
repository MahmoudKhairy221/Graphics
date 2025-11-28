#include "MotionDetector.h"
#include "Player.h"
#include "Camera.h"
#include "Collision.h"
#include "Scene.h"
#include "Audio.h"

MotionDetector::MotionDetector(const glm::vec3& position, float radius)
    : Obstacle(ObstacleType::MOTION_DETECTOR, position)
    , m_radius(radius)
    , m_triggered(false)
    , m_triggerCooldown(0.0f)
    , m_scene(nullptr)
{
    m_collisionBox = AABB(
        position - glm::vec3(radius),
        position + glm::vec3(radius)
    );
}

void MotionDetector::Update(float deltaTime) {
    if (m_triggerCooldown > 0.0f) {
        m_triggerCooldown -= deltaTime;
        if (m_triggerCooldown <= 0.0f) {
            m_triggered = false;
        }
    }
}

void MotionDetector::Render(Camera* camera) {
    if (!camera) return;
    
    // Render detector as a pulsing sphere when triggered
    // Placeholder rendering
}

bool MotionDetector::CheckCollision(const AABB& playerAABB) {
    glm::vec3 playerCenter = (playerAABB.min + playerAABB.max) * 0.5f;
    float distance = glm::length(playerCenter - m_position);
    
    if (distance < m_radius && !m_triggered) {
        m_triggered = true;
        m_triggerCooldown = 5.0f; // Alarm lasts 5 seconds
        TriggerAlarm();
        return true;
    }
    
    return false;
}

void MotionDetector::OnCollision(Player* player) {
    // Motion detector doesn't damage, just triggers alarm
    // Score penalty is handled by the alarm system
}

void MotionDetector::TriggerAlarm() {
    if (m_scene) {
        m_scene->TriggerAlarm();
        Audio::PlayAlarmSound();
    }
}

