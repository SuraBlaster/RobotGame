#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"
#include "ProjectileManager.h"

//ドローン
class EnemyDrone : public Enemy
{
public:
    EnemyDrone();
    ~EnemyDrone() override;

    //更新処理
    void Update(float elapsedTime)override;

    //描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    //デバッグプリミティブ描画
    void DrawDebugPrimitive()override;

    //弾を発射する処理
    void LaunchProjectile();

    //縄張り設定
    void SetTerritory(const DirectX::XMFLOAT3& origin, float range);

    //弾とプレイヤーの当たり判定
    void CollisionProjectilesVsPlayer();

    ProjectileManager* GetProjectile() override { return &projectileManager; }

    //GUI
    void DrawDebugGUI()override;

private:
    //ターゲット位置をランダム設定
    void SetRandomTargetPosition();

    //目標地点へ移動
    void MoveToTarget(float elapsedTime, float speedrate);

    //プレイヤー索敵
    bool SearchPlayer();

    //徘徊ステート
    void TransitionWanderState();

    //徘徊ステート更新処理
    void UpdateWanderState(float elapsedTime);

    //待機ステートへ遷移
    void TransitionIdleState();

    //待機ステート更新処理
    void UpdateIdleState(float elapsedTime);

    //追跡ステートへ遷移
    void TransitionPursuitState();

    //追跡ステート更新処理
    void UpdatePursuitState(float elapsedTime);

    //攻撃ステートへ遷移
    void TransitionAttackState();

    //攻撃ステート更新処理
    void UpdateAttackState(float elapsedTime);

    //戦闘待機ステートに遷移
    void TransitionIdleBattleState();

    //戦闘待機ステート更新処理
    void UpdateIdleBattleState(float elapsedTime);

    //ダメージステートへ遷移
    void TransitionDamageState();

    //ダメージステート更新処理
    void UpdateDamageState(float elapsedTime);

    //死亡ステートへ遷移
    void TransitionDeathState();

    //死亡ステート更新処理
    void UpdateDeathState(float elapsedTime);

    void UpdateVerticalVelocity(float elapsedFrame);
private:
    //ステート
    enum class State
    {
        Wander,
        Idle,
        Pursuit,
        Attack,
        IdleBattle,
        Damage,
        Death,
    };

    //アニメーション
    enum Animation
    {
        Anim_Idle,
        Anim_Attack,
    };
protected:
    //死亡したときに呼ばれる
    void OnDead() override;

    //ダメージを受けた時に呼ばれる
    void OnDamaged()override;
private:
    Model* model = nullptr;
    State state = State::Wander;
    DirectX::XMFLOAT3 targetPosition = { 0,0,0 };
    DirectX::XMFLOAT3 territoryOrigin = { 0,0,0 };

    DirectX::XMFLOAT3 dir;
    DirectX::XMFLOAT3 pos;

    bool deadFlag;

    ProjectileManager projectileManager;
    float territoryRange = 10.0f;
    float moveSpeed = 1.0f;
    float turnSpeed = DirectX::XMConvertToRadians(360);
    float stateTimer = 0.0f;
    float searchRange = 7.0f;
    float attackRange = 5.0f;
    float syuziRange = 1.5f;
    float delay = 0.0f;
};

