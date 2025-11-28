#ifndef HUD_H
#define HUD_H

#include <string>

class HUD {
public:
    HUD();
    ~HUD();
    
    void Render(int score, float health, int collectedCount, int totalCollectibles, 
                bool alarmActive, const std::string& objectiveHint);
    
private:
    void RenderScore(int score);
    void RenderHealth(float health);
    void RenderCollectibles(int collected, int total);
    void RenderObjective(const std::string& hint);
    void RenderAlarmOverlay(bool active);
    
    unsigned int m_fontTexture;
    void InitializeFont();
    void RenderText(const std::string& text, float x, float y, float scale, const float* color);
};

#endif // HUD_H

