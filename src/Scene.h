#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>
#include <string>
#include "Collectible.h"
#include "Obstacle.h"
#include "Portal.h"
#include "Console.h"

class Player;
class Camera;
class Lighting;
class Game;

class Scene {
public:
    Scene(Player* player, Lighting* lighting, Game* game);
    virtual ~Scene();
    
    virtual void Update(float deltaTime) = 0;
    virtual void Render(Camera* camera) = 0;
    virtual void HandleInteraction(const glm::vec3& playerPos) = 0;
    
    bool IsWinConditionMet() const { return m_winConditionMet; }
    bool IsAlarmActive() const { return m_alarmActive; }
    int GetCollectedCount() const { return m_collectedCount; }
    int GetTotalCollectibles() const { return m_totalCollectibles; }
    std::string GetObjectiveHint() const { return m_objectiveHint; }
    
    void TriggerAlarm() { m_alarmActive = true; }
    void ClearAlarm() { m_alarmActive = false; }
    
protected:
    Player* m_player;
    Lighting* m_lighting;
    Game* m_game;
    
    std::vector<std::unique_ptr<Collectible>> m_collectibles;
    std::vector<std::unique_ptr<Obstacle>> m_obstacles;
    std::unique_ptr<Portal> m_portal;
    std::unique_ptr<Console> m_console;
    
    bool m_winConditionMet;
    bool m_alarmActive;
    int m_collectedCount;
    int m_totalCollectibles;
    std::string m_objectiveHint;
    
    void CheckCollectibleCollisions();
    void CheckObstacleCollisions();
    void UpdateCollectibles(float deltaTime);
    void UpdateObstacles(float deltaTime);
    void RenderCollectibles(Camera* camera);
    void RenderObstacles(Camera* camera);
};

#endif // SCENE_H

