#ifndef SECURITYCAMERA_H
#define SECURITYCAMERA_H

#include "Obstacle.h"

class Scene;

class SecurityCamera : public Obstacle {
public:
    SecurityCamera(const glm::vec3& position, float rotationSpeed, float fov, float range);
    
    void Update(float deltaTime) override;
    void Render(Camera* camera) override;
    bool CheckCollision(const AABB& playerAABB) override;
    void OnCollision(Player* player) override;
    
    bool IsDetectingPlayer() const { return m_detectingPlayer; }
    void SetScene(Scene* scene) { m_scene = scene; }
    
private:
    float m_rotationAngle;
    float m_rotationSpeed;
    float m_fov; // Field of view in degrees
    float m_range;
    bool m_detectingPlayer;
    Scene* m_scene;
    
    glm::vec3 GetLookDirection() const;
    bool IsPlayerInFrustum(const glm::vec3& playerPos) const;
};

#endif // SECURITYCAMERA_H

