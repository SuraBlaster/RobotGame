#pragma once

#include "Player.h"
#include "CameraController.h"
#include "Scene.h"
#include "Graphics/Sprite.h"

// ゲームシーン
class SceneGame : public Scene
{
public:
	SceneGame() {}
	~SceneGame() override{}

	// 初期化
	void Initialize()override;

	// 終了化
	void Finalize()override;

	// 更新処理
	void Update(float elapsedTime)override;

	// 描画処理
	void Render()override;

private:
	void RenderEnemyGauge(
		ID3D11DeviceContext* dc,
		const DirectX::XMFLOAT4X4& view,
		const DirectX::XMFLOAT4X4& projection
	);

	void pauseUpdate();
	void pauseRender(ID3D11DeviceContext* dc);
private:
	bool isCameraControll = true;
	bool isOldCameraControll = true;
	bool isPause = false;

	Player* player = nullptr;

	CameraController* cameraController = nullptr;

	float screenWidth, screenHeight,
		  textureWidth, textureHeight;

	Sprite* gauge = nullptr;

	std::unique_ptr<Sprite> toTitleSpr = nullptr;
	std::unique_ptr<Sprite> backSpr = nullptr;
	std::unique_ptr<Sprite> sprite = nullptr;

	SpriteData spriteSD;
	SpriteData toTitleSD;
	SpriteData backSD;
};
