#pragma once

#include "Graphics/Model.h"
#include "Stage.h"

class StageMain : public Stage
{
public:
    StageMain();
    ~StageMain()override;


    //更新処理
    void Update(float elapsedTime)override;

    //描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    //レイキャスト
    bool RayCast(const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end, HitResult& hit)override;

    //トルク設定
    void SetTorque(const DirectX::XMFLOAT3& torque) { this->torque = torque; }

    void ChangeGravity(HitResult hit) override;
private:
    void RotationStage(float elapsedTime);
    void UpdateTransform();

    enum GravityDirection {
        Down,
        Up,
        West,
        East,
        North,
        South
    };

    GravityDirection selectedDirection = GravityDirection::Down;
    DirectX::XMVECTOR GetGravityVector(GravityDirection dir);
private:
    Model* model = nullptr;

    DirectX::XMVECTOR currentGravityDir = DirectX::XMVectorSet(0, -1, 0, 0); // 下向き

    DirectX::XMVECTOR currentStageRotation = DirectX::XMQuaternionIdentity(); // 無回転;


    DirectX::XMFLOAT3 position = { 0,0,0 };
    DirectX::XMFLOAT3 angle = { 0,0,0 };
    DirectX::XMFLOAT3 oldAngle = { 0,0,0 };
    DirectX::XMFLOAT3 scale = { 1,1,1 };
    DirectX::XMFLOAT4X4 transform = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
    DirectX::XMFLOAT4X4 oldTransform = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };

    DirectX::XMFLOAT3 torque = { 0,0,0 };

    float rotateDuration = 3.0f;

};