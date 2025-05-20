#pragma once

#include<DirectXMath.h>
#include <CameraEffect_Death.h>
#include <CameraEffect_Clear.h>

class CameraController
{
public:
    CameraController(){}
    ~CameraController(){}

    void Update(float elapsedTime);

    void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }

    void ZeroClear() { isZeroClear = true; }

    void CameraController::StartDeath();

    void StartClear();

    float rollSpeed = DirectX::XMConvertToRadians(10);
    float normalRollSpeed = DirectX::XMConvertToRadians(120);
    bool i = true;
private:
    DirectX::XMFLOAT3 target = { 0,0,0 };
    DirectX::XMFLOAT3 angle = { DirectX::XMConvertToRadians(-10),0,0 };

    CameraEffect_Death cameraEffect_Death;
    CameraEffect_Clear cameraEffect_Clear;


    float range = 12.0f;
    float beforRange = range;
    float minAngleX = DirectX::XMConvertToRadians(45);
    float maxAngleX = DirectX::XMConvertToRadians(-45);
    bool  isZeroClear = true;
};
