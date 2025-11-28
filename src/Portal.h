#ifndef PORTAL_H
#define PORTAL_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera;
class Game;

class Portal {
public:
    Portal(const glm::vec3& position, bool requiresGateCondition = false);
    
    void Update(float deltaTime);
    void Render(Camera* camera);
    
    bool IsUnlocked() const { return m_unlocked; }
    void Unlock() { m_unlocked = true; }
    void SetGateConditionMet(bool met) { m_gateConditionMet = met; }
    bool CanActivate() const { return !m_requiresGateCondition || m_gateConditionMet; }
    
    bool CheckPlayerReach(const glm::vec3& playerPos) const;
    glm::vec3 GetPosition() const { return m_position; }
    
private:
    glm::vec3 m_position;
    bool m_unlocked;
    bool m_requiresGateCondition;
    bool m_gateConditionMet;
    
    // Animation
    float m_pulseTime;
    float m_scale;
    float m_emissiveIntensity;
    
    void UpdateAnimation(float deltaTime);
    void RenderMesh();
};

#endif // PORTAL_H

