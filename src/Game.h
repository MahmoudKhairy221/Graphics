#ifndef GAME_H
#define GAME_H

#include <GLFW/glfw3.h>
#include <memory>
#include "Scene.h"
#include "NeoTokyoScene.h"
#include "TempleScene.h"
#include "Player.h"
#include "Camera.h"
#include "HUD.h"
#include "Lighting.h"

enum class GameState {
    MENU,
    PLAYING,
    WIN,
    LOSE
};

enum class SceneType {
    NEO_TOKYO,
    TEMPLE
};

class Game {
public:
    Game(GLFWwindow* window);
    ~Game();

    void Update(float deltaTime);
    void Render();
    void HandleInput(GLFWwindow* window, float deltaTime);
    
    void SetState(GameState state) { m_state = state; }
    GameState GetState() const { return m_state; }
    
    void SwitchScene(SceneType sceneType);
    Scene* GetCurrentScene() { return m_currentScene.get(); }
    Player* GetPlayer() { return m_player.get(); }
    Camera* GetCamera() { return m_camera.get(); }
    Lighting* GetLighting() { return m_lighting.get(); }
    
    int GetScore() const { return m_score; }
    void AddScore(int points) { m_score += points; }
    void ResetScore() { m_score = 0; }

private:
    GLFWwindow* m_window;
    GameState m_state;
    SceneType m_currentSceneType;
    
    std::unique_ptr<Scene> m_currentScene;
    std::unique_ptr<Player> m_player;
    std::unique_ptr<Camera> m_camera;
    std::unique_ptr<Lighting> m_lighting;
    std::unique_ptr<HUD> m_hud;
    
    int m_score;
    bool m_alarmActive;
    
    void InitializeScenes();
    void CheckWinCondition();
    void CheckLoseCondition();
};

#endif // GAME_H

