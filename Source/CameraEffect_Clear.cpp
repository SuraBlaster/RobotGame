#include "Camera.h"
#include "CameraEffect_Clear.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>

void CameraEffect_Clear::Start(const DirectX::XMFLOAT3& playerPos, const DirectX::XMFLOAT3& cameraPos)
{
    active = true;
    timer = 0.0f;
    playerPosition = playerPos;
    shakeOffset = { 0.0f, 0.0f, 0.0f };
}

void CameraEffect_Clear::Update(float deltaTime)
{
    if (!active) return;

    timer += deltaTime;
    if (timer >= totalDuration) {
        Camera::Instance().SetFadeFlag(true);
    }

    float currentRange = startRange;
    float heightOffset = 0.0f;
    shakeOffset = { 0.0f, 0.0f, 0.0f };

    // --- Phase 1: ゆっくり近づく（0.0〜0.3s）
    if (timer < 0.3f) {
        float t = timer / 0.3f;
        currentRange = Lerp(startRange, midRange, t);
        heightOffset = Lerp(2.0f, 3.0f, t);
    }
    // --- Phase 2: 静止（0.3〜0.6s）
    else if (timer < 0.6f) {
        currentRange = midRange;
        heightOffset = 3.0f;
    }
    // --- Phase 3: 一気にズーム（0.6〜0.9s）
    else if (timer < 0.9f) {
        float t = (timer - 0.6f) / 0.3f;
        currentRange = Lerp(midRange, endRange, t);
        heightOffset = Lerp(3.0f, 1.5f, t);

        // --- Phase 4: シェイク（0.85〜0.9s）
        if (timer >= 0.85f) {
            float shakeStrength = 0.1f;
            shakeOffset.x = (rand() / (float)RAND_MAX - 0.5f) * 2.0f * shakeStrength;
            shakeOffset.y = (rand() / (float)RAND_MAX - 0.5f) * 2.0f * shakeStrength;
        }
    }
    // --- Phase 5: 完全静止（0.9〜1.9s）
    else {
        currentRange = endRange;
        heightOffset = 1.5f;
    }

    // --- カメラのeyeとfocus計算
    DirectX::XMFLOAT3 forward = { 0.0f, 0.0f, -1.0f };
    DirectX::XMFLOAT3 eye = {
        playerPosition.x + forward.x * currentRange + shakeOffset.x,
        playerPosition.y + heightOffset + shakeOffset.y,
        playerPosition.z + forward.z * currentRange + shakeOffset.z
    };

    DirectX::XMFLOAT3 focus = {
        playerPosition.x,
        playerPosition.y + 1.5f,
        playerPosition.z
    };

    DirectX::XMFLOAT3 up = { 0.0f, 1.0f, 0.0f };

    Camera::Instance().SetLookAt(eye, focus, up);
        
    
}

bool CameraEffect_Clear::IsActive() const
{
    return active;
}