#include "HUD.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <sstream>
#include <cmath>

HUD::HUD() : m_fontTexture(0) {
    InitializeFont();
}

HUD::~HUD() {
    if (m_fontTexture != 0) {
        glDeleteTextures(1, &m_fontTexture);
    }
}

void HUD::InitializeFont() {
    // Initialize font rendering
    // For now, placeholder - would use a font library or bitmap font
}

void HUD::Render(int score, float health, int collectedCount, int totalCollectibles, 
                 bool alarmActive, const std::string& objectiveHint) {
    // Save current state
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    // Set up orthographic projection for HUD (using modern OpenGL)
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Render HUD elements
    RenderScore(score);
    RenderHealth(health);
    RenderCollectibles(collectedCount, totalCollectibles);
    RenderObjective(objectiveHint);
    RenderAlarmOverlay(alarmActive);
    
    // Restore
    glEnable(GL_DEPTH_TEST);
}

void HUD::RenderScore(int score) {
    std::ostringstream oss;
    oss << "Score: " << score;
    float color[] = {1.0f, 1.0f, 1.0f, 1.0f};
    RenderText(oss.str(), 20.0f, 20.0f, 1.0f, color);
}

void HUD::RenderHealth(float health) {
    std::ostringstream oss;
    oss << "Health: " << (int)health;
    float color[] = {1.0f, 0.0f, 0.0f, 1.0f};
    RenderText(oss.str(), 20.0f, 60.0f, 1.0f, color);
    
    // Health bar using modern OpenGL
    float barWidth = 200.0f;
    float barHeight = 20.0f;
    float x = 20.0f;
    float y = 100.0f;
    
    // Use immediate mode for simplicity (will be replaced with proper rendering)
    // In production, use VAO/VBO for HUD rendering
    // For now, placeholder - actual rendering would use shaders and proper geometry
}

void HUD::RenderCollectibles(int collected, int total) {
    std::ostringstream oss;
    oss << "Collected: " << collected << "/" << total;
    float color[] = {1.0f, 1.0f, 0.0f, 1.0f};
    RenderText(oss.str(), 20.0f, 140.0f, 1.0f, color);
}

void HUD::RenderObjective(const std::string& hint) {
    if (!hint.empty()) {
        float color[] = {0.8f, 0.8f, 1.0f, 1.0f};
        RenderText(hint, 20.0f, 180.0f, 0.8f, color);
    }
}

void HUD::RenderAlarmOverlay(bool active) {
    if (!active) return;
    
    static float pulseTime = 0.0f;
    pulseTime += 0.02f;
    if (pulseTime > 6.28f) pulseTime = 0.0f;
    
    float alpha = (std::sin(pulseTime) + 1.0f) * 0.5f * 0.3f; // Pulsing red overlay
    
    // This would use post-processing shader in full implementation
    // For now, placeholder - actual rendering would use fullscreen quad with shader
}

void HUD::RenderText(const std::string& text, float x, float y, float scale, const float* color) {
    // Placeholder text rendering
    // In a full implementation, would use a font rendering library
    // For now, this is a placeholder that would be replaced with actual font rendering
    glColor4fv(color);
    // Actual text rendering would go here
}

