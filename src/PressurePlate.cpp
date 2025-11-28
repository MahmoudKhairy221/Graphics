#include "PressurePlate.h"
#include "Player.h"
#include "Camera.h"
#include "Collision.h"
#include "Audio.h"

PressurePlate::PressurePlate(const glm::vec3& position, float triggerDelay)
    : Obstacle(ObstacleType::PRESSURE_PLATE, position)
    , m_triggerDelay(triggerDelay)
    , m_triggered(false)
    , m_triggerTimer(0.0f)
    , m_animationTime(0.0f)
    , m_dartFired(false)
{
    m_collisionBox = AABB(
        position - glm::vec3(0.5f, 0.1f, 0.5f),
        position + glm::vec3(0.5f, 0.1f, 0.5f)
    );
}

void PressurePlate::Update(float deltaTime) {
    m_animationTime += deltaTime;
    
    if (m_triggered) {
        m_triggerTimer += deltaTime;
        
        if (m_triggerTimer >= m_triggerDelay && !m_dartFired) {
            FireDart();
            TriggerTrapDoor();
            m_dartFired = true;
        }
    }
}

void PressurePlate::Render(Camera* camera) {
    if (!camera) return;
    
    // Render plate with animation (telegraph timing)
    // Plate depresses when triggered
    float depression = m_triggered ? 0.05f : 0.0f;
    // Placeholder rendering
}

bool PressurePlate::CheckCollision(const AABB& playerAABB) {
    if (m_collisionBox.Intersects(playerAABB)) {
        if (!m_triggered) {
            m_triggered = true;
            m_triggerTimer = 0.0f;
        }
        return true;
    }
    return false;
}

void PressurePlate::OnCollision(Player* player) {
    // Plate triggers trap after delay
    if (m_triggered && m_dartFired && player) {
        player->TakeDamage(15.0f);
        Audio::PlayCollisionSound();
        // Apply knockback
    }
}

void PressurePlate::FireDart() {
    // Spawn dart projectile or apply damage directly
}

void PressurePlate::TriggerTrapDoor() {
    // Open trap door or create pit
}

