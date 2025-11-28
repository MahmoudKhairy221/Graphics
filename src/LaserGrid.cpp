#include "LaserGrid.h"
#include "Player.h"
#include "Camera.h"
#include "Collision.h"
#include "Audio.h"
#include <cmath>
#include <cstdlib>

LaserGrid::LaserGrid(const glm::vec3& position, const glm::vec3& size, int segmentsX, int segmentsZ)
    : Obstacle(ObstacleType::LASER_GRID, position)
    , m_size(size)
    , m_segmentsX(segmentsX)
    , m_segmentsZ(segmentsZ)
    , m_toggleTimer(0.0f)
    , m_toggleInterval(2.0f)
    , m_damage(10.0f)
{
    // Initialize segment states randomly
    m_segmentStates.resize(m_segmentsX);
    for (int i = 0; i < m_segmentsX; i++) {
        m_segmentStates[i].resize(m_segmentsZ);
        for (int j = 0; j < m_segmentsZ; j++) {
            m_segmentStates[i][j] = (rand() % 2 == 0);
        }
    }
    
    // Set collision box
    m_collisionBox = AABB(
        position - size * 0.5f,
        position + size * 0.5f
    );
}

void LaserGrid::Update(float deltaTime) {
    m_toggleTimer += deltaTime;
    if (m_toggleTimer >= m_toggleInterval) {
        ToggleSegments();
        m_toggleTimer = 0.0f;
    }
}

void LaserGrid::ToggleSegments() {
    // Toggle random segments to create timing challenges
    for (int i = 0; i < m_segmentsX; i++) {
        for (int j = 0; j < m_segmentsZ; j++) {
            if (rand() % 3 == 0) { // 1/3 chance to toggle
                m_segmentStates[i][j] = !m_segmentStates[i][j];
            }
        }
    }
}

void LaserGrid::Render(Camera* camera) {
    if (!camera) return;
    
    float segmentSizeX = m_size.x / m_segmentsX;
    float segmentSizeZ = m_size.z / m_segmentsZ;
    
    // Render active laser beams
    for (int i = 0; i < m_segmentsX; i++) {
        for (int j = 0; j < m_segmentsZ; j++) {
            if (m_segmentStates[i][j]) {
                glm::vec3 segmentPos = m_position + glm::vec3(
                    (i - m_segmentsX / 2.0f) * segmentSizeX,
                    0.0f,
                    (j - m_segmentsZ / 2.0f) * segmentSizeZ
                );
                
                glm::vec3 start = segmentPos + glm::vec3(0.0f, 0.0f, 0.0f);
                glm::vec3 end = segmentPos + glm::vec3(0.0f, m_size.y, 0.0f);
                
                RenderLaserBeam(start, end);
            }
        }
    }
}

void LaserGrid::RenderLaserBeam(const glm::vec3& start, const glm::vec3& end) {
    // Render laser beam as a line with emissive red color
    // This will use modern OpenGL rendering
    // Placeholder: actual rendering will use shaders
}

bool LaserGrid::CheckCollision(const AABB& playerAABB) {
    // Check if player intersects with any active laser segment
    if (!m_collisionBox.Intersects(playerAABB)) {
        return false;
    }
    
    // Check individual segments
    float segmentSizeX = m_size.x / m_segmentsX;
    float segmentSizeZ = m_size.z / m_segmentsZ;
    
    glm::vec3 playerCenter = (playerAABB.min + playerAABB.max) * 0.5f;
    glm::vec3 localPos = playerCenter - m_position;
    
    int segmentX = (int)((localPos.x + m_size.x * 0.5f) / segmentSizeX);
    int segmentZ = (int)((localPos.z + m_size.z * 0.5f) / segmentSizeZ);
    
    if (segmentX >= 0 && segmentX < m_segmentsX && 
        segmentZ >= 0 && segmentZ < m_segmentsZ) {
        return m_segmentStates[segmentX][segmentZ];
    }
    
    return false;
}

void LaserGrid::OnCollision(Player* player) {
    if (player) {
        player->TakeDamage(m_damage);
        Audio::PlayCollisionSound();
    }
}

