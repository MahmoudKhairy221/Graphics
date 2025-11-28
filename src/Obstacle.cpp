#include "Obstacle.h"

Obstacle::Obstacle(ObstacleType type, const glm::vec3& position)
    : m_type(type)
    , m_position(position)
{
}

