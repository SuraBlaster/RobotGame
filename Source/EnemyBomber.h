#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"
#include "Effect.h"

//�X���C��
class EnemyBomber : public Enemy
{
public:
    EnemyBomber();
    ~EnemyBomber() override;

    static EnemyBomber& Instance()
    {
        static EnemyBomber instance;
        return instance;
    }

    //�X�V����
    void Update(float elapsedTime)override;

    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    //�f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive()override;

    //�꒣��ݒ�
    void SetTerritory(const DirectX::XMFLOAT3& origin, float range);

    int GetDeadcount() { return bomberdeadcount; }
    void SetDeadcount(int bomberdeadcount) { this->bomberdeadcount = bomberdeadcount; }
private:
    //�^�[�Q�b�g�ʒu�������_���ݒ�
    void SetRandomTargetPosition();

    //�ڕW�n�_�ֈړ�
    void MoveToTarget(float elapsedTime, float speedrate);

    //�v���C���[���G
    bool SearchPlayer();

    //�m�[�h�ƃv���C���[�̏Փˏ���
    void CollisionNodeVsPlayer(const char* nodeName, float boneRadius);

    //�p�j�X�e�[�g�֑J��
    void TransitionWanderState();

    //�p�j�X�e�[�g�X�V����
    void UpdateWanderState(float elapsedTime);

    //�ҋ@�X�e�[�g�֑J��
    void TransitionIdleState();

    //�ҋ@�X�e�[�g�X�V����
    void UpdateIdleState(float elapsedTime);

    //�ǐՃX�e�[�g�֑J��
    void TransitionPursuitState();

    //�ǐՃX�e�[�g�X�V����
    void UpdatePursuitState(float elapsedTime);

    //�_���[�W�X�e�[�g�X�V����
    void UpdateDamageState(float elapsedTime);

    //���S�X�e�[�g�֑J��
    void TransitionDeathState();

    //���S�X�e�[�g�X�V����
    void UpdateDeathState(float elapsedTime);

    void AddBomberdeadcount();
private:
    //�X�e�[�g
    enum class State
    {
        Wander,
        Idle,
        Pursuit,
        Damage,
        Death,
    };

    //�A�j���[�V����
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
    //���S�����Ƃ��ɌĂ΂��
    void OnDead() override;
private:
    Model* model = nullptr;
    Effect* explosionEffect = nullptr;
    State state = State::Wander;
    DirectX::XMFLOAT3 targetPosition = { 0,0,0 };
    DirectX::XMFLOAT3 territoryOrigin = { 0,0,0 };
    float territoryRange = 10.0f;
    float moveSpeed = 3.0f;
    float turnSpeed = DirectX::XMConvertToRadians(360);
    float stateTimer = 0.0f;
    float searchRange = 5.0f;
    float attackRange = 1.5f;

    int bomberdeadcount;
    float delay = 0.0f;
    float ExplosionRadius = 3.0f;
    int bdd;
};
