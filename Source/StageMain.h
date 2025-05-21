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

    //更新処理
    void Update(float elapsedTime)override;

    //描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    //レイキャスト
    bool RayCast(const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end, HitResult& hit)override;

    void ChangeGravity();

    bool GetIsRotation() { return isRotationAnimation; }

    bool GetIsAnten() { return isAnten; }

    float GetAlpha() { return alpha; }
    Transform transform;

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
    DirectX::XMFLOAT3 nowAngle = {};


private:
    bool isRotationAnimation = false;
    bool isAnten = false;
    int antenTimer = 0;
    float alpha;

    Model* model = nullptr;


    GravityDirection selectedDirection = GravityDirection::YMinus;
    const float PIDIV180 = 0.017452f;    // PAI/180
};