#include "Scene.h"
#include "Player.h"
#include "Camera.h"
#include "Collision.h"
#include "Collectible.h"
#include "Obstacle.h"
#include <algorithm>

Scene::Scene(Player* player, Lighting* lighting, Game* game)
    : m_player(player)
    , m_lighting(lighting)
    , m_game(game)
    , m_winConditionMet(false)
    , m_alarmActive(false)
    , m_collectedCount(0)
    , m_totalCollectibles(0)
{
}

Scene::~Scene() {
}

void Scene::CheckCollectibleCollisions() {
    if (!m_player) return;
    
    Sphere playerSphere(m_player->GetPosition(), m_player->GetRadius());
    
    for (auto it = m_collectibles.begin(); it != m_collectibles.end();) {
        if ((*it)->IsCollected()) {
            ++it;
            continue;
        }
        
        Sphere collectibleSphere((*it)->GetPosition(), (*it)->GetRadius());
        
        if (Collision::CheckSphereSphere(playerSphere, collectibleSphere)) {
            (*it)->Collect();
            m_collectedCount++;
            if (m_game) {
                m_game->AddScore((*it)->GetPoints());
            }
            it = m_collectibles.erase(it);
        } else {
            ++it;
        }
    }
}

void Scene::CheckObstacleCollisions() {
    if (!m_player) return;
    
    AABB playerAABB(m_player->GetAABBMin(), m_player->GetAABBMax());
    
    for (auto& obstacle : m_obstacles) {
        if (obstacle->CheckCollision(playerAABB)) {
            obstacle->OnCollision(m_player);
        }
    }
}

void Scene::UpdateCollectibles(float deltaTime) {
    for (auto& collectible : m_collectibles) {
        collectible->Update(deltaTime);
    }
}

void Scene::UpdateObstacles(float deltaTime) {
    for (auto& obstacle : m_obstacles) {
        obstacle->Update(deltaTime);
    }
}

void Scene::RenderCollectibles(Camera* camera) {
    for (auto& collectible : m_collectibles) {
        if (!collectible->IsCollected()) {
            collectible->Render(camera);
        }
    }
}

void Scene::RenderObstacles(Camera* camera) {
    for (auto& obstacle : m_obstacles) {
        obstacle->Render(camera);
    }
}

