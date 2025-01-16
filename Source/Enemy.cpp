#include "Enemy.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"

EnemyManager* manager{};

void Enemy::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //判定衝突用のデバッグ球を描画
    debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
    
    //判定衝突用のデバッグ円柱を描画
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

}

void Enemy::Destroy()
{
    manager->Instance().Remove(this);
}
