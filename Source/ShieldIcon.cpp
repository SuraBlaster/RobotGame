#include "ShieldIcon.h"
#include "Graphics/Graphics.h"
#include "Player.h"

void ShieldIcon::Initialize()
{
    sprite = std::make_unique<Sprite>("Data/Sprite/ShieldCount.png");
    
}

void ShieldIcon::Update(float elapsedTime)
{
    
}

void ShieldIcon::Render()
{

    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();

    ShieldCount = Player::Instance().GetShieldCount();

    float textureWidth = static_cast<float>(sprite->GetTextureWidth());
    float textureHeight = static_cast<float>(sprite->GetTextureHeight());

    

    for (int i = 0; i < ShieldCount; i++)
    {
        sprite->Render(dc,
        20 * i, 200,
        20,20,
        0, 0,
        textureWidth,
        textureHeight,
        0,
        1, 1, 1, 1
    );
    }
    
}