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
        25, 60, 0, 20,   // dw = 0 �� �ŏ��͕�0�i���������Ȃ��j
        0, 0,
        0, 0,
        0,
        1, 1, 1, 1.0f
    };
}

void ShieldGauge::Update(float elapsedTime)
{
    // �ő�V�[���h����
    const float maxShield = 60.0f;

    // ���݂̎c��V�[���h���ԁi0�`60�j
    float shield = Player::Instance().GetTimer() * 2.0f;
    shield = std::clamp(shield, 0.0f, maxShield);

    // ���[�g�i0.0�`1.0�j
    float shieldRate = shield / maxShield;

    // �e�N�X�`���T�C�Y�擾
    float texW = static_cast<float>(shieldUISpr->GetTextureWidth());
    float texH = static_cast<float>(shieldUISpr->GetTextureHeight());

    // �O�ʃQ�[�W�i���ݒl�j
    shieldUISD.dw = 500.0f * shieldRate;
    shieldUISD.dh = 20.0f;
    shieldUISD.sw = texW * shieldRate;
    shieldUISD.sh = texH;
    shieldUISD.sx = 0.0f;
    shieldUISD.sy = 0.0f;

    bool nowHit = Player::Instance().GetHit();

    // 1�񂾂�����������F�O�t���[����false�ō��t���[����true�Ȃ�q�b�g
    if (!prevShieldHit && nowHit) {
        isShieldAnimating = true;
        shieldUITimer = 15; // �� ����̂݃Z�b�g
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


    // �O�t���[���̏�Ԃ��X�V
    prevShieldHit = nowHit;

    // �o�b�N�o�[�i�A�j���[�V�����Ǐ]�j
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
    // �V�F�C�N����
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

    // �ʒu�ݒ�
    shieldUISD.dx = 25.0f + shakeX;
    shieldUISD.dy = 60.0f + shakeY;

    shieldUIBackSD.dx = 25.0f + shakeX;
    shieldUIBackSD.dy = 60.0f + shakeY;
    shieldUIBackSD.dh = 20.0f;

    // �`��
    shieldUIBackSpr->Render(dc, shieldUIBackSD);
    shieldUISpr->Render(dc, shieldUISD);

}
