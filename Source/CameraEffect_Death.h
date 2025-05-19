#pragma once
#include <DirectXMath.h>

class CameraEffect_Death
{
public:
    void CameraEffect_Death::Start(const DirectX::XMFLOAT3& playerPosition, const DirectX::XMFLOAT3& fromPosition);
    void CameraEffect_Death::Update(float elapsedTime);

    bool CameraEffect_Death::IsActive() const;
private:
    DirectX::XMFLOAT3 centerPos;
    float elapsedTime = 0.0f;
    float duration = 3.0f;
    bool active = false;

    float initialAngle = 0.0f;
    float radiusStart = 10.0f;
    float radiusEnd = 3.0f;
};
