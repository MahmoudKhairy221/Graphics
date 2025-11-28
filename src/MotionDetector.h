#ifndef MOTIONDETECTOR_H
#define MOTIONDETECTOR_H

#include "Obstacle.h"

class Scene;

class MotionDetector : public Obstacle {
public:
    MotionDetector(const glm::vec3& position, float radius);
    
    void Update(float deltaTime) override;
    void Render(Camera* camera) override;
    bool CheckCollision(const AABB& playerAABB) override;
    void OnCollision(Player* player) override;
    
    bool IsTriggered() const { return m_triggered; }
    void SetScene(Scene* scene) { m_scene = scene; }
    
private:
    float m_radius;
    bool m_triggered;
    float m_triggerCooldown;
    Scene* m_scene;
    
    void TriggerAlarm();
};

#endif // MOTIONDETECTOR_H

