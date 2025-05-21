#include "StageMain.h"
#include <algorithm>
#include "Camera.h"
#include "Input/Mouse.h"
#include "Input/Input.h"
#include "Player.h"

static StageMain* instance = nullptr;

//コンストラクタ
StageMain::StageMain()
{
    instance = this;
    model = new Model("Data/Model/ExampleStage/testmap.mdl");
    transform.scale.x *= 1.5f;
    transform.scale.y *= 1.5f;
    transform.scale.z *= 1.5f;
    /*transform.scale.x *= 0.01f;
    transform.scale.y *= 0.01f;
    transform.scale.z *= 0.01f;*/
}

//デストラクタ
StageMain::~StageMain()
{
    //ステージモデルを破棄
    delete model;
}

StageMain& StageMain::Instance()
{
    return *instance;
}

//更新処理
void StageMain::Update(float elapsedTime)
{
    transform.updateMatrix();
    //if (selectedDirection != Down)
    Mouse& mouse = Input::Instance().GetMouse();
    if (mouse.GetWheel() > 0.0f)
    {
        StageMain::Instance().ChangeGravity();
    }

    RotationStage(elapsedTime);
    //const DirectX::XMFLOAT4X4 transformIdentity = { 1,0,0,0 ,0,1,0,0 ,0,0,1,0 ,0,0,0,1 };
    model->UpdateTransform(transform.getMatrix());
}

void StageMain::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //model->UpdateTransform(transform.getMatrix());

    transform.drawDebugGUI("StageMain");

    //シェーダーにモデルを描画してもらう
    shader->Draw(dc, model);
}

bool StageMain::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}

void StageMain::RotationStage(float elapsedTime)
{
    DirectX::XMVECTOR front, right;

    front = DirectX::XMLoadFloat3(&transform.getFront());
    right = DirectX::XMLoadFloat3(&transform.getRight());
    DirectX::XMVECTOR Rotation = DirectX::XMLoadFloat4(&transform.rotation);
    DirectX::XMFLOAT3 angle = transform.getAngle();
    constexpr float radian90 = DirectX::XMConvertToRadians(90);
    constexpr float radian89 = DirectX::XMConvertToRadians(89);
    const float rotationSpeed = 40.0f;
    //DirectX::XMVECTOR q = DirectX::XMQuaternionRotationRollPitchYaw(angle.x, angle.y, angle.z);

    switch (selectedDirection) {
    case YMinus:
    {
        Rotation = DirectX::XMQuaternionRotationRollPitchYaw(0, 0, 0);
    }
    case YPuls:
    {
        Rotation = DirectX::XMQuaternionRotationRollPitchYaw(0, 180, 0);

    }
    case XMinus:
    {
        Rotation = DirectX::XMQuaternionRotationRollPitchYaw(0, 0, -90);

    }
    case XPuls:
    {
        Rotation = DirectX::XMQuaternionRotationRollPitchYaw(0, 0, 90);

    }
    case ZMinus:
    {
        Rotation = DirectX::XMQuaternionRotationRollPitchYaw(-90, 0, 0);

    }
    case ZPuls:
    {
        Rotation = DirectX::XMQuaternionRotationRollPitchYaw(90, 0, 0);

    }
    }

    DirectX::XMStoreFloat4(&transform.rotation, Rotation);
}

void StageMain::ChangeGravity()
{
    using namespace DirectX;
    if (isRotationAnimation) return;

    Camera& camera = Camera::Instance();
    XMFLOAT3 front;
   XMStoreFloat3(&front, XMVector3Normalize(XMLoadFloat3(&camera.GetFront())));

    XMFLOAT3 start = camera.GetEye();
    XMFLOAT3 end = camera.GetEye();
    end.z = front.z * 100;
    HitResult hit;

    if (RayCast(start, end, hit))
    {
        XMFLOAT3 vec;
        XMStoreFloat3(&vec, XMVectorSubtract(XMLoadFloat3(&hit.position), XMLoadFloat3(&transform.position)));

        DirectX::XMFLOAT3 absVec;
        absVec.x = std::fabs(vec.x);
        absVec.y = std::fabs(vec.y);
        absVec.z = std::fabs(vec.z);

        float maxAbs = absVec.x;
        int maxIndex = 0;
        float originalValue = vec.x;

        if (absVec.y > maxAbs)
        {
            maxAbs = absVec.y;
            maxIndex = 1;
            originalValue = vec.y;
        }
        else if (absVec.z > maxAbs)
        {
            maxAbs = absVec.z;
            maxIndex = 2;
            originalValue = vec.z;
        }

        if (originalValue - maxAbs == 0.0f)
        {
            maxIndex += 3;
        }

        switch (maxIndex)
        {
        case 0: selectedDirection = XPuls; break;
        case 1: selectedDirection = YPuls; break;
        case 2: selectedDirection = ZPuls; break;
        case 3: selectedDirection = XMinus; break;
        case 4: selectedDirection = YMinus; break;
        case 5: selectedDirection = ZMinus; break;
        }
    }
}
