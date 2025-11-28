#ifndef CONSOLE_H
#define CONSOLE_H

#include <glm/glm.hpp>

class Camera;
class Portal;

class Console {
public:
    Console(const glm::vec3& position);
    
    void Update(float deltaTime);
    void Render(Camera* camera);
    
    bool IsActivated() const { return m_activated; }
    void Activate();
    
    bool CheckInteraction(const glm::vec3& playerPos) const;
    glm::vec3 GetPosition() const { return m_position; }
    
    void SetPortal(Portal* portal) { m_portal = portal; }
    
private:
    glm::vec3 m_position;
    bool m_activated;
    Portal* m_portal;
    
    float m_uiAnimationTime;
    bool m_uiAnimating;
    
    void RenderUI();
};

#endif // CONSOLE_H

