#include "Player.h"
#include <algorithm>

Player::Player(const glm::vec3& position)
    : m_position(position)
    , m_velocity(0.0f)
    , m_speed(5.0f)
    , m_jumpForce(8.0f)
    , m_health(100.0f)
    , m_maxHealth(100.0f)
    , m_onGround(false)
    , m_gravity(-20.0f)
{
}

void Player::Update(float deltaTime) {
    ApplyGravity(deltaTime);
    ApplyFriction(deltaTime);
    
    // Update position
    m_position += m_velocity * deltaTime;
    
    // Keep player above ground (basic ground collision)
    if (m_position.y < 1.0f) {
        m_position.y = 1.0f;
        m_velocity.y = 0.0f;
        m_onGround = true;
    } else {
        m_onGround = false;
    }
}

void Player::Move(const glm::vec3& direction, float deltaTime) {
    glm::vec3 moveDir = direction;
    moveDir.y = 0.0f; // Don't move vertically with WASD
    moveDir = glm::normalize(moveDir);
    
    m_velocity.x = moveDir.x * m_speed;
    m_velocity.z = moveDir.z * m_speed;
}

void Player::Jump() {
    if (m_onGround) {
        m_velocity.y = m_jumpForce;
        m_onGround = false;
    }
}

void Player::TakeDamage(float damage) {
    m_health = std::max(0.0f, m_health - damage);
}

glm::vec3 Player::GetAABBMin() const {
    return m_position - glm::vec3(0.5f, 1.0f, 0.5f);
}

glm::vec3 Player::GetAABBMax() const {
    return m_position + glm::vec3(0.5f, 0.0f, 0.5f);
}

void Player::ApplyGravity(float deltaTime) {
    if (!m_onGround) {
        m_velocity.y += m_gravity * deltaTime;
    }
}

void Player::ApplyFriction(float deltaTime) {
    // Apply friction to horizontal velocity
    float friction = 0.9f;
    m_velocity.x *= friction;
    m_velocity.z *= friction;
    
    // Stop very small velocities
    if (std::abs(m_velocity.x) < 0.1f) m_velocity.x = 0.0f;
    if (std::abs(m_velocity.z) < 0.1f) m_velocity.z = 0.0f;
}

