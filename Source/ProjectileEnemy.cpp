#include "ProjectileEnemy.h"
#include "ProjectileManager.h"
#include "Graphics/Graphics.h"

ProjectileEnemy::ProjectileEnemy(ProjectileManager* manager)
    :manager(manager)
{
    manager->RegisterEnemy(this);
}

void ProjectileEnemy::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
}

void ProjectileEnemy::Destroy()
{
    manager->RemoveEnemy(this);
}

void ProjectileEnemy::UpdateTransform()
{
    DirectX::XMVECTOR Front, Up, Right;

    Front = DirectX::XMLoadFloat3(&direction);
    Front = DirectX::XMVector3Normalize(Front);

    Up = DirectX::XMVectorSet(0.001f, 1, 0, 0);
    Up = DirectX::XMVector3Normalize(Up);

    Right = DirectX::XMVector3Cross(Up, Front);
    Right = DirectX::XMVector3Normalize(Right);

    Up = DirectX::XMVector3Cross(Front, Right);
    Up = DirectX::XMVector3Normalize(Up);

    DirectX::XMFLOAT3 right, up, front;
    DirectX::XMStoreFloat3(&right, Right);
    DirectX::XMStoreFloat3(&up, Up);
    DirectX::XMStoreFloat3(&front, Front);

    transform._11 = right.x * scale.x;
    transform._12 = right.y * scale.x;
    transform._13 = right.z * scale.x;
    transform._14 = 0.0f;
    transform._21 = up.x * scale.y;
    transform._22 = up.y * scale.y;
    transform._23 = up.z * scale.y;
    transform._24 = 0.0f;
    transform._31 = front.x * scale.z;
    transform._32 = front.y * scale.z;
    transform._33 = front.z * scale.z;
    transform._34 = 0.0f;
    transform._41 = position.x;
    transform._42 = position.y;
    transform._43 = position.z;
    transform._44 = 1.0f;

    this->direction = front;
}