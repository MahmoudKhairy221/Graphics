#ifndef LASERGRID_H
#define LASERGRID_H

#include "Obstacle.h"
#include <vector>

class LaserGrid : public Obstacle {
public:
    LaserGrid(const glm::vec3& position, const glm::vec3& size, int segmentsX, int segmentsZ);
    
    void Update(float deltaTime) override;
    void Render(Camera* camera) override;
    bool CheckCollision(const AABB& playerAABB) override;
    void OnCollision(Player* player) override;
    
private:
    glm::vec3 m_size;
    int m_segmentsX;
    int m_segmentsZ;
    std::vector<std::vector<bool>> m_segmentStates; // true = active
    float m_toggleTimer;
    float m_toggleInterval;
    float m_damage;
    
    void ToggleSegments();
    void RenderLaserBeam(const glm::vec3& start, const glm::vec3& end);
};

#endif // LASERGRID_H

