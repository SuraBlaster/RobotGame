#include "StageMain.h"
#include <algorithm>
#include "Camera.h"

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
    DirectX::XMVECTOR up, front, right;

    up = DirectX::XMLoadFloat3(&transform.getUp());
    front = DirectX::XMLoadFloat3(&transform.getFront());
    right = DirectX::XMLoadFloat3(&transform.getRight());
    DirectX::XMVECTOR Rotation = DirectX::XMLoadFloat4(&transform.rotation);
    DirectX::XMFLOAT3 angle = transform.getAngle();
    constexpr float radian90 = DirectX::XMConvertToRadians(90);
    const float rotationSpeed = 40.0f;

    switch (selectedDirection) {
    case Down:
    {
        nowAngle = angle;
        return;
    }
    case West:
    {
        float rotationVelocity = rotationSpeed * PIDIV180 * elapsedTime;
        if (angle.z + rotationVelocity >= radian90)
            rotationVelocity = radian90 - angle.z;

        DirectX::XMVECTOR q = DirectX::XMQuaternionRotationAxis(front, rotationVelocity);
        Rotation = DirectX::XMQuaternionMultiply(Rotation, q);
        ;
        if (std::abs(nowAngle.z - angle.z) >= radian90)
        {
            selectedDirection = Down;
            isRotationAnimation = false;
        }
        break;
    }
    case East:
    {
        float rotationVelocity = -rotationSpeed * PIDIV180 * elapsedTime;
        if (angle.z - rotationVelocity <= radian90)
            rotationVelocity = radian90 + angle.z;

        DirectX::XMVECTOR q = DirectX::XMQuaternionRotationAxis(front, rotationVelocity);
        Rotation = DirectX::XMQuaternionMultiply(Rotation, q);
        if (std::abs(nowAngle.z - angle.z) >= radian90)
        {
            selectedDirection = Down;
            isRotationAnimation = false;
        }
        break;
    }
    case North:
    {
        float rotationVelocity = rotationSpeed * PIDIV180 * elapsedTime;
        if (angle.x + rotationVelocity >= radian90)
            rotationVelocity = radian90 - angle.x;

        DirectX::XMVECTOR q = DirectX::XMQuaternionRotationAxis(right, rotationVelocity);
        Rotation = DirectX::XMQuaternionMultiply(Rotation, q);
        if (std::abs(nowAngle.x - angle.x) >= radian90)
        {
            selectedDirection = Down;
            isRotationAnimation = false;
        }
        break;
    }
    case South:
    {
        float rotationVelocity = -rotationSpeed * PIDIV180 * elapsedTime;
        if (angle.x - rotationVelocity <= radian90)
            rotationVelocity = radian90 + angle.x;

        DirectX::XMVECTOR q = DirectX::XMQuaternionRotationAxis(right, rotationVelocity);
        Rotation = DirectX::XMQuaternionMultiply(Rotation, q);
        if (std::abs(nowAngle.x - angle.x) >= radian90)
        {
            selectedDirection = Down;
            isRotationAnimation = false;
        }
        break;
    }
    }

    DirectX::XMStoreFloat4(&transform.rotation, Rotation);
}

void StageMain::ChangeGravity()
{
    if (isRotationAnimation) return;

    Camera& camera = Camera::Instance();
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&camera.GetFront())));

    if (std::abs(front.x) < std::abs(front.z))
    {
        if (front.z > 0.0f)
        {
            selectedDirection = North;
        }
        else
        {
            selectedDirection = South;
        }
    }
    else
    {
        if (front.x > 0.0f)
        {
            selectedDirection = East;
        }
        else
        {
            selectedDirection = West;
        }
    }

    if (selectedDirection != Down)
        isRotationAnimation = true;
}

