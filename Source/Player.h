#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "ProjectileManager.h"
#include "Effect.h"

class Player : public Character
{
private:
    DirectX::XMFLOAT3 GetMoveVec() const;

    //移動入力処理
    bool InputMove(float elapsedTime);

    //攻撃入力処理
    bool InputAttack();

    void InputProjectile();

    void CollisionPlayerVsEnemies();

    //待機ステートへ遷移
    void TransitionIdleState();

    //待機ステート更新処理
    void UpdateIdleState(float elapsedTime);

    //移動ステートへ遷移
    void TransitionMoveState();

    //移動ステート更新処理
    void UpdateMoveState(float elapsedTime);

    //ジャンプステートへ遷移
    void TransitionJumpState();

    //ジャンプステート更新処理
    void UpdateJumpState(float elapsedTime);

    //着地ステートへ遷移
    void TransitionLandState();

    //着地ステート更新処理
    void UpdateLandState(float elapsedTime);

    //攻撃ステートに遷移
    void TransitionAttackState();

    //攻撃ステート更新処理
    void UpdateAttackState(float elapsedTime);

    //ダメージステートに遷移
    void TransitionDamageState();

    //ダメージステート更新処理
    void UpdateDamageState(float elapsedTime);

    //死亡ステートに遷移
    void TransitionDeathState();

    //死亡ステート更新処理
    void UpdateDeathState(float elapsedTime);

    //復活ステートに遷移
    void TransitionReviveState();

    //復活ステート更新処理
    void UpdateReviveState(float elapsedTime);

    //ノードとエネミーの衝突処理
    void CollisionNodeVsEnemies(const char* nodeName, float nodeRadius);

protected:
    //ダメージを受けた時に呼ばれる
    void OnDamaged()override;

    //死亡したときに呼ばれる
    void OnDead()override;

public:
    Player();
    ~Player()override;

    //インスタンス取得
    static Player& Instance();

    //更新処理
    void Update(float elapsedTime);

    //描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    //GUI描画
    void DrawDebugGUI();

    //デバッグプリミティブ描画
    void DrawDebugPrimitive();

    //ジャンプ入力処理
    bool InputJump();

    //着地したときに呼ばれる
    void OnLanding() override;

    void CollisionprojectilesVsEnemies();

    
private:
    //アニメーション
    enum Animation
    {
        Anim_Attack,
        Anim_Death,
        Anim_Falling,
        Anim_GetHit1,
        Anim_GetHit2,
        Anim_Idle,
        Anim_Jump,
        Anim_Jump_Flip,
        Anim_Landing,
        Anim_Revive,
        Anim_Running,
        Anim_Walking,
    };

    enum class State
    {
        Idle,
        Move,
        Jump,
        Land,
        Attack,
        Damage,
        Death,
        Revive,
    };
private:
    ProjectileManager projectileManager;

    Model* model = nullptr;

    Effect* hitEffect = nullptr;

    bool attackCollisionFlag = false;

    float moveSpeed = 5.0f;

    float turnSpeed = DirectX::XMConvertToRadians(720);

    float jumpSpeed = 20.0f;

    int   jumpCount = 0;

    int   jumpLimit = 2;

    float leftHandRadius = 0.4f;

    State state = State::Idle;
};