#ifndef PRESSUREPLATE_H
#define PRESSUREPLATE_H

#include "Obstacle.h"

class PressurePlate : public Obstacle {
public:
    PressurePlate(const glm::vec3& position, float triggerDelay);
    
    void Update(float deltaTime) override;
    void Render(Camera* camera) override;
    bool CheckCollision(const AABB& playerAABB) override;
    void OnCollision(Player* player) override;
    
    bool IsTriggered() const { return m_triggered; }
    
private:
    float m_triggerDelay;
    bool m_triggered;
    float m_triggerTimer;
    float m_animationTime;
    bool m_dartFired;
    
    void FireDart();
    void TriggerTrapDoor();
};

#endif // PRESSUREPLATE_H

