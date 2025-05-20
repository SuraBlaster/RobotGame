#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
#include <memory>

#include"Audio/Audio.h"
#include"Audio/AudioSource.h"
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
    SpriteData startSD;
    SpriteData gameCloseSD;

    //BGM
    std::unique_ptr<AudioSource> TitleBGM;
};
