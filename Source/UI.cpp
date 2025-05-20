#include "UI.h"
#include "Player.h"
#include "Camera.h"
#include <algorithm>

void UserInterface::Initialize()
{
	healthUIBackSpr = std::make_unique<Sprite>("Data/Sprite/HpEmpty.png");
	healthUIBackSD = {
		25, 20, 500, 30,
		0, 0,
		static_cast<float>(healthUIBackSpr->GetTextureWidth()),
		static_cast<float>(healthUIBackSpr->GetTextureHeight()),
		0,
		1, 1, 1, 1.0f
	};

	healthUISpr = std::make_unique<Sprite>("Data/Sprite/HpGauge.png");
	healthUISD = {
		25, 20, 500, 30,
		0, 0,
		static_cast<float>(healthUISpr->GetTextureWidth()),
		static_cast<float>(healthUISpr->GetTextureHeight()),
		0,
		1, 1, 1, 1.0f
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
	// 最大HPと現在HP
	float maxHP = static_cast<float>(Player::Instance().GetMaxHealth());
	float currentHP = static_cast<float>(Player::Instance().GetHealth());

	// HP比率（0〜1）
	float hpRate = std::clamp(currentHP / maxHP, 0.0f, 1.0f);

	// テクスチャサイズ
	float texW = static_cast<float>(healthUISpr->GetTextureWidth());
	float texH = static_cast<float>(healthUISpr->GetTextureHeight());

	// ===== ✅ フロントバー（即時反映） =====
	healthUISD.dw = 500.0f * hpRate;
	healthUISD.dh = 30.0f;
	healthUISD.sw = texW * hpRate;
	healthUISD.sh = texH;
	healthUISD.sx = 0.0f;
	healthUISD.sy = 0.0f;

	// ===== ✅ 攻撃を受けたか確認 =====
	bool nowHit = Player::Instance().GetHpHit();

	if (!prevHealthHit && nowHit)
	{
		isHealthAnimating = true;
		healthUITimer = 15;

		healthUIBackSD.dw = healthUISD.dw + 40.0f;
		healthUIBackSD.dw = std::clamp(healthUIBackSD.dw, 0.0f, 500.0f);

		float backRate = healthUIBackSD.dw / 500.0f;
		healthUIBackSD.sw = texW * backRate;
		healthUIBackSD.sh = texH;
		healthUIBackSD.sx = 0.0f;
		healthUIBackSD.sy = 0.0f;
	}

	// ✅ 一定時間経過後にフラグをオフに
	if (healthUITimer == 0)
	{
		Player::Instance().SetHpHit(false);
	}

	prevHealthHit = nowHit;

	// === ✅ 遅延バーの追従処理 ===
	float targetDW = 500.0f * hpRate;
	if (healthUIBackSD.dw > targetDW)
	{
		healthUIBackSD.dw -= 2.0f;
		if (healthUIBackSD.dw < targetDW)
		{
			healthUIBackSD.dw = targetDW;
			isHealthAnimating = false;
		}

		float backRate = healthUIBackSD.dw / 500.0f;
		healthUIBackSD.sw = texW * backRate;
		healthUIBackSD.sh = texH;
		healthUIBackSD.sx = 0.0f;
		healthUIBackSD.sy = 0.0f;
	}

	// === ✅ 状態保存 ===
	prevHealthHit = nowHit;
}

void UserInterface::Render(ID3D11DeviceContext* dc)
{
	//shader->Draw(dc, compass.get()->GetModel());
	float shakeX = 0.0f;
	float shakeY = 0.0f;

	if (isHealthAnimating && healthUITimer > 0)
	{
		auto getRand = []() -> float {
			const int max = RAND_MAX / 2;
			return static_cast<float>(rand() - max) / max;
		};
		float shakeRange = 4.0f;
		shakeX = getRand() * shakeRange;
		shakeY = getRand() * shakeRange;
		healthUITimer--;
	}

	// ===== ✅ 描画座標設定 =====
	healthUISD.dx = 25.0f + shakeX;
	healthUISD.dy = 20.0f + shakeY;

	healthUIBackSD.dx = 25.0f + shakeX;
	healthUIBackSD.dy = 20.0f + shakeY;
	healthUIBackSD.dh = 30.0f;

	// ===== ✅ 描画順 =====
	healthUIBackSpr->Render(dc, healthUIBackSD);
	healthUISpr->Render(dc, healthUISD);

}
