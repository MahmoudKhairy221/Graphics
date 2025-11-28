#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <glm/glm.hpp>
#include "Collision.h"

class Player;
class Camera;
class Scene;

enum class ObstacleType {
    LASER_GRID,
    MOTION_DETECTOR,
    SECURITY_CAMERA,
    PRESSURE_PLATE,
    MOVING_PLATFORM,
    GUARDIAN_STATUE
};

class Obstacle {
public:
    Obstacle(ObstacleType type, const glm::vec3& position);
    virtual ~Obstacle() = default;
    
    virtual void Update(float deltaTime) = 0;
    virtual void Render(Camera* camera) = 0;
    virtual bool CheckCollision(const AABB& playerAABB) = 0;
    virtual void OnCollision(Player* player) = 0;
    
    ObstacleType GetType() const { return m_type; }
    glm::vec3 GetPosition() const { return m_position; }
    
protected:
    ObstacleType m_type;
    glm::vec3 m_position;
    AABB m_collisionBox;
};

#endif // OBSTACLE_H

