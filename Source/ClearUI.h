#pragma once
#include "Graphics/Sprite.h"
#include <memory>

class ClearUI
{
public:
    ClearUI() {};
    ~ClearUI() {};

    void Initialize();

    void Update(float elapsedTime);

    void Render();

private:
    std::unique_ptr<Sprite> GameClearBack;
    std::unique_ptr<Sprite> GameClear;
    std::unique_ptr<Sprite> ReturnSelect;
    std::unique_ptr<Sprite> ReturnTitle;

    SpriteData GameClearBackSD{};
    SpriteData GameClearSD{};
    SpriteData ReturnSelectSD{};
    SpriteData ReturnTitleSD{};
    
    float transparency = 0.0f;
};
