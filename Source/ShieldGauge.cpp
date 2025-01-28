#include "ShieldGauge.h"
#include "Graphics/Graphics.h"

ShieldGauge::ShieldGauge()
{
    sprite[0] = std::make_unique<Sprite>("Data/Sprite/ShieldGauge.png");
    sprite[1] = std::make_unique<Sprite>("Data/Sprite/ShieldBack.png");
}

ShieldGauge::~ShieldGauge()
{
}

void ShieldGauge::Update()
{
    
}

void ShieldGauge::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();

    float screenWidth = static_cast<float>(graphics.GetScreenWidth());
    float screenHeight = static_cast<float>(graphics.GetScreenHeight());
    float textureWidth = static_cast<float>(sprite[0]->GetTextureWidth());
    float textureHeight = static_cast<float>(sprite[0]->GetTextureHeight());

    sprite[0]->Render(dc,
        10,100,
        screenWidth,
        screenHeight,
        500,100,
        textureWidth,
        textureHeight,
        0,
        1,1,1,1
    );
}
