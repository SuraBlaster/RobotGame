#include "ShieldGauge.h"
#include "Graphics/Graphics.h"
#include "Player.h"
#include <algorithm>
#include <Input/Input.h>

void ShieldGauge::Initialize()
{

    shieldUISpr = std::make_unique<Sprite>("Data/Sprite/ShieldGauge.png");
    shieldUIBackSpr = std::make_unique<Sprite>("Data/Sprite/ShieldBack.png");

    shieldUISD = {
        25, 60, 500, 20,
        0, 0,
        static_cast<float>(shieldUISpr->GetTextureWidth()),
        static_cast<float>(shieldUISpr->GetTextureHeight()),
        0,
        1, 1, 1, 1.0f
    };

    shieldUIBackSD = {
        25, 60, 0, 20,   // dw = 0 ← 最初は幅0（何も見えない）
        0, 0,
        0, 0,
        0,
        1, 1, 1, 1.0f
    };
}

void ShieldGauge::Update(float elapsedTime)
{
    // 最大シールド時間
    const float maxShield = 60.0f;

    // 現在の残りシールド時間（0～60）
    float shield = Player::Instance().GetTimer() * 2.0f;
    shield = std::clamp(shield, 0.0f, maxShield);

    // レート（0.0～1.0）
    float shieldRate = shield / maxShield;

    // テクスチャサイズ取得
    float texW = static_cast<float>(shieldUISpr->GetTextureWidth());
    float texH = static_cast<float>(shieldUISpr->GetTextureHeight());

    // 前面ゲージ（現在値）
    shieldUISD.dw = 500.0f * shieldRate;
    shieldUISD.dh = 20.0f;
    shieldUISD.sw = texW * shieldRate;
    shieldUISD.sh = texH;
    shieldUISD.sx = 0.0f;
    shieldUISD.sy = 0.0f;

    bool nowHit = Player::Instance().GetHit();

    // 1回だけ反応させる：前フレームがfalseで今フレームがtrueならヒット
    if (!prevShieldHit && nowHit) {
        isShieldAnimating = true;
        shieldUITimer = 15; // ← 初回のみセット
        shieldUIBackSD.dw = shieldUISD.dw + 30.0f;
        shieldUIBackSD.dw = std::clamp(shieldUIBackSD.dw, 0.0f, 500.0f);

        float backRate = shieldUIBackSD.dw / 500.0f;
            shieldUIBackSD.sw = texW * backRate;
            shieldUIBackSD.sh = texH;
            shieldUIBackSD.sx = 0.0f;
            shieldUIBackSD.sy = 0.0f;
        
    }

    if (shieldUITimer == false)
    {
        Player::Instance().SetHit(false);
    }


    // 前フレームの状態を更新
    prevShieldHit = nowHit;

    // バックバー（アニメーション追従）
    float backTargetDW = 500.0f * shieldRate;
    if (shieldUIBackSD.dw > backTargetDW) {
        isShieldAnimating = true;
        shieldUIBackSD.dw -= 1.5f;
        if (shieldUIBackSD.dw < backTargetDW) {
            shieldUIBackSD.dw = backTargetDW;
            isShieldAnimating = false;
        }

        float backRate = shieldUIBackSD.dw / 500.0f;
        if (shieldRate > 0)
        {
            shieldUIBackSD.sw = texW * backRate;
            shieldUIBackSD.sh = texH;
            shieldUIBackSD.sx = 0.0f;
            shieldUIBackSD.sy = 0.0f;
        }
        else
        {
            shieldUIBackSD.dw = 0.0f;
            shieldUIBackSD.sw = 0;
            shieldUIBackSD.sh = texH;
            shieldUIBackSD.sx = 0.0f;
            shieldUIBackSD.sy = 0.0f;
        }
    }
}

void ShieldGauge::Render(ID3D11DeviceContext* dc)
{
    // シェイク処理
    float shakeX = 0.0f;
    float shakeY = 0.0f;
    if (isShieldAnimating && shieldUITimer > 0) {
        auto getRand = []() -> float {
            const int max = RAND_MAX / 2;
            return static_cast<float>(rand() - max) / max;
        };
        float shakeRange = 3.0f;
        shakeX = getRand() * shakeRange;
        shakeY = getRand() * shakeRange;
        shieldUITimer--;
    }

    // 位置設定
    shieldUISD.dx = 25.0f + shakeX;
    shieldUISD.dy = 60.0f + shakeY;

    shieldUIBackSD.dx = 25.0f + shakeX;
    shieldUIBackSD.dy = 60.0f + shakeY;
    shieldUIBackSD.dh = 20.0f;

    // 描画
    shieldUIBackSpr->Render(dc, shieldUIBackSD);
    shieldUISpr->Render(dc, shieldUISD);

}
