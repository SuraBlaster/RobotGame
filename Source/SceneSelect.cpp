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
	Audio& audio = audio.Instance();
	SelectBGM = audio.LoadAudioSource("Data/Audio/BGM/Select.wav");
	KuruKuruBGM = audio.LoadAudioSource("Data/Audio/BGM/KuruKuru.wav");
	SelectBGM->sourceVoice->SetVolume(50);
	KuruKuruBGM->sourceVoice->SetVolume(50);
	KuruKuru = false;
	coreRoom = new Sprite("Data/Sprite/CoreRoom.png");
	corridor = new Sprite("Data/Sprite/Corridor.png");

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
	// マウス入力取得
	Mouse& mouse = Input::Instance().GetMouse();

	if (timer < 0.0f)
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
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

	// 画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// 描画処理
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ライト方向（下方向）

	//カメラ初期設定
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	// 3Dモデル描画
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);

		//ステージ描画
		StageManager::Instance().Render(dc, shader);

		shader->End(dc);
	}

	// 3Dデバッグ描画
	{
		// ラインレンダラ描画実行
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// デバッグレンダラ描画実行
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}

	// 2Dスプライト描画
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

	// 当たり判定
	{
		
	}

}

