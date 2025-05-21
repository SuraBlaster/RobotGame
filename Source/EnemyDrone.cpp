#include "EnemyDrone.h"
#include <Graphics/DebugRenderer.h>
#include "Graphics/Graphics.h"
#include <Mathf.h>
#include <Player.h>
#include <Collision.h>
#include <ProjectileEnemyToPlayer.h>

EnemyDrone::EnemyDrone()
{
    model = new Model("Data/Model/Drone/Drone.mdl");

    //�X�P�[�����O
    scale.x = scale.y = scale.z = 0.001f;

    radius = 0.5f;

    height = 1.0f;

    dronedeadcount = 0;
    ddd = 0;
    //�p�j�X�e�[�W�֑J��
    TransitionIdleState();
}

EnemyDrone::~EnemyDrone()
{
    delete model;
}

void EnemyDrone::Update(float elapsedTime)
{
    Player& player = Player::Instance();
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

    //���͏����X�V
    UpdateVelocity(elapsedTime);

    //���G���ԍX�V
    UpdateInvincibleTimer(elapsedTime);

    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();

    //�e�X�V
    projectileManager.Update(elapsedTime);

    CollisionProjectilesVsPlayer();

    //���f���A�j���[�V�����X�V
    model->UpdateAnimation(elapsedTime);

    //���f���s����X�V
    model->UpdateTransform(transform);

    UpdatePlayerPosition(player.GetPosition());

    delay -= elapsedTime;
}



void EnemyDrone::DrawDebugPrimitive()
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

void EnemyDrone::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
    territoryOrigin = origin;
    territoryRange = range;
}


void EnemyDrone::SetRandomTargetPosition()
{
    targetPosition.x = Mathf::RandomRange(territoryOrigin.x, territoryOrigin.x + territoryRange);
    targetPosition.y = Mathf::RandomRange(territoryOrigin.y, territoryOrigin.y + territoryRange);
    targetPosition.z = Mathf::RandomRange(territoryOrigin.z, territoryOrigin.z + territoryRange);
}

void EnemyDrone::MoveToTarget(float elapsedTime, float speedRate)
{
    //�^�[�Q�b�g�����ւ̐i�s�x�N�g�����Z�o
    float vx = targetPosition.x - position.x;
    float vy = targetPosition.y - position.y;
    float vz = targetPosition.z - position.z;
    float dist = sqrtf(vx * vx + vz * vz);


    vx /= dist;
    vz /= dist;

    //�ړ�����
    Move(vx, vz, moveSpeed * speedRate);
    Turn(elapsedTime, vx, vz, turnSpeed * speedRate);
}

bool EnemyDrone::SearchPlayer()
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

void EnemyDrone::TransitionWanderState()
{
    state = State::Wander;

    //�ڕW�n�_�ݒ�
    //SetRandomTargetPosition();

    //�����A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Attack, true);
}

void EnemyDrone::UpdateWanderState(float elapsedTime)
{
    //�ڕW�n�_�܂�XZ���ʂł̋�������
    float vx = targetPosition.x - position.x;
    float vz = targetPosition.z - position.z;
    float distSq = vx * vx + vz * vz;
    if (distSq < radius * radius)
    {
        //���̖ڕW�n�_�ݒ�
        SetRandomTargetPosition();

        //�ҋ@�X�e�[�g�J��
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

void EnemyDrone::TransitionIdleState()
{
    state = State::Idle;

    //�^�C�}�[�������_���ݒ�
    stateTimer = Mathf::RandomRange(3.0f, 4.0f);

    //�ҋ@�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Attack, false);
}

void EnemyDrone::UpdateIdleState(float elapsedTime)
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

void EnemyDrone::TransitionPursuitState()
{
    state = State::Pursuit;

    //���b�ԒǐՂ���^�C�}�[�������_���ݒ�
    stateTimer = Mathf::RandomRange(3.0f, 5.0f);

    //�ړ��A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Attack, false);
}

void EnemyDrone::UpdatePursuitState(float elapsedTime)
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
    }

    //�v���C���[���߂Â��ƍU���X�e�[�g�ɑJ��
    float vx = targetPosition.x - position.x;
    float vz = targetPosition.z - position.z;
    float dist = sqrtf(vx * vx + vz * vz);

    if (dist <= attackRange)
    {
        //�U���X�e�[�g�ɑJ��
        TransitionAttackState();
    }
}

void EnemyDrone::TransitionAttackState()
{
    state = State::Attack;

    //�U���A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Attack, false);

    projectilecount = 0;
}

void EnemyDrone::UpdateAttackState(float elapsedTime)
{
    float animationTime = model->GetCurrentAnimationSeconds();
    if (animationTime >= 0.1f && animationTime < 0.35f)
    {
        if (projectilecount <= 1)
        {
            LaunchProjectile();
        }
        
    }

    //�U���A�j���[�V�������I�������퓬�ҋ@�X�e�[�g�֑J��
    if (!model->IsPlayAnimation())
    {
        TransitionIdleBattleState();
    }
}

void EnemyDrone::LaunchProjectile()
{
    Player& player = Player::Instance();

    // �� 3�t���[���O�̈ʒu�Ɍ������悤�ɂȂ�
    DirectX::XMStoreFloat3(&dir,
        DirectX::XMVector3Normalize(
            DirectX::XMVectorSubtract(
                DirectX::XMLoadFloat3(&GetPreviousPlayerPos(3)),
                DirectX::XMLoadFloat3(&position)
            )
        )
    );

    pos.x = position.x;
    pos.y = position.y + Enemy::GetHeight() / 2;
    pos.z = position.z;

    ProjectileEnemyToPlayer* projectileEnemyToPlayer = new ProjectileEnemyToPlayer(&projectileManager);
    if (projectilecount <=1)
    {
        projectileEnemyToPlayer->Launch(dir, pos);
        projectilecount++;
    }
    
}

void EnemyDrone::TransitionIdleBattleState()
{
    state = State::IdleBattle;

    //���b�ԑҋ@����^�C�}�[�������_���ݒ�
    stateTimer = Mathf::RandomRange(5.0f, 6.0f);

    //�퓬�ҋ@�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Attack, true);
}

void EnemyDrone::UpdateIdleBattleState(float elapsedTime)
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

void EnemyDrone::TransitionDamageState()
{
    state = State::Damage;

    //�_���[�W�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Attack, false);
}

void EnemyDrone::UpdateDamageState(float elapsedTime)
{
    //�_���[�W�A�j���[�V�������I�������퓬�ҋ@�X�e�[�g�ɑJ��
    if (!model->IsPlayAnimation())
    {
        TransitionIdleBattleState();
    }
}

void EnemyDrone::TransitionDeathState()
{
    state = State::Death;

    stateTimer = 3.0f;

    //���S�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Idle, true);
}

void EnemyDrone::UpdateDeathState(float elapsedTime)
{
    stateTimer -= elapsedTime;
    if (stateTimer < 0.0f)
    {
        Destroy();
    }
}
void EnemyDrone::Adddeadcount()
{
    ddd = EnemyDrone::Instance().GetDeadcount();
    ddd++;
    EnemyDrone::Instance().SetDeadcount(ddd);
}

void EnemyDrone::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);

    projectileManager.Render(dc, shader);
}

void EnemyDrone::OnDead()
{
    Adddeadcount();
    TransitionDeathState();
    deadFlag = true;
}

void EnemyDrone::OnDamaged()
{
    TransitionDamageState();
}

void EnemyDrone::UpdateVerticalVelocity(float elapsedFrame)
{
    Player& player = Player::Instance();
    float playery = player.GetPosition().y;
    //�d�͏���
    if (deadFlag)
    {
        velocity.y += gravity * (elapsedFrame / 30);
    }
    else
    {
        if (playery + 3.0f > position.y)
        {
            velocity.y -= gravity * (elapsedFrame / 30);
        }
        else if(player.GetPosition().y - position.y > 2.0f)
        {
            //velocity.y += gravity;
        }
        else
        {
            velocity.y = 0.0f;
        }
    }
    
}

void EnemyDrone::UpdatePlayerPosition(const DirectX::XMFLOAT3& newPos)
{
    positionHistory.push_back(newPos);
    if (positionHistory.size() > kHistoryLimit) {
        positionHistory.pop_front();
    }
}

DirectX::XMFLOAT3 EnemyDrone::GetPreviousPlayerPos(int offset)
{
    if (positionHistory.size() > offset) {
        return positionHistory[positionHistory.size() - 1 - offset];
    }
    else if (!positionHistory.empty()) {
        // ����������Ȃ��ꍇ�͍ŐV�ʒu��Ԃ�
        return positionHistory.back();
    }
    else {
        // �����L�^����Ă��Ȃ��ꍇ�̃f�t�H���g
        return { 0.0f, 0.0f, 0.0f };
    }
}

void EnemyDrone::CollisionProjectilesVsPlayer()
{
    //�������菈��
    int projectileCount = projectileManager.GetProjectileEnemyCount();
    for (int i = 0; i < projectileCount; ++i)
    {
        ProjectileEnemy* projectile = projectileManager.GetProjectileEnemy(i);

        Player& player = Player::Instance();
        //�Փˏ���
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectSphereVsCylinder(
            projectile->GetPosition(),
            projectile->GetRadius(),
            player.GetPosition(),
            player.GetRadius(),
            player.GetHeight(),
            outPosition
        ))
        {
            if (player.ApplyDamage(1))
            {
                {
                    DirectX::XMFLOAT3 impulse{};

                    const float power = 10.0f;
                    const DirectX::XMFLOAT3& e = player.GetPosition();
                    const DirectX::XMFLOAT3& p = projectile->GetPosition();
                    float vx = e.x - p.x;
                    float vz = e.z - p.z;
                    float lengthXZ = sqrtf(vx * vx + vz * vz);
                    vx /= lengthXZ;
                    vz /= lengthXZ;

                    impulse.x = vx * power;
                    impulse.y = power;
                    impulse.z = vz * power;

                    //player.AddImpulse(impulse);
                }


                projectile->Destroy();
            }

            {
                DirectX::XMFLOAT3 e = player.GetPosition();
                e.y += player.GetHeight() * 0.5f;
            }
        }

        //player.SetHit(false);
    }
}

void EnemyDrone::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("EnemyDrone", nullptr, ImGuiWindowFlags_None))
    {
        if (ImGui::CollapsingHeader("State", ImGuiTreeNodeFlags_DefaultOpen))
        {
            switch (state)
            {
            case State::Idle:       ImGui::LabelText("State", "Idle"); break;
            case State::Wander:     ImGui::LabelText("State", "Wander"); break;
            case State::Pursuit:    ImGui::LabelText("State", "Pursuit"); break;
            case State::Attack:     ImGui::LabelText("State", "Attack"); break;
            case State::IdleBattle:    ImGui::LabelText("State", "IdleBattle"); break;
            case State::Damage:    ImGui::LabelText("State", "Damage"); break;
            case State::Death:     ImGui::LabelText("State", "Death"); break;
            }

        }

        ImGui::InputFloat3("Position", &position.x);

        DirectX::XMFLOAT3 a;
        a.x = DirectX::XMConvertToDegrees(angle.x);
        a.y = DirectX::XMConvertToDegrees(angle.y);
        a.z = DirectX::XMConvertToDegrees(angle.z);

        ImGui::InputFloat3("Angle", &a.x);
        angle.x = DirectX::XMConvertToRadians(a.x);
        angle.y = DirectX::XMConvertToRadians(a.y);
        angle.z = DirectX::XMConvertToRadians(a.z);

        ImGui::InputFloat3("Velocity", &velocity.x);

        ImGui::InputFloat3("Scale", &scale.x);
    }


    ImGui::End();
}

