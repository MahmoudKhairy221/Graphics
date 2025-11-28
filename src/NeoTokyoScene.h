#ifndef NEOTOKYOSCENE_H
#define NEOTOKYOSCENE_H

#include "Scene.h"

class NeoTokyoScene : public Scene {
public:
    NeoTokyoScene(Player* player, Lighting* lighting, Game* game);
    ~NeoTokyoScene() override;
    
    void Update(float deltaTime) override;
    void Render(Camera* camera) override;
    void HandleInteraction(const glm::vec3& playerPos) override;
    
private:
    void InitializeScene();
    void CheckPortalReach();
};

#endif // NEOTOKYOSCENE_H

