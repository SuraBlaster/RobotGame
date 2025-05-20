#include "SceneTitle.h"
#include "SceneGame.h"
#include "Graphics/Graphics.h"
#include "SceneManager.h"
#include "Input/Input.h"
#include "SceneLoading.h"
#include "SceneSelect.h"

void SceneTitle::Initialize()
{
    //BGM初期化
    Audio& audio = audio.Instance();
    //BGM設定
    TitleBGM = audio.LoadAudioSource("Data/Audio/BGM/Title.wav");
    //BGM音量設定
    TitleBGM->sourceVoice->SetVolume(100);
    //スプライト初期化
    sprite = new Sprite("Data/Sprite/Title.png");
    startSpr = std::make_unique<Sprite>("Data/Sprite/UI0.png");
    gameCloseSpr = std::make_unique<Sprite>("Data/Sprite/UI4.png");

    startSD = {
        800,500, 250, 50,
        0, 0,
        static_cast<float>(startSpr->GetTextureWidth()),
        static_cast<float>(startSpr->GetTextureHeight())
    };

    gameCloseSD = {
    300, 500, 250, 50,
    0, 0,
    static_cast<float>(gameCloseSpr->GetTextureWidth()),
    static_cast<float>(gameCloseSpr->GetTextureHeight())
    };
}
void SceneTitle::Finalize()
{
    if (sprite != nullptr)
    {
        delete sprite;
        sprite = nullptr;
    }
}

void SceneTitle::Update(float elapsedTime)
{
    //GamePad& gamepad = Input::Instance().GetGamePad();
    ////何かボタンを押したら遷移
    //const GamePadButton anyButton =
    //    GamePad::BTN_A
    //    | GamePad::BTN_B
    //    | GamePad::BTN_X
    //    | GamePad::BTN_Y
    //    ;
   /* if (gamepad.GetButtonDown() & anyButton)
    {
        SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
    }*/
    TitleBGM->Play(true);
    Mouse& mouse = Input::Instance().GetMouse();

    if (mouse.mouseVsRect(startSD.dx, startSD.dy,
        startSD.dw, startSD.dh))
    {
        //TitleSE->Play(false);
        startSD.r = startSD.g = startSD.b = 0.7f;
        if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
        {
            SceneManager::Instance().ChangeScene(new SceneLoading(new SceneSelect));
        }
    }
    else
    {
        startSD.r = startSD.g = startSD.b = 1.0f;
    }

    if (mouse.mouseVsRect(gameCloseSD.dx, gameCloseSD.dy,
        gameCloseSD.dw, gameCloseSD.dh))
    {
        //TitleSE->Play(false);
        gameCloseSD.r = gameCloseSD.g = gameCloseSD.b = 0.7f;
        if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
        {
            SceneManager::Instance().GameClose();
        }
    }
    else
    {
        gameCloseSD.r = gameCloseSD.g = gameCloseSD.b = 1.0f;
    }
}

void SceneTitle::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    //画面クリア&レンダーターゲット設定
    FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f };
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    //2Dスプライト描画
    {
        float screenWidth = static_cast<float>(graphics.GetScreenWidth());
        float screenHeight = static_cast<float>(graphics.GetScreenHeight());
        float textureWidth = static_cast<float>(sprite->GetTextureWidth());
        float textureHeight = static_cast<float>(sprite->GetTextureHeight());
        //タイトルスプライト描画
        sprite->Render(dc,
            0, 0, screenWidth, screenHeight,
            0, 0, textureWidth, textureHeight,
            0,
            1, 1, 1, 1);

        startSpr->Render(dc, startSD);
        gameCloseSpr->Render(dc, gameCloseSD);
    }

}
