#include "EnemyDrone.h"
#include <Graphics/DebugRenderer.h>
#include "Graphics/Graphics.h"
#include <Mathf.h>
#include <Player.h>
#include <Collision.h>
#include <ProjectileEnemyToPlayer.h>

EnemyDrone::EnemyDrone()
{
    model = new Model("Data/Model/Drone/Drone.mdl");

    //スケーリング
    scale.x = scale.y = scale.z = 0.001f;

    radius = 0.5f;

    height = 1.0f;

    dronedeadcount = 0;
    ddd = 0;
    //徘徊ステージへ遷移
    TransitionIdleState();
}

EnemyDrone::~EnemyDrone()
{
    delete model;
}

void EnemyDrone::Update(float elapsedTime)
{
    Player& player = Player::Instance();
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

    //弾更新
    projectileManager.Update(elapsedTime);

    CollisionProjectilesVsPlayer();

    //モデルアニメーション更新
    model->UpdateAnimation(elapsedTime);

    //モデル行列を更新
    model->UpdateTransform(transform);

    UpdatePlayerPosition(player.GetPosition());

    delay -= elapsedTime;
}



void EnemyDrone::DrawDebugPrimitive()
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
    debugRenderer->DrawSphere(position, searchRange, DirectX::XMFLOAT4(0, 0, 1, 1));

    //攻撃範囲をデバッグ円柱描画
    debugRenderer->DrawSphere(position, attackRange, DirectX::XMFLOAT4(1, 0, 0, 1));
}

void EnemyDrone::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
    territoryOrigin = origin;
    territoryRange = range;
}


void EnemyDrone::SetRandomTargetPosition()
{
    targetPosition.x = Mathf::RandomRange(territoryOrigin.x, territoryOrigin.x + territoryRange);
    targetPosition.y = Mathf::RandomRange(territoryOrigin.y, territoryOrigin.y + territoryRange);
    targetPosition.z = Mathf::RandomRange(territoryOrigin.z, territoryOrigin.z + territoryRange);
}

void EnemyDrone::MoveToTarget(float elapsedTime, float speedRate)
{
    //ターゲット方向への進行ベクトルを算出
    float vx = targetPosition.x - position.x;
    float vy = targetPosition.y - position.y;
    float vz = targetPosition.z - position.z;
    float dist = sqrtf(vx * vx + vz * vz);


    vx /= dist;
    vz /= dist;

    //移動処理
    Move(vx, vz, moveSpeed * speedRate);
    Turn(elapsedTime, vx, vz, turnSpeed * speedRate);
}

bool EnemyDrone::SearchPlayer()
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

void EnemyDrone::TransitionWanderState()
{
    state = State::Wander;

    //目標地点設定
    //SetRandomTargetPosition();

    //歩きアニメーション再生
    model->PlayAnimation(Anim_Attack, true);
}

void EnemyDrone::UpdateWanderState(float elapsedTime)
{
    //目標地点までXZ平面での距離判定
    float vx = targetPosition.x - position.x;
    float vz = targetPosition.z - position.z;
    float distSq = vx * vx + vz * vz;
    if (distSq < radius * radius)
    {
        //次の目標地点設定
        SetRandomTargetPosition();

        //待機ステート遷移
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

void EnemyDrone::TransitionIdleState()
{
    state = State::Idle;

    //タイマーをランダム設定
    stateTimer = Mathf::RandomRange(3.0f, 4.0f);

    //待機アニメーション再生
    model->PlayAnimation(Anim_Attack, false);
}

void EnemyDrone::UpdateIdleState(float elapsedTime)
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

void EnemyDrone::TransitionPursuitState()
{
    state = State::Pursuit;

    //数秒間追跡するタイマーをランダム設定
    stateTimer = Mathf::RandomRange(3.0f, 5.0f);

    //移動アニメーション再生
    model->PlayAnimation(Anim_Attack, false);
}

void EnemyDrone::UpdatePursuitState(float elapsedTime)
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
    float vz = targetPosition.z - position.z;
    float dist = sqrtf(vx * vx + vz * vz);

    if (dist <= attackRange)
    {
        //攻撃ステートに遷移
        TransitionAttackState();
    }
}

void EnemyDrone::TransitionAttackState()
{
    state = State::Attack;

    //攻撃アニメーション再生
    model->PlayAnimation(Anim_Attack, false);

    projectilecount = 0;
}

void EnemyDrone::UpdateAttackState(float elapsedTime)
{
    float animationTime = model->GetCurrentAnimationSeconds();
    if (animationTime >= 0.1f && animationTime < 0.35f)
    {
        if (projectilecount <= 1)
        {
            LaunchProjectile();
        }
        
    }

    //攻撃アニメーションが終わったら戦闘待機ステートへ遷移
    if (!model->IsPlayAnimation())
    {
        TransitionIdleBattleState();
    }
}

void EnemyDrone::LaunchProjectile()
{
    Player& player = Player::Instance();

    // ★ 3フレーム前の位置に向かうようになる
    DirectX::XMStoreFloat3(&dir,
        DirectX::XMVector3Normalize(
            DirectX::XMVectorSubtract(
                DirectX::XMLoadFloat3(&GetPreviousPlayerPos(3)),
                DirectX::XMLoadFloat3(&position)
            )
        )
    );

    pos.x = position.x;
    pos.y = position.y + Enemy::GetHeight() / 2;
    pos.z = position.z;

    ProjectileEnemyToPlayer* projectileEnemyToPlayer = new ProjectileEnemyToPlayer(&projectileManager);
    if (projectilecount <=1)
    {
        projectileEnemyToPlayer->Launch(dir, pos);
        projectilecount++;
    }
    
}

void EnemyDrone::TransitionIdleBattleState()
{
    state = State::IdleBattle;

    //数秒間待機するタイマーをランダム設定
    stateTimer = Mathf::RandomRange(5.0f, 6.0f);

    //戦闘待機アニメーション再生
    model->PlayAnimation(Anim_Attack, true);
}

void EnemyDrone::UpdateIdleBattleState(float elapsedTime)
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

void EnemyDrone::TransitionDamageState()
{
    state = State::Damage;

    //ダメージアニメーション再生
    model->PlayAnimation(Anim_Attack, false);
}

void EnemyDrone::UpdateDamageState(float elapsedTime)
{
    //ダメージアニメーションが終わったら戦闘待機ステートに遷移
    if (!model->IsPlayAnimation())
    {
        TransitionIdleBattleState();
    }
}

void EnemyDrone::TransitionDeathState()
{
    state = State::Death;

    stateTimer = 3.0f;

    //死亡アニメーション再生
    model->PlayAnimation(Anim_Idle, true);
}

void EnemyDrone::UpdateDeathState(float elapsedTime)
{
    stateTimer -= elapsedTime;
    if (stateTimer < 0.0f)
    {
        Destroy();
    }
}
void EnemyDrone::Adddeadcount()
{
    ddd = EnemyDrone::Instance().GetDeadcount();
    ddd++;
    EnemyDrone::Instance().SetDeadcount(ddd);
}

void EnemyDrone::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);

    projectileManager.Render(dc, shader);
}

void EnemyDrone::OnDead()
{
    Adddeadcount();
    TransitionDeathState();
    deadFlag = true;
}

void EnemyDrone::OnDamaged()
{
    TransitionDamageState();
}

void EnemyDrone::UpdateVerticalVelocity(float elapsedFrame)
{
    Player& player = Player::Instance();
    float playery = player.GetPosition().y;
    //重力処理
    if (deadFlag)
    {
        velocity.y += gravity * (elapsedFrame / 30);
    }
    else
    {
        if (playery + 3.0f > position.y)
        {
            velocity.y -= gravity * (elapsedFrame / 30);
        }
        else if(player.GetPosition().y - position.y > 2.0f)
        {
            //velocity.y += gravity;
        }
        else
        {
            velocity.y = 0.0f;
        }
    }
    
}

void EnemyDrone::UpdatePlayerPosition(const DirectX::XMFLOAT3& newPos)
{
    positionHistory.push_back(newPos);
    if (positionHistory.size() > kHistoryLimit) {
        positionHistory.pop_front();
    }
}

DirectX::XMFLOAT3 EnemyDrone::GetPreviousPlayerPos(int offset)
{
    if (positionHistory.size() > offset) {
        return positionHistory[positionHistory.size() - 1 - offset];
    }
    else if (!positionHistory.empty()) {
        // 履歴が足りない場合は最新位置を返す
        return positionHistory.back();
    }
    else {
        // 何も記録されていない場合のデフォルト
        return { 0.0f, 0.0f, 0.0f };
    }
}

void EnemyDrone::CollisionProjectilesVsPlayer()
{
    //総当たり処理
    int projectileCount = projectileManager.GetProjectileEnemyCount();
    for (int i = 0; i < projectileCount; ++i)
    {
        ProjectileEnemy* projectile = projectileManager.GetProjectileEnemy(i);

        Player& player = Player::Instance();
        //衝突処理
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectSphereVsCylinder(
            projectile->GetPosition(),
            projectile->GetRadius(),
            player.GetPosition(),
            player.GetRadius(),
            player.GetHeight(),
            outPosition
        ))
        {
            if (player.ApplyDamage(1))
            {
                {
                    DirectX::XMFLOAT3 impulse{};

                    const float power = 10.0f;
                    const DirectX::XMFLOAT3& e = player.GetPosition();
                    const DirectX::XMFLOAT3& p = projectile->GetPosition();
                    float vx = e.x - p.x;
                    float vz = e.z - p.z;
                    float lengthXZ = sqrtf(vx * vx + vz * vz);
                    vx /= lengthXZ;
                    vz /= lengthXZ;

                    impulse.x = vx * power;
                    impulse.y = power;
                    impulse.z = vz * power;

                    //player.AddImpulse(impulse);
                }


                projectile->Destroy();
            }

            {
                DirectX::XMFLOAT3 e = player.GetPosition();
                e.y += player.GetHeight() * 0.5f;
            }
        }

        //player.SetHit(false);
    }
}

void EnemyDrone::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("EnemyDrone", nullptr, ImGuiWindowFlags_None))
    {
        if (ImGui::CollapsingHeader("State", ImGuiTreeNodeFlags_DefaultOpen))
        {
            switch (state)
            {
            case State::Idle:       ImGui::LabelText("State", "Idle"); break;
            case State::Wander:     ImGui::LabelText("State", "Wander"); break;
            case State::Pursuit:    ImGui::LabelText("State", "Pursuit"); break;
            case State::Attack:     ImGui::LabelText("State", "Attack"); break;
            case State::IdleBattle:    ImGui::LabelText("State", "IdleBattle"); break;
            case State::Damage:    ImGui::LabelText("State", "Damage"); break;
            case State::Death:     ImGui::LabelText("State", "Death"); break;
            }

        }

        ImGui::InputFloat3("Position", &position.x);

        DirectX::XMFLOAT3 a;
        a.x = DirectX::XMConvertToDegrees(angle.x);
        a.y = DirectX::XMConvertToDegrees(angle.y);
        a.z = DirectX::XMConvertToDegrees(angle.z);

        ImGui::InputFloat3("Angle", &a.x);
        angle.x = DirectX::XMConvertToRadians(a.x);
        angle.y = DirectX::XMConvertToRadians(a.y);
        angle.z = DirectX::XMConvertToRadians(a.z);

        ImGui::InputFloat3("Velocity", &velocity.x);

        ImGui::InputFloat3("Scale", &scale.x);
    }


    ImGui::End();
}

