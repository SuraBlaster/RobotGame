#pragma once

#include "Graphics/Model.h"
#include "Stage.h"

class StageMain : public Stage
{
public:
    StageMain();
    ~StageMain()override;


    //�X�V����
    void Update(float elapsedTime)override;

    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    //���C�L���X�g
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

    DirectX::XMVECTOR currentGravityDir = DirectX::XMVectorSet(0, -1, 0, 0); // ������

    DirectX::XMVECTOR currentStageRotation = DirectX::XMQuaternionIdentity(); // ����];

    GravityDirection selectedDirection = GravityDirection::Gravity_Down;

    float rotateDuration = 3.0f
};