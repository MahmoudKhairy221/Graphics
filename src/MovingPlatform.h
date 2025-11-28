#ifndef MOVINGPLATFORM_H
#define MOVINGPLATFORM_H

#include "Obstacle.h"
#include <vector>

class MovingPlatform : public Obstacle {
public:
    MovingPlatform(const glm::vec3& startPos, const glm::vec3& endPos, float speed);
    
    void Update(float deltaTime) override;
    void Render(Camera* camera) override;
    bool CheckCollision(const AABB& playerAABB) override;
    void OnCollision(Player* player) override;
    
    bool IsPlayerOnPlatform() const { return m_playerOnPlatform; }
    
private:
    glm::vec3 m_startPos;
    glm::vec3 m_endPos;
    glm::vec3 m_direction;
    float m_speed;
    float m_distance;
    float m_traveled;
    bool m_forward;
    bool m_playerOnPlatform;
    
    void UpdatePosition(float deltaTime);
};

#endif // MOVINGPLATFORM_H

