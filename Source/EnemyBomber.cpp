#include "EnemyBomber.h"
#include "Graphics/Graphics.h"
#include "Mathf.h"
#include "Player.h"
#include "Collision.h"
#include <ShieldGauge.h>

EnemyBomber::EnemyBomber()
{
    model = new Model("Data/Model/Bomber/EnemyBomber.mdl");

    //�X�P�[�����O
    scale.x = scale.y = scale.z = 0.01f;

    radius = 0.5f;

    height = 1.0f;

    health = 1;
    explosionEffect = new Effect("Data/Effect/Explosion.efkefc");
    bomberdeadcount = 0;
    //�p�j�X�e�[�W�֑J��
    TransitionWanderState();
}

EnemyBomber::~EnemyBomber()
{
    delete model;

    delete explosionEffect;
}

//�X�V����
void EnemyBomber::Update(float elapsedTime)
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
    case State::Death:
        UpdateDeathState(elapsedTime);
        break;

    }
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

    SetRandomTargetPosition();
    delay -= elapsedTime;
}

void EnemyBomber::DrawDebugPrimitive()
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
    debugRenderer->DrawCylinder(position, searchRange, 1.0f, DirectX::XMFLOAT4(0, 0, 1, 1));

    //�U���͈͂��f�o�b�O�~���`��
    debugRenderer->DrawCylinder(position, ExplosionRadius, 1.0f, DirectX::XMFLOAT4(1, 0, 0, 1));
}

void EnemyBomber::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
    territoryOrigin = origin;
    territoryRange = range;
}



void EnemyBomber::SetRandomTargetPosition()
{
    targetPosition.x = Mathf::RandomRange(territoryOrigin.x, territoryOrigin.x + territoryRange);
    targetPosition.y = 0;
    targetPosition.z = Mathf::RandomRange(territoryOrigin.z, territoryOrigin.z + territoryRange);
    

}

void EnemyBomber::MoveToTarget(float elapsedTime, float speedRate)
{
    //�^�[�Q�b�g�����ւ̐i�s�x�N�g�����Z�o
    float vx = targetPosition.x - position.x;
    float vz = targetPosition.z - position.z;
    float dist = sqrtf(vx * vx + vz * vz);
    vx /= dist;
    vz /= dist;

    //�ړ�����
    Move(vx, vz, moveSpeed * speedRate);
    Turn(elapsedTime, vx, vz, turnSpeed * speedRate);
}

bool EnemyBomber::SearchPlayer()
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

void EnemyBomber::CollisionNodeVsPlayer(const char* nodeName, float nodeRadius)
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

        int playerTimer = player.GetTimer();

        if (Collision::IntersectSphereVsCylinder(
            nodePosition,
            nodeRadius,
            player.GetPosition(),
            player.GetRadius(),
            player.GetHeight(),
            outPosition))
        {
            if (playerTimer > 0 && delay <= 0.0f)
            {
                if (delay <= 0)
                {
                    player.SetHit(true);
                }

                delay = 0.1f;

                playerTimer -= 6.0f;
                player.SetTimer(playerTimer);


            }
            else if (delay <= 0.0f)
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
                    float power = 10.0f;
                    vec.x *= power;
                    vec.z *= power;

                    //Y�����ɂ��͂��|����
                    //vec.y = 5.0f;

                    //������΂�
                    //player.AddImpulse(vec);
                }
            }
        }

        //player.SetHit(false);
    }
}

void EnemyBomber::TransitionWanderState()
{
    state = State::Wander;

    //�ڕW�n�_�ݒ�
    SetRandomTargetPosition();

    //�����A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Walk, true);
}

void EnemyBomber::UpdateWanderState(float elapsedTime)
{
    //�ڕW�n�_�܂�XZ���ʂł̋�������
    float vx = targetPosition.x - position.x;
    float vz = targetPosition.z - position.z;
    float distSq = vx * vx + vz * vz;
    if (distSq < radius * radius)
    {
        //���̖ڕW�n�_�ݒ�
        SetRandomTargetPosition();

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
}

void EnemyBomber::TransitionIdleState()
{
    state = State::Idle;

    //�^�C�}�[�������_���ݒ�
    stateTimer = Mathf::RandomRange(3.0f, 5.0f);

    //�ҋ@�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Idle, true);
}

void EnemyBomber::UpdateIdleState(float elapsedTime)
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
}

void EnemyBomber::TransitionPursuitState()
{
    state = State::Pursuit;

    //��������܂ł̎���
    stateTimer = 10.0f;

    //�����A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Walk, true);
}

void EnemyBomber::UpdatePursuitState(float elapsedTime)
{
    //�ڕW�ʒu���v���C���[�ʒu�ɐݒ�
    targetPosition = Player::Instance().GetPosition();

    //�ڕW�n�_�Ɉړ�
    MoveToTarget(elapsedTime, 1.0f);

    //�^�C�}�[����
    stateTimer -= elapsedTime;
    if (stateTimer < 0.0f)
    {
        CollisionNodeVsPlayer("EyeBall", ExplosionRadius);
        explosionEffect->Play(position, 0.5f);
        TransitionDeathState();
    }
}

void EnemyBomber::TransitionDeathState()
{
    state = State::Death;

    //���S�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Attack, false);
    AddBomberdeadcount();
    model->PlayAnimation(Anim_Die, false);
    Destroy();
}

void EnemyBomber::UpdateDeathState(float elapsedTime)
{
    float animationTime = model->GetCurrentAnimationSeconds();
    if (animationTime > 0.57f)
    {
        CollisionNodeVsPlayer("Bomb", ExplosionRadius);
        explosionEffect->Play(position, 0.5f);
    }

    //CollisionNodeVsPlayer("EyeBall", ExplosionRadius);
    if (!model->IsPlayAnimation())
    {
        Destroy();
    }
}

void EnemyBomber::UpdateVerticalVelocity(float elapsedFrame)
{
    //�d�͏���
    velocity.y += gravity * elapsedFrame; 
}

void EnemyBomber::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);

    DrawDebugPrimitive();
}

void EnemyBomber::AddBomberdeadcount()
{
    bdd = EnemyBomber::Instance().GetDeadcount();
    bdd++;
    EnemyBomber::Instance().SetDeadcount(bdd);
}



void EnemyBomber::OnDead()
{
   /* int d = bomberdeadcount + 1;
    EnemyBomber::Instance().SetDeadcount(d);*/
    //AddBomberdeadcount();
    TransitionDeathState();
}




