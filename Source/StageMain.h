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

    void ChangeGravity(HitResult hit);
private:
    Model* model = nullptr;

private:
    enum GravityDirection {
        Down,
        Up,
        West,
        East,
        North,
        South
    };

    void RotationStage(float elapsedTime);

    DirectX::XMVECTOR GetGravityVector(GravityDirection dir);

    DirectX::XMVECTOR currentGravityDir = DirectX::XMVectorSet(0, -1, 0, 0); // 下向き

    DirectX::XMVECTOR currentStageRotation = DirectX::XMQuaternionIdentity(); // 無回転;

    GravityDirection selectedDirection = GravityDirection::Gravity_Down;

    float rotateDuration = 3.0f
};