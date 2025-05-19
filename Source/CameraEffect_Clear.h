#pragma once
#include <DirectXMath.h>

#pragma once
#include <DirectXMath.h>

class CameraEffect_Clear
{
public:
    void Start(const DirectX::XMFLOAT3& playerPosition, const DirectX::XMFLOAT3& cameraPosition);
    void Update(float deltaTime);
    bool IsActive() const;

private:
    bool active = false;
    float timer = 0.0f;
    const float totalDuration = 1.8f;

    // 各フェーズ用のrange
    float startRange = 10.0f;
    float midRange = 7.0f;
    float endRange = 3.5f;

    DirectX::XMFLOAT3 playerPosition{};
    DirectX::XMFLOAT3 shakeOffset{};

    // ヘルパー
    float Lerp(float a, float b, float t) const {
        return a + (b - a) * t;
    }
};
