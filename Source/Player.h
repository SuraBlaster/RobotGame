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

    //�ړ����͏���
    bool InputMove(float elapsedTime);

    //�U�����͏���
    bool InputAttack();

    void InputProjectile();

    void CollisionPlayerVsEnemies();

    //�ҋ@�X�e�[�g�֑J��
    void TransitionIdleState();

    //�ҋ@�X�e�[�g�X�V����
    void UpdateIdleState(float elapsedTime);

    //�ړ��X�e�[�g�֑J��
    void TransitionMoveState();

    //�ړ��X�e�[�g�X�V����
    void UpdateMoveState(float elapsedTime);

    //�W�����v�X�e�[�g�֑J��
    void TransitionJumpState();

    //�W�����v�X�e�[�g�X�V����
    void UpdateJumpState(float elapsedTime);

    //�U���X�e�[�g�ɑJ��
    void TransitionAttackState();

    //�U���X�e�[�g�X�V����
    void UpdateAttackState(float elapsedTime);

    //�_���[�W�X�e�[�g�ɑJ��
    void TransitionDamageState();

    //�_���[�W�X�e�[�g�X�V����
    void UpdateDamageState(float elapsedTime);

    //���S�X�e�[�g�ɑJ��
    void TransitionDeathState();

    //���S�X�e�[�g�X�V����
    void UpdateDeathState(float elapsedTime);

    //�o���A�W�J�X�e�[�g�ɑJ��
    void TransitionBarrierState();

    //�o���A�W�J�X�e�[�g�X�V����
    void UpdateBarrierState(float elapsedTime);

    //�m�[�h�ƃG�l�~�[�̏Փˏ���
    void CollisionNodeVsEnemies(const char* nodeName, float nodeRadius);

    //����̃A�^�b�`����
    void AttachWeapon();

protected:
    //�_���[�W���󂯂����ɌĂ΂��
    void OnDamaged()override;

    //���S�����Ƃ��ɌĂ΂��
    void OnDead()override;

public:
    Player();
    ~Player()override;

    //�C���X�^���X�擾
    static Player& Instance();

    //�X�V����
    void Update(float elapsedTime);

    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    //GUI�`��
    void DrawDebugGUI();

    //�f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive();

    //�W�����v���͏���
    bool InputJump();

    void CollisionprojectilesVsEnemies();

    //�o���A�X�V����
    void UpdateBarrier();

    void SetRimit(const int& rimit) { barrierRimit = rimit; }

    const int& GetRimit() const { return barrierRimit; }

    Model* GetModel() const { return model; }

    const bool GetAttackFlag() const { return attackCollisionFlag; }
private:
    //�A�j���[�V����
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

    //�o���A�Ŗh����c���
    int barrierRimit = 0;

    //���o���A���W�J����Ă��邩�ǂ���
    bool firstFlag = false;
};