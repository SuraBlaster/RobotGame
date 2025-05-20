#include "Camera.h"
#include "CameraEffect_Death.h"
#include <algorithm>
#include <cmath>

void CameraEffect_Death::Start(const DirectX::XMFLOAT3& playerPosition, const DirectX::XMFLOAT3& cameraPosition)
{
    centerPos = playerPosition;
    elapsedTime = 0.0f;
    active = true;

    float dx = cameraPosition.x - centerPos.x;
    float dz = cameraPosition.z - centerPos.z;

    initialAngle = std::atan2(dz, dx);
    radiusStart = std::sqrt(dx * dx + dz * dz);
}

void CameraEffect_Death::Update(float deltaTime)
{
    if (!active) return;

    elapsedTime += deltaTime;
    float t = std::min(elapsedTime / duration, 1.0f);

    // イージング（EaseOutQuad）
    float easedT = 1.0f - (1.0f - t) * (1.0f - t);

    // カメラを1周させる
    float angle = initialAngle + DirectX::XM_2PI * easedT;

    // rangeを近づける
    float radius = radiusStart + (radiusEnd - radiusStart) * easedT;

    // 高さを持ち上げる
    float height = centerPos.y + 2.0f + 2.0f * easedT;

    DirectX::XMFLOAT3 eye;
    eye.x = centerPos.x + std::cos(angle) * radius;
    eye.z = centerPos.z + std::sin(angle) * radius;
    eye.y = height;

    Camera::Instance().SetLookAt(eye, centerPos, { 0, 1, 0 });

    if (t >= 1.0f) {
        Camera::Instance().SetFadeFlag(true);
    }
}

bool CameraEffect_Death::IsActive() const
{
    return active;
}