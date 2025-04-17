#pragma once

#include "Player.h"
#include "CameraController.h"
#include "Scene.h"
#include "Graphics/Sprite.h"
#include "ShieldGauge.h"
#include "ShieldIcon.h"

// �Q�[���V�[��
class SceneGame : public Scene
{
public:
	SceneGame() {}
	~SceneGame() override{}

	// ������
	void Initialize()override;

	// �I����
	void Finalize()override;

	// �X�V����
	void Update(float elapsedTime)override;

	// �`�揈��
	void Render()override;

private:
	void RenderEnemyGauge(
		ID3D11DeviceContext* dc,
		const DirectX::XMFLOAT4X4& view,
		const DirectX::XMFLOAT4X4& projection
	);

private:

	Player* player = nullptr;

	CameraController* cameraController = nullptr;

	Sprite* gauge = nullptr;

	std::unique_ptr<ShieldGauge> shieldGauge = nullptr;

	std::unique_ptr<ShieldIcon> shieldIcon = nullptr;

	DirectX::XMFLOAT4 lightdir;
};
