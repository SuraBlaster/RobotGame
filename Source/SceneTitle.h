#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
#include <memory>

class SceneTitle : public Scene
{
public:
    SceneTitle() {}
    ~SceneTitle()override{}

    //‰Šú‰»
    void Initialize()override;

    //I—¹‰»
    void Finalize()override;

    //XVˆ—
    void Update(float elapsedTime)override;

    //•`‰æˆ—
    void Render()override;

private:
    std::unique_ptr<Sprite> startSpr = nullptr;
    std::unique_ptr<Sprite> gameCloseSpr = nullptr;

    Sprite* sprite = nullptr;
    Sprite* goodBye = nullptr;
    SpriteData startSD;
    SpriteData gameCloseSD;

    bool exitFlag;
    float timer;
    float transparency;
};
