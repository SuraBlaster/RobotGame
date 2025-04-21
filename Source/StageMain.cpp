#include "StageMain.h"
#include <algorithm>



//コンストラクタ
StageMain::StageMain()
{
    model = new Model("Data/Model/ExampleStage/ExampleStage.mdl");
    currentGravityDir = GetGravityVector(GravityDirection::Gravity_Down);
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
    //やること名塩
}

void StageMain::Render(ID3D11DeviceContext* dc, Shader* shader)
{
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
    case Up:        return DirectX::XMVectorSet(0, 1, 0, 0);
    case West:      return DirectX::XMVectorSet(-1, 0, 0, 0);
    case East:      return DirectX::XMVectorSet(1, 0, 0, 0);
    case North:     return DirectX::XMVectorSet(0, 0, 1, 0);
    case South:     return DirectX::XMVectorSet(0, 0, -1, 0);
    }
    return DirectX::XMVectorSet(0, -1, 0, 0);
}

void StageMain::RotationStage(float elapsedTime)
{
    DirectX::XMVECTOR oldGravity = currentGravityDir;
    DirectX::XMVECTOR newGravity = GetGravityVector(selectedDirection);

    DirectX::XMVECTOR rotationAxis = DirectX::XMVector3Cross(oldGravity, newGravity);
    float dot = DirectX::XMVectorGetX(DirectX::XMVector3Dot(oldGravity, newGravity));
    dot = max(min(dot, 1.0f), -1.0f); // 安全のためClamp

    // 180度反転の場合はクロス積がゼロになるので補正が必要
    if (DirectX::XMVector3Equal(rotationAxis, DirectX::XMVectorZero())) {
        // 重力が完全に逆（例: Down→Up）なので、適当な垂直軸で180度回転
        DirectX::XMVECTOR fallbackAxis = DirectX::XMVectorSet(1, 0, 0, 0); // Y軸に近ければX軸とか
        rotationAxis = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(oldGravity, fallbackAxis));
    }

    float angle = acosf(dot);
    DirectX::XMVECTOR rotationQuat = DirectX::XMQuaternionRotationAxis(DirectX::XMVector3Normalize(rotationAxis), angle);


    // ステージ回転
    DirectX::XMVECTOR stageQuat = currentStageRotation;

    // 合成して新しい姿勢へ
    DirectX::XMVECTOR targetStageQuat = DirectX::XMQuaternionMultiply(rotationQuat, stageQuat);
    targetStageQuat = DirectX::XMQuaternionNormalize(targetStageQuat);

    // 補間：徐々に回す（例: 1秒かけて回転）
    float t = std::clamp(elapsedTime / rotateDuration, 0.0f, 1.0f);
    currentStageRotation = DirectX::XMQuaternionSlerp(stageQuat, targetStageQuat, t);
}

void StageMain::ChangeGravity(HitResult hit)
{
    DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);
    DirectX::XMFLOAT3 normal;
    DirectX::XMStoreFloat3(&normal, DirectX::XMVector3Normalize(Normal));

    if (normal.x >= 0.8f)
    {
        selectedDirection = East;
    }
    else if (normal.x <= -0.8f)
    {
        selectedDirection = West;
    }

    if (normal.y >= 0.8f)
    {
        selectedDirection = Up;
    }
    else if (normal.y <= -0.8f)
    {
        selectedDirection = Down;
    }

    if (normal.z >= 0.8f)
    {
        selectedDirection = North;
    }
    else if (normal.z <= -0.8f)
    {
        selectedDirection = South;
    }
}

