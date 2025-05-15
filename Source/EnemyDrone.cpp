#include "EnemyDrone.h"

EnemyDrone::EnemyDrone()
{
}

EnemyDrone::~EnemyDrone()
{
}

void EnemyDrone::Update(float elapsedTime)
{
}

void EnemyDrone::Render(ID3D11DeviceContext* dc, Shader* shader)
{
}

void EnemyDrone::DrawDebugPrimitive()
{
}

void EnemyDrone::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
}

void EnemyDrone::UpdateDeathState(float elapsedTime)
{
}


void EnemyDrone::SetRandomTargetPosition()
{
}

void EnemyDrone::MoveToTarget(float elapsedTime, float speedrate)
{
}

bool EnemyDrone::SearchPlayer()
{
    return false;
}

void EnemyDrone::CollisionNodeVsPlayer(const char* nodeName, float boneRadius)
{
}

void EnemyDrone::TransitionWanderState()
{
}

void EnemyDrone::UpdateWanderState(float elapsedTime)
{
}

void EnemyDrone::TransitionIdleState()
{
}

void EnemyDrone::UpdateIdleState(float elapsedTime)
{
}

void EnemyDrone::TransitionPursuitState()
{
}

void EnemyDrone::UpdatePursuitState(float elapsedTime)
{
}

void EnemyDrone::TransitionAttackState()
{
}

void EnemyDrone::UpdateAttackState(float elapsedTime)
{
}

void EnemyDrone::TransitionIdleBattleState()
{
}

void EnemyDrone::UpdateIdleBattleState(float elapsedTime)
{
}

void EnemyDrone::TransitionDamageState()
{
}

void EnemyDrone::UpdateDamageState(float elapsedTime)
{
}

void EnemyDrone::TransitionDeathState()
{
    
}

void EnemyDrone::OnDead()
{
    TransitionDeathState();
}

void EnemyDrone::OnDamaged()
{
    TransitionDamageState();
}
