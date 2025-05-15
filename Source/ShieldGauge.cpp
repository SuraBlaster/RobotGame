#include "ShieldGauge.h"
#include "Graphics/Graphics.h"
#include "Player.h"

void ShieldGauge::Initialize()
{
    sprite[0] = std::make_unique<Sprite>("Data/Sprite/ShieldGauge.png");
    sprite[1] = std::make_unique<Sprite>("Data/Sprite/ShieldBack.png");
}

void ShieldGauge::Update(float elapsedTime)
{
    ShieldTimer = Player::Instance().GetTimer() * 2;
    hit = Player::Instance().GetHit();

    if (hit)
    {
        ShieldDelay = 0.5f;
        Player::Instance().SetHit(false);
    }

    if (ShieldDelay > 0.0f)
    {
        ShieldDelay -= elapsedTime;
    }
    else if (ShieldDelay <= 0.0f && ShieldBack > ShieldTimer)
    {
        ShieldBack -= elapsedTime * 20;
    }
    else if (ShieldDelay <= 0.0f && ShieldTimer > 0)
    {
        ShieldBack = ShieldTimer;
    }
    else
    {
        ShieldBack = 0;
    }
    
    

}

void ShieldGauge::Render()
{
    
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();

    float textureWidth = static_cast<float>(sprite[0]->GetTextureWidth());
    float textureHeight = static_cast<float>(sprite[0]->GetTextureHeight());

    sprite[1]->Render(dc,
        10, 100,
        ShieldBack * 10,
        25,
        0, 0,
        textureWidth - ShieldBack,
        textureHeight,
        0,
        1, 1, 1, 1
    );

    sprite[0]->Render(dc,
        10,100,
        ShieldTimer * 10,
        25,
        0,0,
        textureWidth - ShieldTimer,
        textureHeight,
        0,
        1,1,1,1
    );


}
