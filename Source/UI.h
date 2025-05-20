#pragma once
#include <memory>
#include "Graphics/Sprite.h"
#include "Graphics/Model.h"
#include "Graphics/Shader.h"
#include "Player.h"

class UserInterface
{
public:
	UserInterface() {}
	~UserInterface() {}

	// ������
	void Initialize();

	// �I����
	void Finalize();

	// �X�V����
	void Update(float elapsedTime);

	// �`�揈��
	void Render(ID3D11DeviceContext* dc);

private:

	std::unique_ptr<Sprite> healthUISpr = nullptr;
	std::unique_ptr<Sprite> healthUIBackSpr = nullptr;
	SpriteData healthUISD{};
	SpriteData healthUIBackSD{};
	bool prevHealthHit = false;
	bool isHealthAnimating = false;
	int healthUITimer = 0;

	//std::unique_ptr<Compass> compass;
};