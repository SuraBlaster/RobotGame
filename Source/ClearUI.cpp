#include "ClearUI.h"
#include "Graphics/Graphics.h"
#include "Player.h"
#include "Camera.h"
#include <Input/Input.h>
#include <Input/Mouse.h>
#include "SceneLoading.h"
#include "SceneTitle.h"
#include "SceneSelect.h"


void ClearUI::Initialize()
{
	Graphics& graphics = Graphics::Instance();
	float screenWidth = static_cast<float>(graphics.GetScreenWidth());
	float screenHeight = static_cast<float>(graphics.GetScreenHeight());

	GameClearBack = std::make_unique<Sprite>("Data/Sprite/Result.png");
	GameClearBackSD = {
		0, 0, screenWidth, screenHeight,
		0, 0,
		static_cast<float>(GameClearBack->GetTextureWidth()),
		static_cast<float>(GameClearBack->GetTextureHeight()),
		0,
		1, 1, 1, 0.0f
	};

    GameClear = std::make_unique<Sprite>("Data/Sprite/StageClear.png");
	GameClearSD = {
		40, screenHeight / 2 - 300, 1200, 307.0f,
		0, 0,
		static_cast<float>(GameClear->GetTextureWidth()),
		static_cast<float>(GameClear->GetTextureHeight()),
		0,
		1, 1, 1, 0.0f
	};

	ReturnSelect = std::make_unique<Sprite>("Data/Sprite/ReturnSelect.png");
	ReturnSelectSD = {
		screenWidth / 2 - 430, screenHeight / 2 + 55, 450, 215.65f,
		0, 0,
		static_cast<float>(ReturnSelect->GetTextureWidth()),
		static_cast<float>(ReturnSelect->GetTextureHeight()),
		0,
		1, 1, 1, 0.0f
	};

    ReturnTitle = std::make_unique<Sprite>("Data/Sprite/ReturnTitle.png");
	ReturnTitleSD = {
		screenWidth / 2 - 10 , screenHeight / 2 + 50, 400, 230,
		0, 0,
		static_cast<float>(ReturnTitle->GetTextureWidth()),
		static_cast<float>(ReturnTitle->GetTextureHeight()),
		0,
		1, 1, 1, 0.0f
	};

}

void ClearUI::Update(float elapsedTime)
{
	if (Camera::Instance().GetClearFlag())
	{
		GameClearBackSD.a = 0.5f;
		GameClearSD.a = 1.0f;
		ReturnSelectSD.a = 1.0f;
		ReturnTitleSD.a = 1.0f;

		Mouse& mouse = Input::Instance().GetMouse();

		if (mouse.mouseVsRect(ReturnSelectSD.dx, ReturnSelectSD.dy,
			ReturnSelectSD.dw, ReturnSelectSD.dh))
		{
			ReturnSelectSD.r = ReturnSelectSD.g = ReturnSelectSD.b = 0.7f;
			if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
			{
				SceneManager::Instance().ChangeScene(new SceneLoading(new SceneSelect));
				Camera::Instance().SetClearFlag(false);
			}
		}
		else
		{
			ReturnSelectSD.r = ReturnSelectSD.g = ReturnSelectSD.b = 1.0f;
		}

		if (mouse.mouseVsRect(ReturnTitleSD.dx, ReturnTitleSD.dy,
			ReturnTitleSD.dw, ReturnTitleSD.dh))
		{
			ReturnTitleSD.r = ReturnTitleSD.g = ReturnTitleSD.b = 0.7f;
			if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
			{
				SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
				Camera::Instance().SetClearFlag(false);
			}
		}
		else
		{
			ReturnTitleSD.r = ReturnTitleSD.g = ReturnTitleSD.b = 1.0f;
		}
	}

	
}

void ClearUI::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();

	GameClearBack->Render(dc, GameClearBackSD);
	GameClear->Render(dc, GameClearSD);
	ReturnSelect->Render(dc, ReturnSelectSD);
	ReturnTitle->Render(dc, ReturnTitleSD);

}