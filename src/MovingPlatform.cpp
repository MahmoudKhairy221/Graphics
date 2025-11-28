#include "MovingPlatform.h"
#include "Player.h"
#include "Camera.h"
#include "Collision.h"

MovingPlatform::MovingPlatform(const glm::vec3& startPos, const glm::vec3& endPos, float speed)
    : Obstacle(ObstacleType::MOVING_PLATFORM, startPos)
    , m_startPos(startPos)
    , m_endPos(endPos)
    , m_speed(speed)
    , m_traveled(0.0f)
    , m_forward(true)
    , m_playerOnPlatform(false)
{
    m_direction = glm::normalize(endPos - startPos);
    m_distance = glm::length(endPos - startPos);
    
    m_collisionBox = AABB(
        m_position - glm::vec3(1.0f, 0.2f, 1.0f),
        m_position + glm::vec3(1.0f, 0.2f, 1.0f)
    );
}

void MovingPlatform::Update(float deltaTime) {
    UpdatePosition(deltaTime);
    
    // Update collision box
    m_collisionBox = AABB(
        m_position - glm::vec3(1.0f, 0.2f, 1.0f),
        m_position + glm::vec3(1.0f, 0.2f, 1.0f)
    );
    
    m_playerOnPlatform = false;
}

void MovingPlatform::UpdatePosition(float deltaTime) {
    float moveDistance = m_speed * deltaTime;
    
    if (m_forward) {
        m_traveled += moveDistance;
        if (m_traveled >= m_distance) {
            m_traveled = m_distance;
            m_forward = false;
        }
    } else {
        m_traveled -= moveDistance;
        if (m_traveled <= 0.0f) {
            m_traveled = 0.0f;
            m_forward = true;
        }
    }
    
    m_position = m_startPos + m_direction * m_traveled;
}

void MovingPlatform::Render(Camera* camera) {
    if (!camera) return;
    
    // Render platform
    // Placeholder rendering
}

bool MovingPlatform::CheckCollision(const AABB& playerAABB) {
    if (m_collisionBox.Intersects(playerAABB)) {
        m_playerOnPlatform = true;
        // Move player with platform
        return true;
    }
    return false;
}

void MovingPlatform::OnCollision(Player* player) {
    if (player && m_playerOnPlatform) {
        // Player rides the platform - move player with platform velocity
        glm::vec3 platformVelocity = m_forward ? m_direction * m_speed : -m_direction * m_speed;
        // Apply velocity to player (handled in player update)
    }
}

