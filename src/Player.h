#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Player {
public:
    Player(const glm::vec3& position);
    
    void Update(float deltaTime);
    void Move(const glm::vec3& direction, float deltaTime);
    void Jump();
    
    glm::vec3 GetPosition() const { return m_position; }
    void SetPosition(const glm::vec3& pos) { m_position = pos; }
    
    float GetHealth() const { return m_health; }
    void SetHealth(float health) { m_health = health; }
    void TakeDamage(float damage);
    
    glm::vec3 GetVelocity() const { return m_velocity; }
    float GetSpeed() const { return m_speed; }
    
    // Collision bounds
    glm::vec3 GetAABBMin() const;
    glm::vec3 GetAABBMax() const;
    float GetRadius() const { return 0.5f; } // For sphere collision
    
    bool IsOnGround() const { return m_onGround; }
    
private:
    glm::vec3 m_position;
    glm::vec3 m_velocity;
    float m_speed;
    float m_jumpForce;
    float m_health;
    float m_maxHealth;
    
    bool m_onGround;
    float m_gravity;
    
    void ApplyGravity(float deltaTime);
    void ApplyFriction(float deltaTime);
};

#endif // PLAYER_H

