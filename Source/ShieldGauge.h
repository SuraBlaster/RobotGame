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

    void Render(ID3D11DeviceContext* dc);

private:
    std::unique_ptr<Sprite> shieldUISpr = nullptr;
    std::unique_ptr<Sprite> shieldUIBackSpr = nullptr;
    

    SpriteData shieldUISD;
    SpriteData shieldUIBackSD;

    float ShieldDelay = 0;
    //float ShieldBack = 0;
    //float ShieldTimer = 0;
    bool hit = false;

    float maxShield = 100.0f; // 最大シールド量
    float shield = 100.0f;    // 現在のシールド量
    bool isShieldAnimating = false;
    int shieldUITimer = 0;

    bool prevShieldHit = false;
};
