#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "ProjectileManager.h"
#include "Effect.h"
#include "SceneManager.h"

#include"Audio/Audio.h"
#include"Audio/AudioSource.h"
class Player : public Character
{
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
    void UpdateBarrier(float elapsedTime);

    void SetRimit(const int& rimit) { barrierRimit = rimit; }

    const int& GetRimit() const { return barrierRimit; }

    Model* GetModel() const { return model; }

    const bool GetAttackFlag() const { return attackCollisionFlag; }
    bool GetOnDamage() { return onDamage; }
    
private:
    DirectX::XMFLOAT3 GetMoveVec() const;

    //移動入力処理
    bool InputMove(float elapsedTime);

    //剣攻撃時の移動入力処理
    bool InputMoveSword(float elapsedTime);

    //攻撃入力処理
    bool InputAttack();

    //void InputProjectile();

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

protected:
    //ダメージを受けた時に呼ばれる
    void OnDamaged()override;

    //死亡したときに呼ばれる
    void OnDead()override;

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

public:

    enum class WeaponType
    {
        GreatSword,
        Dagger,
    };

    WeaponType weapon;

    //�������
    void ChangeWeapon();

    WeaponType GetWeapon() const { return weapon; }

    float GetTimer() const { return ShieldTimer; }

    void SetTimer(const float timer) { ShieldTimer = timer; }

    bool GetHit() const { return hit; }

    void SetHit(bool hit) { this->hit = hit; }

    int GetShieldCount() const { return ShieldCount; }

    void SetShieldCount(int shieldCount) { this->ShieldCount = shieldCount; }


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

    bool onDamage = false;

    State state = State::Idle;

    //���o���A���W�J����Ă��邩�ǂ���
    bool ShieldFlag = false;

    float ShieldTimer = 0;

    bool hit = false;

    int attackStage = 0;

    int ShieldCount = 0;

    //バリアで防げる残り回数
    int barrierRimit = 0;

    //今バリアが展開されているかどうか
    bool firstFlag = false;

    //SE 
    std::unique_ptr<AudioSource> AttackSE;
    std::unique_ptr<AudioSource> Finish_AttackSE;
    std::unique_ptr<AudioSource> KnifeSE;
    std::unique_ptr<AudioSource> Knife_FinishSE;
    std::unique_ptr<AudioSource> Knife_Finish2SE;
    std::unique_ptr<AudioSource> ChangeSE;
    std::unique_ptr<AudioSource> ChieldSE;
    std::unique_ptr<AudioSource> Damage_ChieldSE;
    std::unique_ptr<AudioSource> Breaking_ChieldSE;
    std::unique_ptr<AudioSource>Damage_PlayerSE;
    std::unique_ptr<AudioSource> Player_DeathSE;
};