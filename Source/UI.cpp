#include "UI.h"
#include "Player.h"
#include "Camera.h"


void Compass::Initialize()
{
	model = std::make_unique<Model>("Data/Model/UI.mdl");
	scale = { 0.0001f,0.0003f,0.0001f };
}

void Compass::Update()
{
	position.x = Camera::Instance().GetFront().x * 0.5f + Camera::Instance().GetEye().x;
	position.y = Camera::Instance().GetFront().y * 0.5f + Camera::Instance().GetEye().y;
	position.z = Camera::Instance().GetFront().z * 0.5f + Camera::Instance().GetEye().z;
	
	//angle.x = Camera::Instance().GetFront().x;
	//angle.y = Camera::Instance().GetFront().y;
	//angle.z = Camera::Instance().GetFront().z;
	
	
	//スケール行列を作成
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	//回転行列を作成
	//DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
	DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
	DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);
	DirectX::XMMATRIX R = Y * X * Z;
	//位置行列を作成
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	//3つの行列を組み合わせ、ワールド行列を作成
	DirectX::XMMATRIX W = S * R * T;
	//計算したワールド座標を取り出す
	DirectX::XMStoreFloat4x4(&transform, W);
	model->UpdateTransform(transform);
}

void UserInterface::Initialize()
{
	playerHealthUISpr = std::make_unique<Sprite>();
	playerHealthUISD = {
		25, 20, 500, 30,
		0, 0, 0, 0,
		0,
		1, 0, 0, 1.0f
	};

	HealthUIBackSpr = std::make_unique<Sprite>("Data/Sprite/UIBack.png");
	HealthUIBackSD = {
		0, 20, 560, 40,
		0, 0,
		static_cast<float>(HealthUIBackSpr->GetTextureWidth()),
		static_cast<float>(HealthUIBackSpr->GetTextureHeight()),
		0,
		1, 1, 1, 1.0f
	};

	playerHealthUIBackSpr = std::make_unique<Sprite>();
	playerHealthUIBackSD = {
		0, 20, 100.0f, 30,
		0, 0, 0, 0,
		0,
		0.5f, 0.5f, 0.5f, 1.0f
	};

	//compass = std::make_unique<Compass>();
	//compass->Initialize();
}

void UserInterface::Finalize()
{
}

void UserInterface::Update(float elapsedTime)
{
	//compass->Update();
}

void UserInterface::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	//shader->Draw(dc, compass.get()->GetModel());
	if (Player::Instance().GetHealth() >= 0)
	{
		if (Player::Instance().GetOnDamage())
		{
			playerHealthUISD.dw = Player::Instance().GetHealth() * 100;
			playerHealthUIBackSD.dw = 100.0f;
			isUIAnimation = true;
			UITimer = 20;
		}

		auto getRand = []()->float
		{
			const int max = RAND_MAX / 2;
			return static_cast<float>(rand() - max) / max;
		};

		float UIShakeRange = 5.0f;

		if (isUIAnimation && UITimer > 0)
		{
			float shakeX = getRand() * UIShakeRange;
			float shakeY = getRand() * UIShakeRange;
			playerHealthUISD.dx += shakeX;
			playerHealthUISD.dy += shakeY;
			HealthUIBackSD.dx += shakeX;
			HealthUIBackSD.dy += shakeY;

			playerHealthUIBackSD.dx = playerHealthUISD.dw + playerHealthUISD.dx;
			playerHealthUIBackSD.dy = playerHealthUISD.dy;

			UITimer--;
		}
		else
		{
			playerHealthUISD.dx = 25;
			playerHealthUISD.dy = 20;

			HealthUIBackSD.dx = 0;
			HealthUIBackSD.dy = 20;

			playerHealthUIBackSD.dx = playerHealthUISD.dw + playerHealthUISD.dx;
			playerHealthUIBackSD.dy = playerHealthUISD.dy;

			playerHealthUIBackSD.dw -= 2;
		}

		if (isUIAnimation)
			playerHealthUIBackSpr->Render(dc, playerHealthUIBackSD);

		if (playerHealthUIBackSD.dw <= 0)
			isUIAnimation = false;

		playerHealthUISpr->Render(dc, playerHealthUISD);
		HealthUIBackSpr->Render(dc, HealthUIBackSD);
	}
}
