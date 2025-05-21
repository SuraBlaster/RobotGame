#include "EnemyBomber.h"
#include "Graphics/Graphics.h"
#include "Mathf.h"
#include "Player.h"
#include "Collision.h"
#include <ShieldGauge.h>

EnemyBomber::EnemyBomber()
{
    model = new Model("Data/Model/Bomber/EnemyBomber.mdl");

    //スケーリング
    scale.x = scale.y = scale.z = 0.01f;

    radius = 0.5f;

    height = 1.0f;

    health = 1;
    explosionEffect = new Effect("Data/Effect/Explosion.efkefc");
    bomberdeadcount = 0;
    //徘徊ステージへ遷移
    TransitionWanderState();
}

EnemyBomber::~EnemyBomber()
{
    delete model;

    delete explosionEffect;
}

//更新処理
void EnemyBomber::Update(float elapsedTime)
{
    //ステートごとの更新処理
    switch (state)
    {
    case State::Wander:
        UpdateWanderState(elapsedTime);
        break;
    case State::Idle:
        UpdateIdleState(elapsedTime);
        break;
    case State::Pursuit:
        UpdatePursuitState(elapsedTime);
        break;
    case State::Death:
        UpdateDeathState(elapsedTime);
        break;

    }
    //速力処理更新
    UpdateVelocity(elapsedTime);

    //無敵時間更新
    UpdateInvincibleTimer(elapsedTime);

    //オブジェクト行列を更新
    UpdateTransform();

    //モデルアニメーション更新
    model->UpdateAnimation(elapsedTime);

    //モデル行列を更新
    model->UpdateTransform(transform);

    SetRandomTargetPosition();
    delay -= elapsedTime;
}

void EnemyBomber::DrawDebugPrimitive()
{
    //基底クラスのデバッグプリミティブ関数
    Enemy::DrawDebugPrimitive();

    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //縄張り範囲をデバッグ円柱描画
    debugRenderer->DrawCylinder(territoryOrigin, territoryRange, 1.0f,
        DirectX::XMFLOAT4(0, 1, 0, 1));

    //ターゲット位置をデバッグ球描画
    debugRenderer->DrawSphere(targetPosition, radius, DirectX::XMFLOAT4(1, 1, 0, 1));

    //索敵範囲をデバッグ円柱描画
    debugRenderer->DrawCylinder(position, searchRange, 1.0f, DirectX::XMFLOAT4(0, 0, 1, 1));

    //攻撃範囲をデバッグ円柱描画
    debugRenderer->DrawCylinder(position, ExplosionRadius, 1.0f, DirectX::XMFLOAT4(1, 0, 0, 1));
}

void EnemyBomber::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
    territoryOrigin = origin;
    territoryRange = range;
}



void EnemyBomber::SetRandomTargetPosition()
{
    targetPosition.x = Mathf::RandomRange(territoryOrigin.x, territoryOrigin.x + territoryRange);
    targetPosition.y = 0;
    targetPosition.z = Mathf::RandomRange(territoryOrigin.z, territoryOrigin.z + territoryRange);
    

}

void EnemyBomber::MoveToTarget(float elapsedTime, float speedRate)
{
    //ターゲット方向への進行ベクトルを算出
    float vx = targetPosition.x - position.x;
    float vz = targetPosition.z - position.z;
    float dist = sqrtf(vx * vx + vz * vz);
    vx /= dist;
    vz /= dist;

    //移動処理
    Move(vx, vz, moveSpeed * speedRate);
    Turn(elapsedTime, vx, vz, turnSpeed * speedRate);
}

bool EnemyBomber::SearchPlayer()
{
    //プレイヤーとの高低差を考慮して3Dでの距離判定をする
    const DirectX::XMFLOAT3& playerPosition = Player::Instance().GetPosition();
    float vx = playerPosition.x - position.x;
    float vy = playerPosition.y - position.y;
    float vz = playerPosition.z - position.z;
    float dist = sqrtf(vx * vx + vy * vy + vz * vz);

    if (dist < searchRange)
    {
        float distXZ = sqrtf(vx * vx + vz * vz);

        //単位ベクトル化
        vx /= distXZ;
        vz /= distXZ;

        //前方ベクトル
        float frontX = sinf(angle.y);
        float frontZ = cosf(angle.y);

        //2つのベクトルの内積値で前後判定
        float dot = (frontX * vx) + (frontZ * vz);
        if (dot > 0.0f)
        {
            return true;
        }
    }
    return false;
}

void EnemyBomber::CollisionNodeVsPlayer(const char* nodeName, float nodeRadius)
{

    //ノードの位置と当たり判定を行う
    Model::Node* node = model->FindNode(nodeName);

    if (node != nullptr)
    {
        //ノードのワールド座標
        DirectX::XMFLOAT3 nodePosition(
            node->worldTransform._41,
            node->worldTransform._42,
            node->worldTransform._43
        );

        //当たり判定表示
        Graphics::Instance().GetDebugRenderer()->DrawSphere(
            nodePosition, nodeRadius, DirectX::XMFLOAT4(0, 0, 1, 1)
        );

        Player& player = Player::Instance();

        DirectX::XMFLOAT3 outPosition;

        int playerTimer = player.GetTimer();

        if (Collision::IntersectSphereVsCylinder(
            nodePosition,
            nodeRadius,
            player.GetPosition(),
            player.GetRadius(),
            player.GetHeight(),
            outPosition))
        {
            if (playerTimer > 0 && delay <= 0.0f)
            {
                if (delay <= 0)
                {
                    player.SetHit(true);
                }

                delay = 0.1f;

                playerTimer -= 6.0f;
                player.SetTimer(playerTimer);


            }
            else if (delay <= 0.0f)
            {
                //ダメージを与える
                if (player.ApplyDamage(1))
                {
                    //敵を吹っ飛ばすベクトルを算出
                    DirectX::XMFLOAT3 vec;
                    vec.x = outPosition.x - nodePosition.x;
                    vec.z = outPosition.z - nodePosition.z;
                    float length = sqrtf(vec.x * vec.x + vec.z * vec.z);
                    vec.x /= length;
                    vec.z /= length;

                    //XZ平面に吹っ飛ばす力を掛ける
                    float power = 10.0f;
                    vec.x *= power;
                    vec.z *= power;

                    //Y方向にも力を掛ける
                    //vec.y = 5.0f;

                    //吹っ飛ばす
                    //player.AddImpulse(vec);
                }
            }
        }

        //player.SetHit(false);
    }
}

void EnemyBomber::TransitionWanderState()
{
    state = State::Wander;

    //目標地点設定
    SetRandomTargetPosition();

    //歩きアニメーション再生
    model->PlayAnimation(Anim_Walk, true);
}

void EnemyBomber::UpdateWanderState(float elapsedTime)
{
    //目標地点までXZ平面での距離判定
    float vx = targetPosition.x - position.x;
    float vz = targetPosition.z - position.z;
    float distSq = vx * vx + vz * vz;
    if (distSq < radius * radius)
    {
        //次の目標地点設定
        SetRandomTargetPosition();

        //待機ステート繊維
        TransitionIdleState();
    }
    //目標地点へ移動
    MoveToTarget(elapsedTime, 0.5f);

    //プレイヤー索敵
    if (SearchPlayer())
    {
        //見つかったら追跡ステートに遷移
        TransitionPursuitState();
    }
}

void EnemyBomber::TransitionIdleState()
{
    state = State::Idle;

    //タイマーをランダム設定
    stateTimer = Mathf::RandomRange(3.0f, 5.0f);

    //待機アニメーション再生
    model->PlayAnimation(Anim_Idle, true);
}

void EnemyBomber::UpdateIdleState(float elapsedTime)
{
    //タイマー処理
    stateTimer -= elapsedTime;
    if (stateTimer < 0.0f)
    {
        //徘徊ステートへ遷移
        TransitionWanderState();
    }

    //プレイヤー索敵
    if (SearchPlayer())
    {
        //見つかったら追跡ステートに遷移
        TransitionPursuitState();
    }
}

void EnemyBomber::TransitionPursuitState()
{
    state = State::Pursuit;

    //爆発するまでの時間
    stateTimer = 10.0f;

    //歩きアニメーション再生
    model->PlayAnimation(Anim_Walk, true);
}

void EnemyBomber::UpdatePursuitState(float elapsedTime)
{
    //目標位置をプレイヤー位置に設定
    targetPosition = Player::Instance().GetPosition();

    //目標地点に移動
    MoveToTarget(elapsedTime, 1.0f);

    //タイマー処理
    stateTimer -= elapsedTime;
    if (stateTimer < 0.0f)
    {
        CollisionNodeVsPlayer("EyeBall", ExplosionRadius);
        explosionEffect->Play(position, 0.5f);
        TransitionDeathState();
    }
}

void EnemyBomber::TransitionDeathState()
{
    state = State::Death;

    //死亡アニメーション再生
    model->PlayAnimation(Anim_Attack, false);
    AddBomberdeadcount();
    model->PlayAnimation(Anim_Die, false);
    Destroy();
}

void EnemyBomber::UpdateDeathState(float elapsedTime)
{
    float animationTime = model->GetCurrentAnimationSeconds();
    if (animationTime > 0.57f)
    {
        CollisionNodeVsPlayer("Bomb", ExplosionRadius);
        explosionEffect->Play(position, 0.5f);
    }

    //CollisionNodeVsPlayer("EyeBall", ExplosionRadius);
    if (!model->IsPlayAnimation())
    {
        Destroy();
    }
}

void EnemyBomber::UpdateVerticalVelocity(float elapsedFrame)
{
    //重力処理
    velocity.y += gravity * elapsedFrame; 
}

void EnemyBomber::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);

    DrawDebugPrimitive();
}

void EnemyBomber::AddBomberdeadcount()
{
    bdd = EnemyBomber::Instance().GetDeadcount();
    bdd++;
    EnemyBomber::Instance().SetDeadcount(bdd);
}



void EnemyBomber::OnDead()
{
   /* int d = bomberdeadcount + 1;
    EnemyBomber::Instance().SetDeadcount(d);*/
    //AddBomberdeadcount();
    TransitionDeathState();
}




