#ifndef TEMPLESCENE_H
#define TEMPLESCENE_H

#include "Scene.h"

class TempleScene : public Scene {
public:
    TempleScene(Player* player, Lighting* lighting, Game* game);
    ~TempleScene() override;
    
    void Update(float deltaTime) override;
    void Render(Camera* camera) override;
    void HandleInteraction(const glm::vec3& playerPos) override;
    
private:
    void InitializeScene();
    void CheckPortalReach();
    void UpdatePortalGateCondition();
};

#endif // TEMPLESCENE_H

