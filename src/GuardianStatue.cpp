#include "GuardianStatue.h"
#include "Player.h"
#include "Camera.h"
#include "Collision.h"
#include "Audio.h"

GuardianStatue::GuardianStatue(const glm::vec3& startPos, const std::vector<glm::vec3>& patrolPath, float speed)
    : Obstacle(ObstacleType::GUARDIAN_STATUE, startPos)
    , m_patrolPath(patrolPath)
    , m_currentPathIndex(0)
    , m_speed(speed)
    , m_knockbackForce(5.0f)
    , m_damage(20.0f)
{
    if (m_patrolPath.empty()) {
        m_patrolPath.push_back(startPos);
    }
    
    m_collisionBox = AABB(
        m_position - glm::vec3(0.8f, 1.5f, 0.8f),
        m_position + glm::vec3(0.8f, 1.5f, 0.8f)
    );
}

void GuardianStatue::Update(float deltaTime) {
    UpdatePatrol(deltaTime);
    
    // Update collision box
    m_collisionBox = AABB(
        m_position - glm::vec3(0.8f, 1.5f, 0.8f),
        m_position + glm::vec3(0.8f, 1.5f, 0.8f)
    );
}

void GuardianStatue::UpdatePatrol(float deltaTime) {
    if (m_patrolPath.size() <= 1) return;
    
    glm::vec3 target = GetNextPatrolPoint();
    glm::vec3 direction = glm::normalize(target - m_position);
    float distance = glm::length(target - m_position);
    
    if (distance < 0.1f) {
        m_currentPathIndex = (m_currentPathIndex + 1) % m_patrolPath.size();
    } else {
        m_position += direction * m_speed * deltaTime;
    }
}

glm::vec3 GuardianStatue::GetNextPatrolPoint() const {
    if (m_patrolPath.empty()) return m_position;
    return m_patrolPath[(m_currentPathIndex + 1) % m_patrolPath.size()];
}

void GuardianStatue::Render(Camera* camera) {
    if (!camera) return;
    
    // Render statue
    // Placeholder rendering
}

bool GuardianStatue::CheckCollision(const AABB& playerAABB) {
    return m_collisionBox.Intersects(playerAABB);
}

void GuardianStatue::OnCollision(Player* player) {
    if (player) {
        player->TakeDamage(m_damage);
        Audio::PlayCollisionSound();
        
        // Apply knockback
        glm::vec3 knockbackDir = glm::normalize(player->GetPosition() - m_position);
        knockbackDir.y = 0.0f; // Horizontal knockback only
        // Apply knockback to player (would need player method for this)
    }
}

