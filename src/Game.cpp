#include "Game.h"
#include <iostream>
#include <glm/glm.hpp>

Game::Game(GLFWwindow* window)
    : m_window(window)
    , m_state(GameState::MENU)
    , m_currentSceneType(SceneType::NEO_TOKYO)
    , m_score(0)
    , m_alarmActive(false)
{
    // Initialize player
    m_player = std::make_unique<Player>(glm::vec3(0.0f, 1.0f, 0.0f));
    
    // Initialize camera
    m_camera = std::make_unique<Camera>(m_player.get());
    
    // Initialize lighting
    m_lighting = std::make_unique<Lighting>();
    
    // Initialize HUD
    m_hud = std::make_unique<HUD>();
    
    // Initialize scenes
    InitializeScenes();
    
    // Start with Neo-Tokyo scene
    SwitchScene(SceneType::NEO_TOKYO);
}

Game::~Game() {
}

void Game::InitializeScenes() {
    // Scenes will be created on demand in SwitchScene
}

void Game::SwitchScene(SceneType sceneType) {
    m_currentSceneType = sceneType;
    
    if (sceneType == SceneType::NEO_TOKYO) {
        m_currentScene = std::make_unique<NeoTokyoScene>(m_player.get(), m_lighting.get(), this);
        m_player->SetPosition(glm::vec3(0.0f, 1.0f, 5.0f));
    } else {
        m_currentScene = std::make_unique<TempleScene>(m_player.get(), m_lighting.get(), this);
        m_player->SetPosition(glm::vec3(0.0f, 1.0f, 5.0f));
    }
    
    m_camera->Reset();
    m_score = 0;
    m_alarmActive = false;
}

void Game::Update(float deltaTime) {
    if (m_state != GameState::PLAYING) {
        return;
    }
    
    // Update lighting
    m_lighting->Update(deltaTime);
    
    // Update current scene
    if (m_currentScene) {
        m_currentScene->Update(deltaTime);
    }
    
    // Update player
    m_player->Update(deltaTime);
    
    // Update camera
    m_camera->Update(deltaTime);
    
    // Check win/lose conditions
    CheckWinCondition();
    CheckLoseCondition();
    
    // Update alarm state from scene
    if (m_currentScene) {
        m_alarmActive = m_currentScene->IsAlarmActive();
    }
}

void Game::Render() {
    if (m_state == GameState::MENU) {
        // Render menu (simple text for now)
        return;
    }
    
    if (m_state == GameState::WIN || m_state == GameState::LOSE) {
        // Render win/lose screen
        return;
    }
    
    // Render scene
    if (m_currentScene) {
        m_currentScene->Render(m_camera.get());
    }
    
    // Render HUD
    if (m_hud) {
        m_hud->Render(m_score, m_player->GetHealth(), 
                     m_currentScene ? m_currentScene->GetCollectedCount() : 0,
                     m_currentScene ? m_currentScene->GetTotalCollectibles() : 0,
                     m_alarmActive,
                     m_currentScene ? m_currentScene->GetObjectiveHint() : "");
    }
}

void Game::HandleInput(GLFWwindow* window, float deltaTime) {
    if (m_state != GameState::PLAYING) {
        // Handle menu/win/lose input
        if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
            if (m_state == GameState::MENU) {
                m_state = GameState::PLAYING;
            } else {
                // Restart game
                SwitchScene(SceneType::NEO_TOKYO);
                m_state = GameState::PLAYING;
            }
        }
        return;
    }
    
    // Player movement
    glm::vec3 moveDir(0.0f);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        moveDir += m_camera->GetForward();
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        moveDir -= m_camera->GetForward();
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        moveDir -= m_camera->GetRight();
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        moveDir += m_camera->GetRight();
    }
    
    if (glm::length(moveDir) > 0.0f) {
        moveDir = glm::normalize(moveDir);
        m_player->Move(moveDir, deltaTime);
    }
    
    // Jump
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        m_player->Jump();
    }
    
    // Camera toggle
    static bool cKeyPressed = false;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && !cKeyPressed) {
        m_camera->ToggleMode();
        cKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE) {
        cKeyPressed = false;
    }
    
    // Interaction
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS || glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        if (m_currentScene) {
            m_currentScene->HandleInteraction(m_player->GetPosition());
        }
    }
    
    // Mouse look
    static double lastX = 0.0, lastY = 0.0;
    static bool firstMouse = true;
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    
    m_camera->ProcessMouseMovement(xoffset, yoffset);
}

void Game::CheckWinCondition() {
    if (m_currentScene && m_currentScene->IsWinConditionMet()) {
        m_state = GameState::WIN;
    }
}

void Game::CheckLoseCondition() {
    if (m_player->GetHealth() <= 0) {
        m_state = GameState::LOSE;
    }
    
    // Check if player fell into pit (y < -10)
    if (m_player->GetPosition().y < -10.0f) {
        m_state = GameState::LOSE;
    }
}

