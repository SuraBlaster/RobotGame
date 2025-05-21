#include "SceneSelect.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "Graphics/Graphics.h"
#include "SceneManager.h"
#include "Input/Input.h"
#include "SceneLoading.h"
#include "StageManager.h"
#include "SelectStage1.h"
#include "SelectStage2.h"
#include <Camera.h>

void SceneSelect::Initialize()
{
<<<<<<< HEAD
	coreRoom = new Sprite("Data/Sprite/CoreRoom.png");
	corridor = new Sprite("Data/Sprite/Corridor.png");

=======
	Audio& audio = audio.Instance();
	SelectBGM = audio.LoadAudioSource("Data/Audio/BGM/Select.wav");
	KuruKuruBGM = audio.LoadAudioSource("Data/Audio/BGM/KuruKuru.wav");
	SelectBGM->sourceVoice->SetVolume(50);
	KuruKuruBGM->sourceVoice->SetVolume(50);
	KuruKuru = false;
>>>>>>> ruisan114514
	StageManager& stageManager = StageManager::Instance();
	SelectStage1* selectStage1 = new SelectStage1();
	selectStage1->SetPosition({ -40,10,10 });
	stageManager.Register(selectStage1);

	SelectStage2* selectStage2 = new SelectStage2();
	selectStage2->SetPosition({ 40,10,10 });
	stageManager.Register(selectStage2);

	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -100),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	camera.SetParspectiveFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		1000.0f
	);

	StageManager::Instance().SetCursorFlag1(false);
	StageManager::Instance().SetCursorFlag2(false);

}

void SceneSelect::Finalize()
{
    if (coreRoom != nullptr)
    {
        delete coreRoom;
        coreRoom = nullptr;
    }

	if (corridor != nullptr)
	{
		delete corridor;
		corridor = nullptr;
	}

	StageManager::Instance().Clear();
}

void SceneSelect::Update(float elapsedTime)
{
<<<<<<< HEAD
	// マウス入力取得
	Mouse& mouse = Input::Instance().GetMouse();

	if (timer < 0.0f)
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
=======
	if (KuruKuru == false) 
	{
		SelectBGM->Play(true);
	}
	else if (KuruKuru == true)
	{
		SelectBGM->Stop();
	}
    GamePad& gamepad = Input::Instance().GetGamePad();

	//菴輔°繝懊ち繝ｳ繧呈款縺励◆繧蛾・遘ｻ
	const GamePadButton anyButton =
		GamePad::BTN_A
		| GamePad::BTN_B
		| GamePad::BTN_X
		| GamePad::BTN_Y
		;

	switch (stage)
	{
	case Stage::Stage1:
		if (gamepad.GetButtonDown() & GamePad::BTN_RIGHT)
		{
			StageManager::Instance().SetStage(Stage::Stage2);
			stage = Stage::Stage2;
		}
		if (gamepad.GetButtonDown() & anyButton)
		{

		
			KuruKuru = true;
			KuruKuruBGM->Play(false);

			SceneSelect::Instance().SetMap(1);
			timer = 2.0f;

			StageManager::Instance().SetButtonFlag(true);
		}
		if (timer < 0.0f)
		{
			
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));

		}
		if (timer > 5.0f)
		{
			KuruKuruBGM->Stop();
		}
		break;
	case Stage::Stage2:
		if (gamepad.GetButtonDown() & GamePad::BTN_LEFT)
		{
			StageManager::Instance().SetStage(Stage::Stage1);
			stage = Stage::Stage1;
		}
		if (gamepad.GetButtonDown() & anyButton)
		{

		
			KuruKuru = true;
			KuruKuruBGM->Play(false);

			SceneSelect::Instance().SetMap(2);
			timer = 2.0f;

			StageManager::Instance().SetButtonFlag(true);
		}
		if (timer < 0.0f)
		{
			
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
		}
		if (timer > 5.0f)
		{
			KuruKuruBGM->Stop();
		}
		break;
>>>>>>> ruisan114514
	}

	if (StageManager::Instance().GetCursorFlag1())
	{
		if (timer <= 0.0f)
		{
			timer = 2.0f;
			StageManager::Instance().SetCursorFlag1(true);
			SceneSelect::Instance().SetMap(1);
		}
	}

	if (StageManager::Instance().GetCursorFlag2())
	{
		if (timer <= 0.0f)
		{
			timer = 2.0f;
			StageManager::Instance().SetCursorFlag2(true);
			SceneSelect::Instance().SetMap(2);
		}
	}

	
	if (timer > 0.0f)
	{
		timer -= elapsedTime;
	}

	
	
	

	StageManager::Instance().Update(elapsedTime);
}

void SceneSelect::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// 逕ｻ髱｢繧ｯ繝ｪ繧｢・・Ξ繝ｳ繝繝ｼ繧ｿ繝ｼ繧ｲ繝・ヨ險ｭ螳・
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0・・.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// 謠冗判蜃ｦ逅・
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// 繝ｩ繧､繝域婿蜷托ｼ井ｸ区婿蜷托ｼ・

	//繧ｫ繝｡繝ｩ蛻晄悄險ｭ螳・
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	// 3D繝｢繝・Ν謠冗判
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);

		//繧ｹ繝・・繧ｸ謠冗判
		StageManager::Instance().Render(dc, shader);

		shader->End(dc);
	}

	// 3D繝・ヰ繝・げ謠冗判
	{
		// 繝ｩ繧､繝ｳ繝ｬ繝ｳ繝繝ｩ謠冗判螳溯｡・
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// 繝・ヰ繝・げ繝ｬ繝ｳ繝繝ｩ謠冗判螳溯｡・
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}

	// 2D繧ｹ繝励Λ繧､繝域緒逕ｻ
	{
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());
		float textureWidth = static_cast<float>(coreRoom->GetTextureWidth());
		float textureHeight = static_cast<float>(coreRoom->GetTextureHeight());

		coreRoom->Render(dc,
			100, 450, 400.0f, 170.0f,
			0, 0, textureWidth, textureHeight,
			0,
			1, 1, 1, 1);

		textureWidth = static_cast<float>(corridor->GetTextureWidth());
		textureHeight = static_cast<float>(corridor->GetTextureHeight());

		corridor->Render(dc,
			780, 450, 400.0f, 170.0f,
			0, 0, textureWidth, textureHeight,
			0,
			1, 1, 1, 1);

	}

<<<<<<< HEAD
	// 当たり判定
=======
	// 2D繝・ヰ繝・げGUI謠冗判
>>>>>>> ruisan114514
	{
		
	}

}

