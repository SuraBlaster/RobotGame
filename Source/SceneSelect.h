#pragma once

#include "CameraController.h"
#include "Scene.h"
#include "Graphics/Sprite.h"
#include "ShieldGauge.h"
#include "ShieldIcon.h"

#include"Audio/Audio.h"
#include"Audio/AudioSource.h"
// ゲームシーン
class SceneSelect : public Scene
{
public:
	SceneSelect() {}
	~SceneSelect() override {}

	// 初期化
	void Initialize()override;

	// 終了化
	void Finalize()override;

	// 更新処理
	void Update(float elapsedTime)override;

	// 描画処理
	void Render()override;

	enum class Stage
	{
		Stage1,
		Stage2,
	};
	Stage stage = Stage::Stage1;
private:
	Sprite* sprite = nullptr;

	//BGM
	std::unique_ptr<AudioSource> SelectBGM;
	std::unique_ptr<AudioSource> KuruKuruBGM;
	float timer;
	//選択したときに曲を止める用
	bool KuruKuru = false;
};
