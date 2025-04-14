#pragma once
#include "Graphics/Sprite.h"
#include <memory>

class ShieldIcon
{
public:
    ShieldIcon() {};
    ~ShieldIcon() {};

    void Initialize();

    void Update(float elapsedTime);

    void Render();

private:
    std::unique_ptr<Sprite> sprite;

    int ShieldCount = 0;
};
