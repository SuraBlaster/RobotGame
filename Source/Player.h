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

    //バリア展開ステートに遷移
    void TransitionBarrierState();

    //バリア展開ステート更新処理
    void UpdateBarrierState(float elapsedTime);

    //ノードとエネミーの衝突処理
    void CollisionNodeVsEnemies(const char* nodeName, float nodeRadius);

    //武器のアタッチ処理
    void AttachWeapon();

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

    void CollisionprojectilesVsEnemies();

    //バリア更新処理
    void UpdateBarrier();

    void SetRimit(const int& rimit) { barrierRimit = rimit; }

    const int& GetRimit() const { return barrierRimit; }

    Model* GetModel() const { return model; }

    const bool GetAttackFlag() const { return attackCollisionFlag; }
private:
    //アニメーション
    enum Animation
    {
        Dagger_Idle,
        Dagger_Run,
        Dagger_Shield,
        Dagger_Jump,
        Dagger_Attack,
        Dagger_Damage,
        Dagger_Death,
        GreatSword_Idle,
        GreatSword_Run,
        GreatSword_Shield,
        GreatSword_Jump,
        GreatSword_Attack,
        GreatSword_Damage,
        GreatSword_Death,
        
    };

    enum class State
    {
        Idle,
        Move,
        Jump,
        Attack,
        Damage,
        Death,
        Barrier,
    };

private:
    ProjectileManager projectileManager;

    Model* model = nullptr;

    Effect* hitEffect = nullptr;

    Effect* barrierFirst = nullptr;

    Effect* barrier = nullptr;

    Effekseer::Handle barrierEffectHandle = -1;

    bool attackCollisionFlag = false;

    float moveSpeed = 5.0f;

    float turnSpeed = DirectX::XMConvertToRadians(720);

    float jumpSpeed = 20.0f;

    int   jumpCount = 0;

    int   jumpLimit = 2;

    float leftHandRadius = 0.4f;

    State state = State::Idle;

    //バリアで防げる残り回数
    int barrierRimit = 0;

    //今バリアが展開されているかどうか
    bool firstFlag = false;
};