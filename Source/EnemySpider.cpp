#include "EnemySpider.h"
#include "Graphics/Graphics.h"
#include "Mathf.h"
#include "Player.h"
#include "Collision.h"
#include <ProjectileHoming.h>
#include "ProjectileStraight.h"
#include "Input/Input.h"
#include "Input/GamePad.h"

//����//
//

EnemySpider::EnemySpider()
{
    model = new Model("Data/Model/spider/spider.mdl");

    //�X�P�[�����O
    scale.x = scale.y = scale.z = 0.4f;

    radius = 0.5f;

    height = 1.0f;

    health = 1;
    //sdd = 0;
    //EnemySpider::Instance().SetDeadcount(0);
    spiderdeadcount = 0;
    //�p�j�X�e�[�W�֑J��
    TransitionWanderState();
}

EnemySpider::~EnemySpider()
{
    delete model;
}
//void set_random_target();
//void draw_rect(float x, float y, float size);
//void update_enemy_position();





//�X�V����
void EnemySpider::Update(float elapsedTime)
{
    //�X�e�[�g���Ƃ̍X�V����
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

    // �G�̈ʒu���X�V
    //update_enemy_position();

    //���͏����X�V
    UpdateVelocity(elapsedTime);

    //���G���ԍX�V
    UpdateInvincibleTimer(elapsedTime);

    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();

    //���f���A�j���[�V�����X�V
    model->UpdateAnimation(elapsedTime);

    //���f���s����X�V
    model->UpdateTransform(transform);

    
    shottimer += 0.01f;
    if (shottimer >= 4.0f) 
    { shottimer = 0.0f; }
    projectileManager.Update(elapsedTime);
}




void EnemySpider::DrawDebugPrimitive()
{
    //���N���X�̃f�o�b�O�v���~�e�B�u�֐�
    Enemy::DrawDebugPrimitive();

    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //�꒣��͈͂��f�o�b�O�~���`��
    debugRenderer->DrawCylinder(territoryOrigin, territoryRange, 1.0f,
        DirectX::XMFLOAT4(0, 1, 0, 1));

    //�^�[�Q�b�g�ʒu���f�o�b�O���`��
    debugRenderer->DrawSphere(targetPosition, radius, DirectX::XMFLOAT4(1, 1, 0, 1));

    //���G�͈͂��f�o�b�O�~���`��
    debugRenderer->DrawSphere(position, searchRange, DirectX::XMFLOAT4(0, 0, 1, 1));

    //�U���͈͂��f�o�b�O�~���`��
    debugRenderer->DrawSphere(position, attackRange, DirectX::XMFLOAT4(1, 0, 0, 1));
}

void EnemySpider::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
    territoryOrigin = origin;
    territoryRange = range;
}

void EnemySpider::SetRandomTargetPosition()
{
    targetPosition.x = Mathf::RandomRange(territoryOrigin.x, territoryOrigin.x + territoryRange);
    targetPosition.y = Mathf::RandomRange(territoryOrigin.y, territoryOrigin.y + territoryRange);
    targetPosition.z = Mathf::RandomRange(territoryOrigin.z, territoryOrigin.z + territoryRange);

}

void EnemySpider::MoveToTarget(float elapsedTime, float speedRate)
{
    //�^�[�Q�b�g�����ւ̐i�s�x�N�g�����Z�o
    float vx = targetPosition.x - position.x;
    float vy = targetPosition.y - position.y;
    float vz = targetPosition.z - position.z;
    float dist = sqrtf(vx * vx + vz * vz);


    vx /= dist;
    vz /= dist;

    //�ړ�����
    Move(vx, vz, vy);
    Turn(elapsedTime, vx, vz, turnSpeed * speedRate);
}

bool EnemySpider::SearchPlayer()
{
    //�v���C���[�Ƃ̍��፷���l������3D�ł̋������������
    const DirectX::XMFLOAT3& playerPosition = Player::Instance().GetPosition();
    float vx = playerPosition.x - position.x;
    float vy = playerPosition.y - position.y;
    float vz = playerPosition.z - position.z;
    float dist = sqrtf(vx * vx + vy * vy + vz * vz);

    if (dist < searchRange)
    {
        float distXZ = sqrtf(vx * vx + vz * vz);

        //�P�ʃx�N�g����
        vx /= distXZ;
        vz /= distXZ;

        //�O���x�N�g��
        float frontX = sinf(angle.y);
        float frontZ = cosf(angle.y);

        //2�̃x�N�g���̓��ϒl�őO�㔻��
        float dot = (frontX * vx) + (frontZ * vz);
        if (dot > 0.0f)
        {
            return true;
        }
    }
    return false;
}

void EnemySpider::CollisionNodeVsPlayer(const char* nodeName, float nodeRadius)
{
    //�m�[�h�̈ʒu�Ɠ����蔻����s��
    Model::Node* node = model->FindNode(nodeName);

    if (node != nullptr)
    {
        //�m�[�h�̃��[���h���W
        DirectX::XMFLOAT3 nodePosition(
            node->worldTransform._41,
            node->worldTransform._42,
            node->worldTransform._43
        );

        //�����蔻��\��
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
            //�_���[�W��^����
            if (player.ApplyDamage(1))
            {
                //�G�𐁂���΂��x�N�g�����Z�o
                DirectX::XMFLOAT3 vec;
                vec.x = outPosition.x - nodePosition.x;
                vec.z = outPosition.z - nodePosition.z;
                float length = sqrtf(vec.x * vec.x + vec.z * vec.z);
                vec.x /= length;
                vec.z /= length;

                //XZ���ʂɐ�����΂��͂��|����
                float power = 1.0f;
                vec.x *= power;
                vec.z *= power;

                //Y�����ɂ��͂��|����
                vec.y = 1.0f;

                //������΂�
               // player.AddImpulse(vec);
            }
        }
    }
}

void EnemySpider::TransitionWanderState()
{
    //state = State::Wander;

    //�ڕW�n�_�ݒ�
    //SetRandomTargetPosition();

    //�����A�j���[�V�����Đ�
   // model->PlayAnimation(Anim_WalkFWD, true);
}

void EnemySpider::UpdateWanderState(float elapsedTime)
{
    //�ڕW�n�_�܂�XZ���ʂł̋�������
    float vx = targetPosition.x - position.x;
    float vz = targetPosition.z - position.z;
    float distSq = vx * vx + vz * vz;
    if (distSq < radius * radius)
    {
        //���̖ڕW�n�_�ݒ�
        //SetRandomTargetPosition();

        //�ҋ@�X�e�[�g�@��
        TransitionIdleState();
    }
    //�ڕW�n�_�ֈړ�
    MoveToTarget(elapsedTime, 0.5f);

    //�v���C���[���G
    if (SearchPlayer())
    {
        //����������ǐՃX�e�[�g�ɑJ��
        TransitionPursuitState();
    }

    //�e�۔���
    InputProjectile();
}

void EnemySpider::TransitionIdleState()
{
    state = State::Idle;

    //�^�C�}�[�������_���ݒ�
    stateTimer = Mathf::RandomRange(3.0f, 5.0f);

    //�ҋ@�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_IdleNormal, true);

    //�e�۔���
    InputProjectile();
}

void EnemySpider::UpdateIdleState(float elapsedTime)
{
    //�^�C�}�[����
    stateTimer -= elapsedTime;
    if (stateTimer < 0.0f)
    {
        //�p�j�X�e�[�g�֑J��
        TransitionWanderState();
    }

    //�v���C���[���G
    if (SearchPlayer())
    {
        //����������ǐՃX�e�[�g�ɑJ��
        TransitionPursuitState();
    }
    //�e�۔���
    //InputProjectile();
}

void EnemySpider::TransitionPursuitState()
{
    state = State::Pursuit;

    //���b�ԒǐՂ���^�C�}�[�������_���ݒ�
    stateTimer = Mathf::RandomRange(3.0f, 5.0f);

    //�����A�j���[�V�����Đ�
    model->PlayAnimation(Anim_RunFWD, true);
    //�e�۔���
    InputProjectile();
}

void EnemySpider::UpdatePursuitState(float elapsedTime)
{
    //�ڕW�ʒu���v���C���[�ʒu�ɐݒ�
    targetPosition = Player::Instance().GetPosition();

    //�ڕW�n�_�Ɉړ�
    MoveToTarget(elapsedTime, 1.0f);

    //�^�C�}�[����
    stateTimer -= elapsedTime;
    if (stateTimer < 0.0f)
    {
        //�ҋ@�X�e�[�g�ɑJ��
        TransitionIdleState();
        atknow = false;
    }

    //�v���C���[���߂Â��ƍU���X�e�[�g�ɑJ��
    float vx = targetPosition.x - position.x;
    float vy = targetPosition.y - position.y;
    float vz = targetPosition.z - position.z;
    float dist = sqrtf(vx * vx + vy * vy + vz * vz);


    if (dist <= attackRange)
    {

        atknow = true;
        //�U���X�e�[�g�ɑJ��
        TransitionAttackState();
    }
    else { atknow = false; }
    //�e�۔���
    //InputProjectile();
}

void EnemySpider::TransitionAttackState()
{
    state = State::Attack;




    //�U���A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Attack1, false);
}

void EnemySpider::UpdateAttackState(float elapsedTime)
{
    float animationTime = model->GetCurrentAnimationSeconds();
    if (animationTime >= 0.1f && animationTime < 0.35f)
    {
        //�ڋʃm�[�h�ƃv���C���[�̏Փˏ���
        CollisionNodeVsPlayer("EyeBall", 0.2f);
    }

    //�U���A�j���[�V�������I�������퓬�ҋ@�X�e�[�g�֑J��
    if (!model->IsPlayAnimation())
    {
        TransitionIdleBattleState();
    }
}

void EnemySpider::TransitionIdleBattleState()
{
    state = State::IdleBattle;

    //���b�ԑҋ@����^�C�}�[�������_���ݒ�
    stateTimer = Mathf::RandomRange(2.0f, 3.0f);

    //�퓬�ҋ@�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_IdleBattle, true);
}

void EnemySpider::UpdateIdleBattleState(float elapsedTime)
{
    //�ڕW�n�_���v���C���[�ʒu�ɐݒ�
    targetPosition = Player::Instance().GetPosition();

    //�^�C�}�[����
    stateTimer -= elapsedTime;
    if (stateTimer < 0.0f)
    {
        //�v���C���[���U���͈͂ɂ����ꍇ�͍U���X�e�[�g�ɑJ��
        float vx = targetPosition.x - position.x;
        float vy = targetPosition.y - position.y;
        float vz = targetPosition.z - position.z;
        float dist = sqrtf(vx * vx + vy * vy + vz * vz);
        if (dist < attackRange)
        {

            //�U���X�e�[�g�ɑJ��
            TransitionAttackState();
        }
        else
        {

            //�p�j�X�e�[�g�ɑJ��
            TransitionIdleState();
        }
    }

    MoveToTarget(elapsedTime, 0.0f);
}

void EnemySpider::TransitionDamageState()
{
    state = State::Damage;

    //�_���[�W�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_GetHit, false);
}

void EnemySpider::UpdateDamageState(float elapsedTime)
{
    //�_���[�W�A�j���[�V�������I�������퓬�ҋ@�X�e�[�g�ɑJ��
    if (!model->IsPlayAnimation())
    {
        TransitionIdleBattleState();
    }
}

void EnemySpider::TransitionDeathState()
{
    state = State::Death;

    //���S�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Die, false);
}

void EnemySpider::UpdateDeathState(float elapsedTime)
{
    //���S�A�j���[�V�������I������玩����j��
    if (!model->IsPlayAnimation())
    {
        Destroy();
    }
}

void EnemySpider::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

void EnemySpider::AddSpiderdeadcount()
{
    sdd = EnemySpider::Instance().GetDeadcount();
    sdd++;
    EnemySpider::Instance().SetDeadcount(sdd);
}


void EnemySpider::OnDead()
{
    /*int d = spiderdeadcount + 1;
    EnemySpider::Instance().SetDeadcount(d);*/
    AddSpiderdeadcount();
    TransitionDeathState();
}

void EnemySpider::OnDamaged()
{

    TransitionDamageState();
}


//�e�۔���
void EnemySpider::InputProjectile()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    //�����Ă�����ɔ���
    //if (shottimer>=3.9f)
    if (gamePad.GetButtonDown() & GamePad::BTN_X)
    {
        DirectX::XMFLOAT3 dir;
        dir.x = sinf(angle.y);
        dir.y = 0.0f;
        dir.z = cosf(angle.y);

        DirectX::XMStoreFloat3(&dir, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&dir)));

        DirectX::XMFLOAT3 pos;
        pos.x = position.x;
        pos.y = position.y + scale.y / 2;
        pos.z = position.z;

        ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
        projectile->Launch(dir, pos);
        projectileManager.Register(projectile);

    }
    //�z�[�~���O
    if (gamePad.GetButtonDown() & GamePad::BTN_Y)
    {
        DirectX::XMFLOAT3 dir;
        dir.x = sinf(angle.y);
        dir.y = 0.0f;
        dir.z = cosf(angle.y);

        DirectX::XMFLOAT3 pos;
        pos.x = position.x;
        pos.y = position.y + height / 2;
        pos.z = position.z;

        DirectX::XMFLOAT3 target;
        target.x = pos.x + dir.x * 1000.0f;
        target.y = pos.y + dir.y * 1000.0f;
        target.z = pos.z + dir.z * 1000.0f;

        float dist = FLT_MAX;
        // EnemyManager& enemyManager = EnemyManager::Instance();
        // int enemyCount = enemyManager.GetEnemyCount();

        // for (int i = 0; i < enemyCount; ++i)
        // {
        //     Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
        //     DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
        //     DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
        //     DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);
        //     DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(V);

        //     float d;
        //     DirectX::XMStoreFloat(&d, D);
        //     if (d < dist)
        //     {
        //         dist = d;
        //         target = enemy->GetPosition();
        //         target.y += enemy->GetHeight() * 0.5f;
        //     }
        // }

        ProjectileHoming* projectile = new ProjectileHoming(&projectileManager);
        projectile->Launch(dir, pos, target);
    }

}

void EnemySpider::UpdateVerticalVelocity(float elapsedFrame)
{
    //�d�͏���
    velocity.y += gravity * elapsedFrame;
}
