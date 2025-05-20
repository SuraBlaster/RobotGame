#include "SceneTitle.h"
#include "SceneGame.h"
#include "Graphics/Graphics.h"
#include "SceneManager.h"
#include "Input/Input.h"
#include "SceneLoading.h"
#include "SceneSelect.h"

void SceneTitle::Initialize()
{
    //スプライト初期化
    sprite = new Sprite("Data/Sprite/Title.png");
    goodBye = new Sprite("Data/Sprite/GoodBye.png");
    startSpr = std::make_unique<Sprite>("Data/Sprite/Start.png");
    gameCloseSpr = std::make_unique<Sprite>("Data/Sprite/Exit.png");

    startSD = {
        50,500, 200, 100,
        0, 0,
        static_cast<float>(startSpr->GetTextureWidth()),
        static_cast<float>(startSpr->GetTextureHeight())
    };

    gameCloseSD = {
        50, 600, 150, 100,
        0, 0,
        static_cast<float>(gameCloseSpr->GetTextureWidth()),
        static_cast<float>(gameCloseSpr->GetTextureHeight())
    };

    timer = 0.0f;
    exitFlag = false;
}
void SceneTitle::Finalize()
{
    if (sprite != nullptr)
    {
        delete sprite;
        sprite = nullptr;
    }

    if (goodBye != nullptr)
    {
        delete goodBye;
        goodBye = nullptr;
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

    Mouse& mouse = Input::Instance().GetMouse();

    if (mouse.mouseVsRect(startSD.dx, startSD.dy,
        startSD.dw, startSD.dh))
    {
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
        gameCloseSD.r = gameCloseSD.g = gameCloseSD.b = 0.7f;
        if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
        {
            exitFlag = true;
            timer = 1.0f;
            transparency = 1.0f;
        }
    }
    else
    {
        gameCloseSD.r = gameCloseSD.g = gameCloseSD.b = 1.0f;
    }

    if (timer >= 0.0f)
    {
        timer-= elapsedTime;
    }
    else if(exitFlag && timer < 0.0f)
    {
        SceneManager::Instance().GameClose();
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

        goodBye->Render(dc,
            0, 0, screenWidth, screenHeight,
            0, 0, textureWidth, textureHeight,
            0,
            1, 1, 1, transparency);

        if (exitFlag == false)
        {
            startSpr->Render(dc, startSD);
            gameCloseSpr->Render(dc, gameCloseSD);
        }
    }
}
