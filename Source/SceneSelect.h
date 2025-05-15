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
	
	//�C���X�^���X�擾
	static SceneSelect& Instance();

	int map = 0;
	int GetMap()  { return map; }
	enum class Stage
	{
		Stage1,
		Stage2,
	};
	Stage stage = Stage::Stage1;
	
private:
	Sprite* sprite = nullptr;
	float timer;
};
