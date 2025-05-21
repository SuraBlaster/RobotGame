#pragma once

#include "CameraController.h"
#include "Scene.h"
#include "Graphics/Sprite.h"
#include "ShieldGauge.h"
#include "ShieldIcon.h"

// �Q�[���V�[��
class SceneSelect : public Scene
{
public:
	//�B��̃C���X�^���X�擾
	static SceneSelect& Instance()
	{
		static SceneSelect instance;
		return instance;
	}

	SceneSelect() {}
	~SceneSelect() override {}

	// ������
	void Initialize()override;

	// �I����
	void Finalize()override;

	// �X�V����
	void Update(float elapsedTime)override;

	// �`�揈��
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
