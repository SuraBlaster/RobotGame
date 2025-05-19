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

	//�����{�^������������J��
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

	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// �`�揈��
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ���C�g�����i�������j

	//�J���������ݒ�
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	// 3D���f���`��
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);

		//�X�e�[�W�`��
		StageManager::Instance().Render(dc, shader);

		shader->End(dc);
	}

	// 3D�f�o�b�O�`��
	{
		// ���C�������_���`����s
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// �f�o�b�O�����_���`����s
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}

	// 2D�X�v���C�g�`��
	{
		
	}

	// 2D�f�o�b�OGUI�`��
	{
		
	}

}