#include "NeoTokyoScene.h"
#include "Game.h"
#include "Player.h"
#include "Camera.h"
#include "Lighting.h"
#include "Collectible.h"
#include "LaserGrid.h"
#include "MotionDetector.h"
#include "SecurityCamera.h"
#include "Console.h"
#include "Portal.h"
#include <glm/glm.hpp>
#include <vector>

NeoTokyoScene::NeoTokyoScene(Player* player, Lighting* lighting, Game* game)
    : Scene(player, lighting, game)
{
    m_totalCollectibles = 6;
    m_objectiveHint = "Activate Control Console to unlock Portal";
    
    InitializeScene();
}

NeoTokyoScene::~NeoTokyoScene() {
}

void NeoTokyoScene::InitializeScene() {
    // Create 6 Temporal Crystals
    m_collectibles.push_back(std::make_unique<Collectible>(glm::vec3(-5.0f, 1.5f, -3.0f), CollectibleType::TEMPORAL_CRYSTAL));
    m_collectibles.push_back(std::make_unique<Collectible>(glm::vec3(5.0f, 1.5f, -3.0f), CollectibleType::TEMPORAL_CRYSTAL));
    m_collectibles.push_back(std::make_unique<Collectible>(glm::vec3(-3.0f, 1.5f, 3.0f), CollectibleType::TEMPORAL_CRYSTAL));
    m_collectibles.push_back(std::make_unique<Collectible>(glm::vec3(3.0f, 1.5f, 3.0f), CollectibleType::TEMPORAL_CRYSTAL));
    m_collectibles.push_back(std::make_unique<Collectible>(glm::vec3(0.0f, 1.5f, -6.0f), CollectibleType::TEMPORAL_CRYSTAL));
    m_collectibles.push_back(std::make_unique<Collectible>(glm::vec3(0.0f, 1.5f, 6.0f), CollectibleType::TEMPORAL_CRYSTAL));
    
    // Create Laser Grid
    m_obstacles.push_back(std::make_unique<LaserGrid>(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(4.0f, 3.0f, 4.0f), 4, 4));
    
    // Create Motion Detector
    auto motionDetector = std::make_unique<MotionDetector>(glm::vec3(-2.0f, 1.0f, -2.0f), 3.0f);
    motionDetector->SetScene(this);
    m_obstacles.push_back(std::move(motionDetector));
    
    // Create Security Camera
    auto securityCamera = std::make_unique<SecurityCamera>(glm::vec3(4.0f, 3.0f, 4.0f), 30.0f, 60.0f, 8.0f);
    securityCamera->SetScene(this);
    m_obstacles.push_back(std::move(securityCamera));
    
    // Create Control Console
    m_console = std::make_unique<Console>(glm::vec3(0.0f, 1.0f, -8.0f));
    
    // Create Exit Portal
    m_portal = std::make_unique<Portal>(glm::vec3(0.0f, 1.0f, 10.0f), true);
    m_console->SetPortal(m_portal.get());
    
    // Setup Sentinel Light path
    if (m_lighting) {
        std::vector<glm::vec3> path = {
            glm::vec3(-5.0f, 3.0f, -5.0f),
            glm::vec3(5.0f, 3.0f, -5.0f),
            glm::vec3(5.0f, 3.0f, 5.0f),
            glm::vec3(-5.0f, 3.0f, 5.0f)
        };
        m_lighting->SetSentinelLightPath(path, 2.0f, glm::vec3(0.6f, 0.8f, 1.0f), 1.2f);
    }
}

void NeoTokyoScene::Update(float deltaTime) {
    // Update collectibles
    UpdateCollectibles(deltaTime);
    
    // Update obstacles
    UpdateObstacles(deltaTime);
    
    // Check collisions
    CheckCollectibleCollisions();
    CheckObstacleCollisions();
    
    // Update console
    if (m_console) {
        m_console->Update(deltaTime);
    }
    
    // Update portal
    if (m_portal) {
        m_portal->Update(deltaTime);
        CheckPortalReach();
    }
}

void NeoTokyoScene::Render(Camera* camera) {
    if (!camera) return;
    
    // Render environment (walls, floor, etc.)
    // Placeholder rendering
    
    // Render collectibles
    RenderCollectibles(camera);
    
    // Render obstacles
    RenderObstacles(camera);
    
    // Render console
    if (m_console) {
        m_console->Render(camera);
    }
    
    // Render portal
    if (m_portal) {
        m_portal->Render(camera);
    }
}

void NeoTokyoScene::HandleInteraction(const glm::vec3& playerPos) {
    // Check console interaction
    if (m_console && m_console->CheckInteraction(playerPos)) {
        m_console->Activate();
        m_portal->SetGateConditionMet(true);
    }
}

void NeoTokyoScene::CheckPortalReach() {
    if (m_portal && m_player && m_portal->CheckPlayerReach(m_player->GetPosition())) {
        m_winConditionMet = true;
    }
}

