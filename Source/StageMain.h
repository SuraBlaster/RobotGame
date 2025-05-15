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

    //çXêVèàóù
    void Update(float elapsedTime)override;

    //ï`âÊèàóù
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    void ChangeGravity();

    bool GetIsRotation() { return isRotationAnimation; }
private:

    enum GravityDirection {
        Down,
        West,
        East,
        North,
        South
    };

    void RotationStage(float elapsedTime);
    Transform transform;
    DirectX::XMFLOAT3 nowAngle = {};
private:
    bool isRotationAnimation = false;
    Model* model = nullptr;

    float rotateDuration = 3.0f;

    GravityDirection selectedDirection = GravityDirection::Down;
    const float PIDIV180 = 0.017452f;    // PAI/180
};