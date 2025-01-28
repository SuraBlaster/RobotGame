#pragma once

#include <DirectXMath.h>

class CameraController
{
public:
    CameraController() {}
    ~CameraController() {}

    void Update(float elapsedTime);
    void UpdateNormal(float elapsedTime);
    void UpdateNormal(float elapsedTime, float range);
    void DrawDebugGUI();
    void ZeroClear() { isZeroClear = true; }
    void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }

    float rollSpeed = DirectX::XMConvertToRadians(10);
    float normalRollSpeed = DirectX::XMConvertToRadians(120);
private:
    DirectX::XMFLOAT3 target = { 0,0,0 };
    DirectX::XMFLOAT3 angle = { DirectX::XMConvertToRadians(-10),0,0 };

    float range = 12.0f;
    float beforRange = range;
    float minAngleX = DirectX::XMConvertToRadians(45);
    float maxAngleX = DirectX::XMConvertToRadians(-45);
    bool isZeroClear = true;
};
