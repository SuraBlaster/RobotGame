#pragma once

#include "Graphics/Model.h"
#include "Stage.h"
#include "Transform.h"

class StageMain : public Stage
{
public:
    StageMain();
    ~StageMain()override;

    static StageMain& Instance();

    //�X�V����
    void Update(float elapsedTime)override;

    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    //���C�L���X�g
    bool RayCast(const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end, HitResult& hit)override;

    void ChangeGravity();

    bool GetIsRotation() { return isRotationAnimation; }
private:

    enum GravityDirection {
        YPuls,
        YMinus,
        XPuls,
        XMinus,
        ZPuls,
        ZMinus,
    };

    void RotationStage(float elapsedTime);
    Transform transform;
    Transform new_transform = {};
    DirectX::XMFLOAT3 nowAngle = {};

private:
    bool isRotationAnimation = false;
    Model* model = nullptr;


    GravityDirection selectedDirection = GravityDirection::YMinus;
    const float PIDIV180 = 0.017452f;    // PAI/180
};