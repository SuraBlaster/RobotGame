#include "StageMain.h"
#include <algorithm>
#include "Camera.h"

//コンストラクタ
StageMain::StageMain()
{
    model = new Model("Data/Model/ExampleStage/ExampleStage.mdl");
}

//デストラクタ
StageMain::~StageMain()
{
    //ステージモデルを破棄
    delete model;
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
    model->UpdateTransform(transform.getMatrix());

    transform.drawDebugGUI("StageMain");

    //シェーダーにモデルを描画してもらう
    shader->Draw(dc, model);
}

bool StageMain::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}

DirectX::XMVECTOR StageMain::GetGravityVector(GravityDirection dir)
{
    switch (dir) {
    case Down:      return DirectX::XMVectorSet(0, -1, 0, 0);
    case West:      return DirectX::XMVectorSet(-1, 0, 0, 0);
    case East:      return DirectX::XMVectorSet(1, 0, 0, 0);
    case North:     return DirectX::XMVectorSet(0, 0, 1, 0);
    case South:     return DirectX::XMVectorSet(0, 0, -1, 0);
    }
    return DirectX::XMVectorSet(0, -1, 0, 0);
}

void StageMain::RotationStage(float elapsedTime)
{
    DirectX::XMVECTOR up, front, right;

    up  = DirectX::XMLoadFloat3(&transform.getUp());
    front = DirectX::XMLoadFloat3(&transform.getFront());
    right = DirectX::XMLoadFloat3(&transform.getRight());
    DirectX::XMVECTOR Rotation = DirectX::XMLoadFloat4(&transform.rotation);
    const float ragian90 = DirectX::XMConvertToRadians(90);

    switch (selectedDirection) {
    case Down:
    {
        nowAngle = transform.getAngle();
        return;
    }
    case West:
    {
        float rotationVelocity = 20 * PIDIV180 * elapsedTime;
        if (transform.getAngle().z + rotationVelocity >= ragian90)
            rotationVelocity = ragian90 - transform.getAngle().z;

        DirectX::XMVECTOR q = DirectX::XMQuaternionRotationAxis(front, rotationVelocity);
        Rotation = DirectX::XMQuaternionMultiply(Rotation, q);
        ;
        if (std::abs(nowAngle.z - transform.getAngle().z) >= ragian90)
            selectedDirection = Down;
        break;
    }
    case East:
    {
        float rotationVelocity = -20 * PIDIV180 * elapsedTime;
        if (transform.getAngle().z - rotationVelocity >= ragian90)
            rotationVelocity = ragian90 - transform.getAngle().z;

        DirectX::XMVECTOR q = DirectX::XMQuaternionRotationAxis(front, rotationVelocity);
        Rotation = DirectX::XMQuaternionMultiply(Rotation, q);
        if (std::abs(nowAngle.z - transform.getAngle().z) >= ragian90)
            selectedDirection = Down;
        break;
    }
    case North:
    {
        float rotationVelocity = 20 * PIDIV180 * elapsedTime;
        if (transform.getAngle().x + rotationVelocity >= ragian90)
            rotationVelocity = ragian90 - transform.getAngle().x;

        DirectX::XMVECTOR q = DirectX::XMQuaternionRotationAxis(right, rotationVelocity);
        Rotation = DirectX::XMQuaternionMultiply(Rotation, q);
        if (std::abs(nowAngle.x - transform.getAngle().x ) >= ragian90)
            selectedDirection = Down;
        break;
    }
    case South:
    {
        float rotationVelocity = -20 * PIDIV180 * elapsedTime;
        if (transform.getAngle().x - rotationVelocity >= ragian90)
            rotationVelocity = ragian90 - transform.getAngle().x;

        DirectX::XMVECTOR q = DirectX::XMQuaternionRotationAxis(right, rotationVelocity);
        Rotation = DirectX::XMQuaternionMultiply(Rotation, q);
        if (std::abs(nowAngle.x - transform.getAngle().x) >= ragian90)
            selectedDirection = Down;
        break;
    }
    }

    DirectX::XMStoreFloat4(&transform.rotation, Rotation);
}

void StageMain::ChangeGravity(HitResult hit)
{
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
}

