#pragma once

#include "CameraController.h"
#include "Scene.h"
#include "Graphics/Sprite.h"
#include "ShieldGauge.h"
#include "ShieldIcon.h"

// ゲームシーン
class SceneSelect : public Scene
{
public:
	//唯一のインスタンス取得
	static SceneSelect& Instance()
	{
		static SceneSelect instance;
		return instance;
	}

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

	int GetMap()  { return map; }
	void SetMap(int map) { this->map = map; }
	int map;

	enum class Stage
	{
		Stage1,
		Stage2,
	};
	Stage stage = Stage::Stage1;
	
private:
	Sprite* coreRoom = nullptr;
	Sprite* corridor = nullptr;
	float timer;
	bool hitFlag = true;
};
