#include "StageManager.h"

void StageManager::Update(float elapsedTime)
{
    for (Stage* stage : stages)
    {
        stage->Update(elapsedTime);
    }
}

void StageManager::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    for (Stage* stage : stages)
    {
        stage->Render(dc, shader);
    }
}

void StageManager::Register(Stage* stage)
{
    stages.emplace_back(stage);
}

void StageManager::Clear()
{
    for (Stage* stage : stages)
    {
        if (stage != nullptr) 
        {
            delete stage;
            stage = nullptr;
        }
    }
    stages.clear();
}

bool StageManager::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{

    bool result = false;
    float closestDistance = FLT_MAX;
    HitResult tempHit;

    DirectX::XMVECTOR startVec = DirectX::XMLoadFloat3(&start);

    for (Stage* stage : stages)
    {
        if (stage->RayCast(start, end, tempHit))
        {
            DirectX::XMVECTOR hitPointVec = DirectX::XMLoadFloat3(&tempHit.position);
            DirectX::XMVECTOR distanceVec = DirectX::XMVectorSubtract(hitPointVec, startVec);
            float distance = DirectX::XMVector3Length(distanceVec).m128_f32[0];

            if (distance < closestDistance)
            {
                closestDistance = distance;
                hit = tempHit;
                result = true;
            }
        }
    }


    return result;
}
