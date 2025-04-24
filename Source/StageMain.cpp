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
//    DirectX::XMVECTOR oldGravity = DirectX::XMLoadFloat4(&transform.rotation);
//    DirectX::XMVECTOR newGravity = GetGravityVector(selectedDirection);
//
//    DirectX::XMVECTOR rotationAxis = DirectX::XMVector3Cross(oldGravity, newGravity);
//    float dot = DirectX::XMVectorGetX(DirectX::XMVector3Dot(oldGravity, newGravity));
//    dot = max(min(dot, 1.0f), -1.0f); // 安全のためClamp
//
//    //// 180度反転の場合はクロス積がゼロになるので補正が必要
//    //if (DirectX::XMVector3Equal(rotationAxis, DirectX::XMVectorZero())) {
//    //    // 重力が完全に逆（例: Down→Up）なので、適当な垂直軸で180度回転
//    //    DirectX::XMVECTOR fallbackAxis = DirectX::XMVectorSet(1, 0, 0, 0); // Y軸に近ければX軸とか
//    //    rotationAxis = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(oldGravity, fallbackAxis));
//    //}
//
//    float angle = acosf(dot);
//    DirectX::XMVECTOR rotationQuat = DirectX::XMQuaternionRotationAxis(DirectX::XMVector3Normalize(rotationAxis), angle);
//
//
//    // ステージ回転
//    DirectX::XMVECTOR stageQuat = oldGravity;
//
//    // 合成して新しい姿勢へ
//    DirectX::XMVECTOR targetStageQuat = DirectX::XMQuaternionMultiply(rotationQuat, stageQuat);
//    targetStageQuat = DirectX::XMQuaternionNormalize(targetStageQuat);
//
//    // 補間：徐々に回す（例: 1秒かけて回転）
//    timer = std::clamp(elapsedTime / rotateDuration, 0.0f, 1.0f);
//    DirectX::XMStoreFloat4(&transform.rotation, DirectX::XMQuaternionSlerp(stageQuat, targetStageQuat, timer));

    DirectX::XMVECTOR up, front, right;

    up  = DirectX::XMLoadFloat3(&transform.getUp());
    front = DirectX::XMLoadFloat3(&transform.getFront());
    right = DirectX::XMLoadFloat3(&transform.getRight());
    DirectX::XMVECTOR Rotation = DirectX::XMLoadFloat4(&transform.rotation);

    switch (selectedDirection) {
    case Down:
    {
        return;
    }
    case West:
    {
        DirectX::XMVECTOR q = DirectX::XMQuaternionRotationAxis(front, 3 * PIDIV180 * elapsedTime);
        Rotation = DirectX::XMQuaternionMultiply(Rotation, q);
    }
    case East:
    {
        DirectX::XMVECTOR q = DirectX::XMQuaternionRotationAxis(front, -3 * PIDIV180 * elapsedTime);
        Rotation = DirectX::XMQuaternionMultiply(Rotation, q);
    }
    case North:
    {
        DirectX::XMVECTOR q = DirectX::XMQuaternionRotationAxis(right, -3 * PIDIV180 * elapsedTime);
        Rotation = DirectX::XMQuaternionMultiply(Rotation, q);
    }
    case South:
    {
        DirectX::XMVECTOR q = DirectX::XMQuaternionRotationAxis(right, 3 * PIDIV180 * elapsedTime);
        Rotation = DirectX::XMQuaternionMultiply(Rotation, q);
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

    //if (normal.x >= 0.8f)
    //{
    //    selectedDirection = West;
    //}
    //else if (normal.x <= -0.8f)
    //{
    //    selectedDirection = East;
    //}

    //if (normal.y >= 0.8f)
    //{
    //    selectedDirection = Down;
    //}
    //else if (normal.y <= -0.8f)
    //{
    //    selectedDirection = Up;
    //}

    //if (normal.z >= 0.8f)
    //{
    //    selectedDirection = South;
    //}
    //else if (normal.z <= -0.8f)
    //{
    //    selectedDirection = East;
    //}
}

