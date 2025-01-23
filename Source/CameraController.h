#pragma once

#include<DirectXMath.h>

class CameraController
{
public:
    CameraController(){}
    ~CameraController(){}

    void Update(float elapsedTime);

    void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }

    void ZeroClear() { isZeroClear = true; }

    float rollSpeed = DirectX::XMConvertToRadians(10);
    float normalRollSpeed = DirectX::XMConvertToRadians(120);
    bool i = true;
private:
    DirectX::XMFLOAT3 target = { 0,0,0 };
    DirectX::XMFLOAT3 angle = { DirectX::XMConvertToRadians(-10),0,0 };

    float range = 12.0f;
    float beforRange = range;
    float minAngleX = DirectX::XMConvertToRadians(45);
    float maxAngleX = DirectX::XMConvertToRadians(-45);
    bool  isZeroClear = true;
};
