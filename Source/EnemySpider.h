#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"

#include"ProjectileManager.h"


//スライム
class EnemySpider : public Enemy
{
public:
    EnemySpider();
    ~EnemySpider() override;

    static EnemySpider& Instance()
    {
        static EnemySpider instance;
        return instance;
    }

    //更新処理
    void Update(float elapsedTime)override;

    //描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    //デバッグプリミティブ描画
    void DrawDebugPrimitive()override;

    //縄張り設定
    void SetTerritory(const DirectX::XMFLOAT3& origin, float range);

    void LimitPosition();

    int GetDeadcount() { return spiderdeadcount; }
    void SetDeadcount(int spiderdeadcount) { this->spiderdeadcount = spiderdeadcount; }
private:
    //ターゲット位置をランダム設定
    void SetRandomTargetPosition();

    //目標地点へ移動
    void MoveToTarget(float elapsedTime, float speedrate);

    //プレイヤー索敵
    bool SearchPlayer();

    //ノードとプレイヤーの衝突処理
    void CollisionNodeVsPlayer(const char* nodeName, float boneRadius);

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
    //弾丸発射
    void InputProjectile();


    void UpdateVerticalVelocity(float elapsedFrame)override;

    void AddSpiderdeadcount();

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
        Anim_IdleNormal,
        Anim_IdleBattle,
        Anim_Attack1,
        Anim_Attack2,
        Anim_WalkFWD,
        Anim_WalkBWD,
        Anim_WalkLeft,
        Anim_WalfRight,
        Anim_RunFWD,
        Anim_SenseSomtingST,
        Anim_Taunt,
        Anim_Victory,
        Anim_GetHit,
        Anim_Dizzy,
        Anim_Die,
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
    float territoryRange = 0.0f;
    float moveSpeed = 3.0f;
    float turnSpeed = DirectX::XMConvertToRadians(360);
    float stateTimer = 0.0f;
    float searchRange = 10.0f;
    float attackRange = 1.0f;
    float syuziRange = 1.5f;
    bool atknow;
    float shottimer;

    int spiderdeadcount;

    ProjectileManager projectileManager;

    float delay = 0.0f;
    int sdd;
};
