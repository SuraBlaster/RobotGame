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

	StageManager::Instance().SetButtonFlag(false);
}

void SceneSelect::Finalize()
{
    if (sprite != nullptr)
    {
        delete sprite;
        sprite = nullptr;
    }

	StageManager::Instance().Clear();
}

void SceneSelect::Update(float elapsedTime)
{
    GamePad& gamepad = Input::Instance().GetGamePad();

	//何かボタンを押したら遷移
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
			timer = 2.0f;
			StageManager::Instance().SetButtonFlag(true);
		}
		if (timer < 0.0f)
		{
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
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
			timer = 2.0f;
			StageManager::Instance().SetButtonFlag(true);
		}
		if (timer < 0.0f)
		{
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
		}
		break;
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
		
	}

	// 2DデバッグGUI描画
	{
		
	}

}