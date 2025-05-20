#include "GameOverUI.h"
#include "Graphics/Graphics.h"
#include "Player.h"
#include "Camera.h"
#include <Input/Input.h>
#include <Input/Mouse.h>
#include "SceneLoading.h"
#include "SceneTitle.h"
#include "SceneSelect.h"


void GameOverUI::Initialize()
{
	Graphics& graphics = Graphics::Instance();
	float screenWidth = static_cast<float>(graphics.GetScreenWidth());
	float screenHeight = static_cast<float>(graphics.GetScreenHeight());

	GameOverBack = std::make_unique<Sprite>("Data/Sprite/GameOver.png");
	GameOverBackSD = {
		0, 0, screenWidth, screenHeight,
		0, 0,
		static_cast<float>(GameOverBack->GetTextureWidth()),
		static_cast<float>(GameOverBack->GetTextureHeight()),
		0,
		1, 1, 1, 0.0f
	};

	GameOver = std::make_unique<Sprite>("Data/Sprite/GameOverTxt.png");
	GameOverSD = {
		40, screenHeight / 2 - 300, 1200, 307.0f,
		0, 0,
		static_cast<float>(GameOver->GetTextureWidth()),
		static_cast<float>(GameOver->GetTextureHeight()),
		0,
		1, 1, 1, 0.0f
	};

	Retry = std::make_unique<Sprite>("Data/Sprite/Retry.png");
	RetrySD = {
		screenWidth / 2 - 430, screenHeight / 2 + 55, 450, 215.65f,
		0, 0,
		static_cast<float>(Retry->GetTextureWidth()),
		static_cast<float>(Retry->GetTextureHeight()),
		0,
		1, 1, 1, 0.0f
	};

	ReturnTitle = std::make_unique<Sprite>("Data/Sprite/OverTitle.png");
	ReturnTitleSD = {
		screenWidth / 2 - 10 , screenHeight / 2 + 50, 400, 230,
		0, 0,
		static_cast<float>(ReturnTitle->GetTextureWidth()),
		static_cast<float>(ReturnTitle->GetTextureHeight()),
		0,
		1, 1, 1, 0.0f
	};

}

void GameOverUI::Update(float elapsedTime)
{
	if (Camera::Instance().GetOverFlag())
	{
		GameOverBackSD.a = 0.5f;
		GameOverSD.a = 1.0f;
		RetrySD.a = 1.0f;
		ReturnTitleSD.a = 1.0f;

		Mouse& mouse = Input::Instance().GetMouse();

		if (mouse.mouseVsRect(RetrySD.dx, RetrySD.dy,
			RetrySD.dw, RetrySD.dh))
		{
			RetrySD.r = RetrySD.g = RetrySD.b = 0.7f;
			if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
			{
				SceneManager::Instance().ChangeScene(new SceneLoading(new SceneSelect));
				Camera::Instance().SetOverFlag(false);
			}
		}
		else
		{
			RetrySD.r = RetrySD.g = RetrySD.b = 1.0f;
		}

		if (mouse.mouseVsRect(ReturnTitleSD.dx, ReturnTitleSD.dy,
			ReturnTitleSD.dw, ReturnTitleSD.dh))
		{
			ReturnTitleSD.r = ReturnTitleSD.g = ReturnTitleSD.b = 0.7f;
			if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
			{
				SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
				Camera::Instance().SetOverFlag(false);
			}
		}
		else
		{
			ReturnTitleSD.r = ReturnTitleSD.g = ReturnTitleSD.b = 1.0f;
		}
	}


}

void GameOverUI::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();

	GameOverBack->Render(dc, GameOverBackSD);
	GameOver->Render(dc, GameOverSD);
	Retry->Render(dc, RetrySD);
	ReturnTitle->Render(dc, ReturnTitleSD);

}