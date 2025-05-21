#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
#include <thread>

#include"Audio/Audio.h"
#include"Audio/AudioSource.h"
class SceneLoading : public Scene
{
public:
    SceneLoading(Scene* nextScene) : nextScene(nextScene){}
    ~SceneLoading()override{}

    void Initialize() override;

    void Finalize() override;

    void Update(float elapsedTime)override;

    void Render()override;

    static void LoadingThread(SceneLoading* scene);
private:
    Sprite* load = nullptr;
    Sprite* dot = nullptr;
    Scene* nextScene = nullptr;
    std::thread* thread = nullptr;
    //BGM
    std::unique_ptr<AudioSource> LoadingBGM;
};
