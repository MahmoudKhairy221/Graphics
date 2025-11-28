#include "TempleScene.h"
#include "Game.h"
#include "Player.h"
#include "Camera.h"
#include "Lighting.h"
#include "Collectible.h"
#include "PressurePlate.h"
#include "MovingPlatform.h"
#include "GuardianStatue.h"
#include "Portal.h"
#include <glm/glm.hpp>
#include <vector>

TempleScene::TempleScene(Player* player, Lighting* lighting, Game* game)
    : Scene(player, lighting, game)
{
    m_totalCollectibles = 6;
    m_objectiveHint = "Collect at least 4 Golden Scarabs to energize Portal";
    
    InitializeScene();
}

TempleScene::~TempleScene() {
}

void TempleScene::InitializeScene() {
    // Create 6 Golden Scarabs
    m_collectibles.push_back(std::make_unique<Collectible>(glm::vec3(-4.0f, 1.2f, -4.0f), CollectibleType::GOLDEN_SCARAB));
    m_collectibles.push_back(std::make_unique<Collectible>(glm::vec3(4.0f, 1.2f, -4.0f), CollectibleType::GOLDEN_SCARAB));
    m_collectibles.push_back(std::make_unique<Collectible>(glm::vec3(-4.0f, 1.2f, 4.0f), CollectibleType::GOLDEN_SCARAB));
    m_collectibles.push_back(std::make_unique<Collectible>(glm::vec3(4.0f, 1.2f, 4.0f), CollectibleType::GOLDEN_SCARAB));
    m_collectibles.push_back(std::make_unique<Collectible>(glm::vec3(0.0f, 1.2f, -6.0f), CollectibleType::GOLDEN_SCARAB));
    m_collectibles.push_back(std::make_unique<Collectible>(glm::vec3(0.0f, 1.2f, 6.0f), CollectibleType::GOLDEN_SCARAB));
    
    // Create Pressure Plates
    m_obstacles.push_back(std::make_unique<PressurePlate>(glm::vec3(-2.0f, 1.0f, -2.0f), 1.5f));
    m_obstacles.push_back(std::make_unique<PressurePlate>(glm::vec3(2.0f, 1.0f, 2.0f), 1.5f));
    
    // Create Moving Platforms
    m_obstacles.push_back(std::make_unique<MovingPlatform>(
        glm::vec3(-3.0f, 1.0f, 0.0f),
        glm::vec3(3.0f, 1.0f, 0.0f),
        2.0f
    ));
    m_obstacles.push_back(std::make_unique<MovingPlatform>(
        glm::vec3(0.0f, 1.0f, -3.0f),
        glm::vec3(0.0f, 1.0f, 3.0f),
        2.0f
    ));
    
    // Create Guardian Statue with patrol path
    std::vector<glm::vec3> patrolPath = {
        glm::vec3(-2.0f, 1.0f, -2.0f),
        glm::vec3(2.0f, 1.0f, -2.0f),
        glm::vec3(2.0f, 1.0f, 2.0f),
        glm::vec3(-2.0f, 1.0f, 2.0f)
    };
    m_obstacles.push_back(std::make_unique<GuardianStatue>(glm::vec3(0.0f, 1.0f, 0.0f), patrolPath, 1.5f));
    
    // Create Exit Portal (requires ≥4 scarabs)
    m_portal = std::make_unique<Portal>(glm::vec3(0.0f, 1.0f, 10.0f), true);
    
    // Setup Sentinel Light path (drifting wisp)
    if (m_lighting) {
        std::vector<glm::vec3> path = {
            glm::vec3(-3.0f, 2.5f, -3.0f),
            glm::vec3(3.0f, 2.5f, -3.0f),
            glm::vec3(3.0f, 2.5f, 3.0f),
            glm::vec3(-3.0f, 2.5f, 3.0f),
            glm::vec3(0.0f, 3.5f, 0.0f)
        };
        m_lighting->SetSentinelLightPath(path, 1.5f, glm::vec3(1.0f, 0.9f, 0.7f), 0.8f);
    }
}

void TempleScene::Update(float deltaTime) {
    // Update collectibles
    UpdateCollectibles(deltaTime);
    
    // Update obstacles
    UpdateObstacles(deltaTime);
    
    // Check collisions
    CheckCollectibleCollisions();
    CheckObstacleCollisions();
    
    // Update portal gate condition
    UpdatePortalGateCondition();
    
    // Update portal
    if (m_portal) {
        m_portal->Update(deltaTime);
        CheckPortalReach();
    }
}

void TempleScene::Render(Camera* camera) {
    if (!camera) return;
    
    // Render environment (temple walls, columns, etc.)
    // Placeholder rendering
    
    // Render collectibles
    RenderCollectibles(camera);
    
    // Render obstacles
    RenderObstacles(camera);
    
    // Render portal
    if (m_portal) {
        m_portal->Render(camera);
    }
}

void TempleScene::HandleInteraction(const glm::vec3& playerPos) {
    // Temple scene doesn't have console, but could have other interactions
}

void TempleScene::CheckPortalReach() {
    if (m_portal && m_player && m_portal->CheckPlayerReach(m_player->GetPosition())) {
        m_winConditionMet = true;
    }
}

void TempleScene::UpdatePortalGateCondition() {
    if (m_portal) {
        // Portal requires at least 4 scarabs
        bool gateMet = m_collectedCount >= 4;
        m_portal->SetGateConditionMet(gateMet);
        
        if (gateMet && !m_portal->IsUnlocked()) {
            m_portal->Unlock();
        }
    }
}

