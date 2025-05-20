#pragma once
#include "Graphics/Sprite.h"
#include <memory>

class GameOverUI
{
public:
    GameOverUI() {};
    ~GameOverUI() {};

    void Initialize();

    void Update(float elapsedTime);

    void Render();

private:
    std::unique_ptr<Sprite> GameOverBack;
    std::unique_ptr<Sprite> GameOver;
    std::unique_ptr<Sprite> Retry;
    std::unique_ptr<Sprite> ReturnTitle;

    SpriteData GameOverBackSD{};
    SpriteData GameOverSD{};
    SpriteData RetrySD{};
    SpriteData ReturnTitleSD{};

    float transparency = 0.0f;
};
