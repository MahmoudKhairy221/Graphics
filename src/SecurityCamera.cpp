#include "SecurityCamera.h"
#include "Player.h"
#include "Camera.h"
#include "Collision.h"
#include "Scene.h"
#include "Audio.h"
#include <cmath>

SecurityCamera::SecurityCamera(const glm::vec3& position, float rotationSpeed, float fov, float range)
    : Obstacle(ObstacleType::SECURITY_CAMERA, position)
    , m_rotationAngle(0.0f)
    , m_rotationSpeed(rotationSpeed)
    , m_fov(fov)
    , m_range(range)
    , m_detectingPlayer(false)
    , m_scene(nullptr)
{
    m_collisionBox = AABB(
        position - glm::vec3(0.5f),
        position + glm::vec3(0.5f)
    );
}

void SecurityCamera::Update(float deltaTime) {
    m_rotationAngle += m_rotationSpeed * deltaTime;
    if (m_rotationAngle > 360.0f) {
        m_rotationAngle -= 360.0f;
    }
}

void SecurityCamera::Render(Camera* camera) {
    if (!camera) return;
    
    // Render camera body and detection cone
    // Placeholder rendering
}

glm::vec3 SecurityCamera::GetLookDirection() const {
    float rad = m_rotationAngle * 3.14159265359f / 180.0f;
    return glm::vec3(std::sin(rad), -0.3f, std::cos(rad)); // Slight downward angle
}

bool SecurityCamera::IsPlayerInFrustum(const glm::vec3& playerPos) const {
    glm::vec3 direction = GetLookDirection();
    return Collision::IsPointInFrustum(playerPos, m_position, direction, m_fov, m_range);
}

bool SecurityCamera::CheckCollision(const AABB& playerAABB) {
    glm::vec3 playerCenter = (playerAABB.min + playerAABB.max) * 0.5f;
    
    bool wasDetecting = m_detectingPlayer;
    m_detectingPlayer = IsPlayerInFrustum(playerCenter);
    
    if (m_detectingPlayer && !wasDetecting) {
        // Just detected player - trigger alarm
        if (m_scene) {
            m_scene->TriggerAlarm();
            Audio::PlayAlarmSound();
        }
        return true;
    }
    
    return false;
}

void SecurityCamera::OnCollision(Player* player) {
    // Camera doesn't damage directly, but triggers alarm on detection
}

