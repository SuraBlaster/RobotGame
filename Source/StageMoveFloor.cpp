#include "StageMoveFloor.h"

StageMoveFloor::StageMoveFloor()
{
    scale.x = scale.z = 3.0f;
    scale.y = 0.5f;

    //ステージモデルを読み込み
    model = new Model("Data/Model/Cube/Cube.mdl");
}

StageMoveFloor::~StageMoveFloor()
{
    //ステージモデルを破棄
    delete model;
   
}

void StageMoveFloor::Update(float elapsedTime)
{
    //前回の情報を保存
    oldTransform = transform;
    oldAngle = angle;

    //スタートからゴールまでの距離を算出する
    DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR Goal = DirectX::XMLoadFloat3(&goal);
    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Goal, Start);
    DirectX::XMVECTOR Length = DirectX::XMVector3Length(Vec);

    float length;
    DirectX::XMStoreFloat(&length, Length);

    //スタートからゴールまでの間を1秒間で進む割合(0.0～1.0)を算出する
    float speed = moveSpeed * elapsedTime;
    float speedRate = speed / length;
    moveRate += speedRate;

    //ゴールに到達、またはスタートに戻った場合、移動方向を反転させる
    if (moveRate <= 0.0f || moveRate >= 1.0f)
    {
        moveSpeed = -moveSpeed;
    }

    //線形補完で位置を算出する
    DirectX::XMVECTOR Position = DirectX::XMVectorLerp(Start, Goal, moveRate);
    DirectX::XMStoreFloat3(&position, Position);

    //回転
    angle.x += torque.x * elapsedTime;
    angle.y += torque.y * elapsedTime;
    angle.z += torque.z * elapsedTime;

    //行列更新
    UpdateTransform();

    const DirectX::XMFLOAT4X4 transformIdentity = { 1,0,0,0 ,0,1,0,0 ,0,0,1,0 ,0,0,0,1 };
    model->UpdateTransform(transformIdentity);
}

//行列更新処理
void StageMoveFloor::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    model->UpdateTransform(transform);

    shader->Draw(dc, model);
}

//レイキャスト
bool StageMoveFloor::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    // 前回のワールド行列と逆行列を求める
    DirectX::XMMATRIX worldMatrix = DirectX::XMLoadFloat4x4(&oldTransform);
    DirectX::XMMATRIX invWorldMatrix = DirectX::XMMatrixInverse(nullptr, worldMatrix);

    // 前回のローカル空間でのレイに変換
    DirectX::XMVECTOR startVec = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR endVec = DirectX::XMLoadFloat3(&end);
    DirectX::XMVECTOR localStartVec = DirectX::XMVector3Transform(startVec, invWorldMatrix);
    DirectX::XMVECTOR localEndVec = DirectX::XMVector3Transform(endVec, invWorldMatrix);
   
    // ローカル空間でのレイとの交点を求める
    DirectX::XMFLOAT3 localStart, localEnd;
    DirectX::XMStoreFloat3(&localStart, localStartVec);
    DirectX::XMStoreFloat3(&localEnd, localEndVec);

    HitResult localHit;
    if (Collision::IntersectRayVsModel(localStart, localEnd, model, localHit))
    {
        // 前回のローカル空間から今回のワールド空間へ変換
        DirectX::XMVECTOR hitPointVec = DirectX::XMLoadFloat3(&localHit.position);
        DirectX::XMVECTOR worldHitPointVec = DirectX::XMVector3Transform(hitPointVec, DirectX::XMLoadFloat4x4(&transform));
        DirectX::XMStoreFloat3(&hit.position, worldHitPointVec);


        hit.rotation.x = angle.x - oldAngle.x;
        hit.rotation.y = angle.y - oldAngle.y;
        hit.rotation.z = angle.z - oldAngle.z;

        return true;
    }
    return false;

}

//行列更新処理
void StageMoveFloor::UpdateTransform()
{
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    DirectX::XMMATRIX W = S * R * T;
    DirectX::XMStoreFloat4x4(&transform, W);
}
