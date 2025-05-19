#pragma once
#include "Graphics/Sprite.h"
#include <memory>

class FadeOut
{
public:
    FadeOut() {};
    ~FadeOut() {};

    void Initialize();

    void Update(float elapsedTime);

    void Render();

private:
    std::unique_ptr<Sprite> sprite;

    float transparency = 0.0f;//“§–¾“x
};
