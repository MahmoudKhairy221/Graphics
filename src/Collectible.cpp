#include "Collectible.h"
#include "Camera.h"
#include "Audio.h"
#include <cmath>

Collectible::Collectible(const glm::vec3& position, CollectibleType type)
    : m_position(position)
    , m_type(type)
    , m_collected(false)
    , m_rotationAngle(0.0f)
    , m_bobOffset(0.0f)
    , m_animationTime(0.0f)
{
}

void Collectible::Update(float deltaTime) {
    if (m_collected) return;
    
    UpdateAnimation(deltaTime);
}

void Collectible::UpdateAnimation(float deltaTime) {
    m_animationTime += deltaTime;
    m_rotationAngle += 90.0f * deltaTime; // Rotate 90 degrees per second
    if (m_rotationAngle > 360.0f) m_rotationAngle -= 360.0f;
    
    m_bobOffset = std::sin(m_animationTime * 2.0f) * 0.2f; // Bob up and down
}

void Collectible::Render(Camera* camera) {
    if (m_collected || !camera) return;
    
    RenderMesh();
}

void Collectible::Collect() {
    m_collected = true;
    Audio::PlayPickupSound();
}

void Collectible::RenderMesh() {
    // This will be implemented with actual mesh rendering using modern OpenGL
    // For now, this is a placeholder - actual rendering will use VAO/VBO and shaders
    // The emissive shader will be used for rendering collectibles
}

