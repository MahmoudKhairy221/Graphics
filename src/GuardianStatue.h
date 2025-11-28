#ifndef GUARDIANSTATUE_H
#define GUARDIANSTATUE_H

#include "Obstacle.h"
#include <vector>

class GuardianStatue : public Obstacle {
public:
    GuardianStatue(const glm::vec3& startPos, const std::vector<glm::vec3>& patrolPath, float speed);
    
    void Update(float deltaTime) override;
    void Render(Camera* camera) override;
    bool CheckCollision(const AABB& playerAABB) override;
    void OnCollision(Player* player) override;
    
private:
    std::vector<glm::vec3> m_patrolPath;
    int m_currentPathIndex;
    float m_speed;
    float m_knockbackForce;
    float m_damage;
    
    void UpdatePatrol(float deltaTime);
    glm::vec3 GetNextPatrolPoint() const;
};

#endif // GUARDIANSTATUE_H

