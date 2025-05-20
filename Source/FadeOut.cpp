#include "FadeOut.h"
#include "Graphics/Graphics.h"
#include "Player.h"
#include "Camera.h"

void FadeOut::Initialize()
{
    sprite = std::make_unique<Sprite>("Data/Sprite/Result.png");

    transparency = 0.0f;

    Camera::Instance().SetFadeFlag(false);
}

void FadeOut::Update(float elapsedTime)
{
    if (Camera::Instance().GetFadeFlag() == true)
    {
        transparency = 0.5f;
    }
}

void FadeOut::Render()
{

    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();

    float screenWidth = static_cast<float>(graphics.GetScreenWidth());
    float screenHeight = static_cast<float>(graphics.GetScreenHeight());
    float textureWidth = static_cast<float>(sprite->GetTextureWidth());
    float textureHeight = static_cast<float>(sprite->GetTextureHeight());

    sprite->Render(dc,
        0, 0,
        screenWidth,
        screenHeight,
        0, 0,
        textureWidth,
        textureHeight,
        0,
        1, 1, 1, transparency
    );


}