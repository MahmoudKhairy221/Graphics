#include "Console.h"
#include "Camera.h"
#include "Portal.h"
#include "Audio.h"
#include <cmath>

Console::Console(const glm::vec3& position)
    : m_position(position)
    , m_activated(false)
    , m_portal(nullptr)
    , m_uiAnimationTime(0.0f)
    , m_uiAnimating(false)
{
}

void Console::Update(float deltaTime) {
    if (m_uiAnimating) {
        m_uiAnimationTime += deltaTime;
        if (m_uiAnimationTime > 1.0f) {
            m_uiAnimationTime = 0.0f;
            m_uiAnimating = false;
        }
    }
}

void Console::Render(Camera* camera) {
    if (!camera) return;
    
    // Render console
    // Placeholder rendering
    
    if (m_uiAnimating) {
        RenderUI();
    }
}

void Console::Activate() {
    if (!m_activated) {
        m_activated = true;
        m_uiAnimating = true;
        m_uiAnimationTime = 0.0f;
        
        if (m_portal) {
            m_portal->Unlock();
        }
        
        Audio::PlayConsoleSound();
    }
}

bool Console::CheckInteraction(const glm::vec3& playerPos) const {
    float distance = glm::length(playerPos - m_position);
    return distance < 2.0f;
}

void Console::RenderUI() {
    // Render UI activation animation
    // Placeholder
}

