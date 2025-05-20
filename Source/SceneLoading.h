#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
#include <thread>

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

    int dotCount = 1;
    float timeAccumulator = 0.0f;
    const float updateInterval = 0.5f;
};
