#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"
#include "ProjectileManager.h"

//�h���[��
class EnemyDrone : public Enemy
{
public:
    EnemyDrone();
    ~EnemyDrone() override;

    //�X�V����
    void Update(float elapsedTime)override;

    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    //�f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive()override;

    //�e�𔭎˂��鏈��
    void LaunchProjectile();

    //�꒣��ݒ�
    void SetTerritory(const DirectX::XMFLOAT3& origin, float range);

    //�e�ƃv���C���[�̓����蔻��
    void CollisionProjectilesVsPlayer();

    ProjectileManager* GetProjectile() override { return &projectileManager; }

    //GUI
    void DrawDebugGUI()override;

private:
    //�^�[�Q�b�g�ʒu�������_���ݒ�
    void SetRandomTargetPosition();

    //�ڕW�n�_�ֈړ�
    void MoveToTarget(float elapsedTime, float speedrate);

    //�v���C���[���G
    bool SearchPlayer();

    //�p�j�X�e�[�g
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

    //�U���X�e�[�g�֑J��
    void TransitionAttackState();

    //�U���X�e�[�g�X�V����
    void UpdateAttackState(float elapsedTime);

    //�퓬�ҋ@�X�e�[�g�ɑJ��
    void TransitionIdleBattleState();

    //�퓬�ҋ@�X�e�[�g�X�V����
    void UpdateIdleBattleState(float elapsedTime);

    //�_���[�W�X�e�[�g�֑J��
    void TransitionDamageState();

    //�_���[�W�X�e�[�g�X�V����
    void UpdateDamageState(float elapsedTime);

    //���S�X�e�[�g�֑J��
    void TransitionDeathState();

    //���S�X�e�[�g�X�V����
    void UpdateDeathState(float elapsedTime);

    void UpdateVerticalVelocity(float elapsedFrame);
private:
    //�X�e�[�g
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

    //�A�j���[�V����
    enum Animation
    {
        Anim_Idle,
        Anim_Attack,
    };
protected:
    //���S�����Ƃ��ɌĂ΂��
    void OnDead() override;

    //�_���[�W���󂯂����ɌĂ΂��
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

