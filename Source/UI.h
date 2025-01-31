#pragma once
#include <memory>
#include "Graphics/Sprite.h"
#include "Graphics/Model.h"
#include "Graphics/Shader.h"
#include "Player.h"

class Compass
{
public:
	Compass() {}
	~Compass() {}

	void Initialize();

	void Update();
	
	Model* GetModel() { return model.get(); }

private:

	std::unique_ptr<Model> model = nullptr;

	DirectX::XMFLOAT3 position = { 0, 0, 0 };
	DirectX::XMFLOAT3 angle = { 0, 0, 0 };
	DirectX::XMFLOAT3 scale = { 1, 1, 1 };
	DirectX::XMFLOAT4X4 transform{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
};

class UserInterface
{
public:
	UserInterface() {}
	~UserInterface() {}

	// 初期化
	void Initialize();

	// 終了化
	void Finalize();

	// 更新処理
	void Update(float elapsedTime);

	// 描画処理
	void Render(ID3D11DeviceContext* dc, Shader* shader);

private:
	bool isUIAnimation = false;
	int UITimer = 20;

	std::unique_ptr<Sprite> playerHealthUISpr = nullptr;
	std::unique_ptr<Sprite> playerHealthUIBackSpr = nullptr;
	std::unique_ptr<Sprite> HealthUIBackSpr = nullptr;

	SpriteData playerHealthUISD;
	SpriteData playerHealthUIBackSD;
	SpriteData HealthUIBackSD;

	//std::unique_ptr<Compass> compass;
};