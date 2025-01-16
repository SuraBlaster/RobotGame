#include "EnemySlime.h"
#include "Graphics/Graphics.h"
#include "Mathf.h"
#include "Player.h"
#include "Collision.h"

EnemySlime::EnemySlime()
{
    model = new Model("Data/Model/Slime/Slime.mdl");

    //スケーリング
    scale.x = scale.y = scale.z = 0.01f;

    radius = 0.5f;

    height = 1.0f;

    //徘徊ステージへ遷移
    TransitionWanderState();
}

EnemySlime::~EnemySlime()
{
    delete model;
}

//更新処理
void EnemySlime::Update(float elapsedTime)
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
    case State::Attack:
        UpdateAttackState(elapsedTime);
        break;
    case State::IdleBattle:
        UpdateIdleBattleState(elapsedTime);
        break;
    case State::Damage:
        UpdateDamageState(elapsedTime);
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
}

void EnemySlime::DrawDebugPrimitive()
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
    debugRenderer->DrawCylinder(position, attackRange, 1.0f, DirectX::XMFLOAT4(1, 0, 0, 1));
}

void EnemySlime::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
    territoryOrigin = origin;
    territoryRange = range;
}

void EnemySlime::SetRandomTargetPosition()
{
    targetPosition.x = Mathf::RandomRange(territoryOrigin.x, territoryOrigin.x + territoryRange);
    targetPosition.y = 0;
    targetPosition.z = Mathf::RandomRange(territoryOrigin.z, territoryOrigin.z + territoryRange);

}

void EnemySlime::MoveToTarget(float elapsedTime, float speedRate)
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

bool EnemySlime::SearchPlayer()
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

void EnemySlime::CollisionNodeVsPlayer(const char* nodeName, float nodeRadius)
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
        if (Collision::IntersectSphereVsCylinder(
            nodePosition,
            nodeRadius,
            player.GetPosition(),
            player.GetRadius(),
            player.GetHeight(),
            outPosition))
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
                vec.y = 5.0f;

                //吹っ飛ばす
                player.AddImpulse(vec);
            }
        }
    }
}

void EnemySlime::TransitionWanderState()
{
    state = State::Wander;

    //目標地点設定
    SetRandomTargetPosition();

    //歩きアニメーション再生
    model->PlayAnimation(Anim_WalkFWD, true);
}

void EnemySlime::UpdateWanderState(float elapsedTime)
{
    //目標地点までXZ平面での距離判定
    float vx = targetPosition.x - position.x;
    float vz = targetPosition.z - position.z;
    float distSq = vx * vx + vz * vz;
    if (distSq < radius * radius)
    {
        //次の目標地点設定
        //SetRandomTargetPosition();

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

void EnemySlime::TransitionIdleState()
{
    state = State::Idle;

    //タイマーをランダム設定
    stateTimer = Mathf::RandomRange(3.0f, 5.0f);

    //待機アニメーション再生
    model->PlayAnimation(Anim_IdleNormal, true);
}

void EnemySlime::UpdateIdleState(float elapsedTime)
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

void EnemySlime::TransitionPursuitState()
{
    state = State::Pursuit;

    //数秒間追跡するタイマーをランダム設定
    stateTimer = Mathf::RandomRange(3.0f, 5.0f);

    //歩きアニメーション再生
    model->PlayAnimation(Anim_RunFWD, true);
}

void EnemySlime::UpdatePursuitState(float elapsedTime)
{
    //目標位置をプレイヤー位置に設定
    targetPosition = Player::Instance().GetPosition();

    //目標地点に移動
    MoveToTarget(elapsedTime, 1.0f);

    //タイマー処理
    stateTimer -= elapsedTime;
    if (stateTimer < 0.0f)
    {
        //待機ステートに遷移
        TransitionIdleState();
    }

    //プレイヤーが近づくと攻撃ステートに遷移
    float vx = targetPosition.x - position.x;
    float vy = targetPosition.y - position.y;
    float vz = targetPosition.z - position.z;
    float dist = sqrtf(vx * vx + vy * vy + vz * vz);
    if (dist < attackRange)
    {
        //攻撃ステートに遷移
        TransitionAttackState();
    }
}

void EnemySlime::TransitionAttackState()
{
    state = State::Attack;

    //攻撃アニメーション再生
    model->PlayAnimation(Anim_Attack1, false);
}

void EnemySlime::UpdateAttackState(float elapsedTime)
{
    float animationTime = model->GetCurrentAnimationSeconds();
    if (animationTime >= 0.1f && animationTime < 0.35f)
    {
        //目玉ノードとプレイヤーの衝突処理
        CollisionNodeVsPlayer("EyeBall", 0.2f);
    }

    //攻撃アニメーションが終わったら戦闘待機ステートへ遷移
    if (!model->IsPlayAnimation())
    {
        TransitionIdleBattleState();
    }
}

void EnemySlime::TransitionIdleBattleState()
{
    state = State::IdleBattle;

    //数秒間待機するタイマーをランダム設定
    stateTimer = Mathf::RandomRange(2.0f, 3.0f);

    //戦闘待機アニメーション再生
    model->PlayAnimation(Anim_IdleBattle, true);
}

void EnemySlime::UpdateIdleBattleState(float elapsedTime)
{
    //目標地点をプレイヤー位置に設定
    targetPosition = Player::Instance().GetPosition();

    //タイマー処理
    stateTimer -= elapsedTime;
    if (stateTimer < 0.0f)
    {
        //プレイヤーが攻撃範囲にいた場合は攻撃ステートに遷移
        float vx = targetPosition.x - position.x;
        float vy = targetPosition.y - position.y;
        float vz = targetPosition.z - position.z;
        float dist = sqrtf(vx * vx + vy * vy + vz * vz);
        if (dist < attackRange)
        {
            //攻撃ステートに遷移
            TransitionAttackState();
        }
        else
        {
            //徘徊ステートに遷移
            TransitionIdleState();
        }
    }

    MoveToTarget(elapsedTime, 0.0f);
}

void EnemySlime::TransitionDamageState()
{
    state = State::Damage;

    //ダメージアニメーション再生
    model->PlayAnimation(Anim_GetHit, false);
}

void EnemySlime::UpdateDamageState(float elapsedTime)
{
    //ダメージアニメーションが終わったら戦闘待機ステートに遷移
    if (!model->IsPlayAnimation())
    {
        TransitionIdleBattleState();
    }
}

void EnemySlime::TransitionDeathState()
{
    state = State::Death;

    //死亡アニメーション再生
    model->PlayAnimation(Anim_Die, false);
}

void EnemySlime::UpdateDeathState(float elapsedTime)
{
    //死亡アニメーションが終わったら自分を破棄
    if (!model->IsPlayAnimation())
    {
        Destroy();
    }
}

void EnemySlime::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

void EnemySlime::OnDead()
{
    TransitionDeathState();
}

void EnemySlime::OnDamaged()
{
    TransitionDamageState();
}



