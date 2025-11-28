#ifndef COLLECTIBLE_H
#define COLLECTIBLE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera;

enum class CollectibleType {
    TEMPORAL_CRYSTAL,
    GOLDEN_SCARAB
};

class Collectible {
public:
    Collectible(const glm::vec3& position, CollectibleType type);
    
    void Update(float deltaTime);
    void Render(Camera* camera);
    void Collect();
    
    bool IsCollected() const { return m_collected; }
    glm::vec3 GetPosition() const { return m_position; }
    float GetRadius() const { return 0.3f; }
    int GetPoints() const { return 500; }
    CollectibleType GetType() const { return m_type; }
    
private:
    glm::vec3 m_position;
    CollectibleType m_type;
    bool m_collected;
    
    // Animation
    float m_rotationAngle;
    float m_bobOffset;
    float m_animationTime;
    
    void UpdateAnimation(float deltaTime);
    void RenderMesh();
};

#endif // COLLECTIBLE_H

