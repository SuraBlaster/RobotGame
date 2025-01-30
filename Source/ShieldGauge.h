#pragma once
#include "Graphics/Sprite.h"
#include <memory>

class ShieldGauge
{
public:
    ShieldGauge() {};
    ~ShieldGauge() {};

    void Initialize();

    void Update(float elapsedTime);

    void Render();

private:
    std::unique_ptr<Sprite> sprite[2];
  
    float ShieldDelay = 0;
    float ShieldBack = 0;
    float ShieldTimer = 0;
    bool hit = false;
};
