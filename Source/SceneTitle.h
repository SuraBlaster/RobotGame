#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
#include <memory>

class SceneTitle : public Scene
{
public:
    SceneTitle() {}
    ~SceneTitle()override{}

    //初期化
    void Initialize()override;

    //終了化
    void Finalize()override;

    //更新処理
    void Update(float elapsedTime)override;

    //描画処理
    void Render()override;

private:
    std::unique_ptr<Sprite> startSpr = nullptr;
    std::unique_ptr<Sprite> gameCloseSpr = nullptr;

    Sprite* sprite = nullptr;
    SpriteData startSD;
    SpriteData gameCloseSD;
};
