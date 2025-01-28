#pragma once
#include "Graphics/Sprite.h"
#include <memory>

class ShieldGauge
{
public:
    ShieldGauge();
    ~ShieldGauge();

    void Update();

    void Render();

private:
    std::unique_ptr<Sprite> sprite[2];
    float ShieldTimer = 0;
    float ShieldBack = 0;
};
