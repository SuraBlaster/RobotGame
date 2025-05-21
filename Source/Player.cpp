#include "Player.h"
#include <imgui.h>
#include "Input/Input.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"
#include "EffectManager.h"
#include "Collision.h"
#include "ProjectileStraight.h"
#include <ProjectileHoming.h>
#include "SceneLoading.h"
#include "SceneTitle.h"
#include "StageMain.h"

static Player* instance = nullptr;

Player& Player::Instance()
{
    return *instance;
}

//コンストラクタ
Player::Player()
{
    //SEの初期化
    Audio& audio = audio.Instance();
    AttackSE=audio.LoadAudioSource("Data/Audio/SE/Player/PlayerAttackSE.wav");
    Finish_AttackSE= audio.LoadAudioSource("Data/Audio/SE/Player/PlayerFinishAttackSE.wav");
    KnifeSE = audio.LoadAudioSource("Data/Audio/SE/Player/Knife.wav");
    Knife_FinishSE = audio.LoadAudioSource("Data/Audio/SE/Player/Knife_Finish.wav");
    Knife_Finish2SE = audio.LoadAudioSource("Data/Audio/SE/Player/Knife_Finish2.wav");
    ChangeSE = audio.LoadAudioSource("Data/Audio/SE/Player/ChangeSE.wav");
    ChieldSE = audio.LoadAudioSource("Data/Audio/SE/Player/ChieldSE.wav");
    Damage_ChieldSE =audio.LoadAudioSource("Data/Audio/SE/Player/Damage_ShieldSE.wav");
    Breaking_ChieldSE = audio.LoadAudioSource("Data/Audio/SE/Player/Breaking_ShieldSE.wav");
    Damage_PlayerSE = audio.LoadAudioSource("Data/Audio/SE/Player/Damage_PlayerSE.wav");
    Player_DeathSE = audio.LoadAudioSource("Data/Audio/SE/Player/Player_DeathSE.wav");
    //SEの音量設定
    AttackSE->sourceVoice->SetVolume(75);
    Finish_AttackSE->sourceVoice->SetVolume(95);
    KnifeSE->sourceVoice->SetVolume(50);
    Knife_FinishSE->sourceVoice->SetVolume(50);
    Knife_Finish2SE->sourceVoice->SetVolume(50);
    ChangeSE->sourceVoice->SetVolume(50);
    ChieldSE->sourceVoice->SetVolume(50);
    Damage_ChieldSE->sourceVoice->SetVolume(50);
    Breaking_ChieldSE->sourceVoice->SetVolume(50);
    Damage_PlayerSE->sourceVoice->SetVolume(50);
    Player_DeathSE->sourceVoice->SetVolume(50);
    //�X�P�[�����O
    model = new Model("Data/Model/Player/Player2.mdl");
    scale.x = scale.y = scale.z = 0.01f;

    //インスタンスポインタ取得
    instance = this;

    //エフェクト読み込み
    hitEffect = new Effect("Data/Effect/thunder.efk");
    barrier = new Effect("Data/Effect/Barrier.efkefc");

    //待機ステートへ遷移
    TransitionIdleState();

    ShieldCount = 3;
}

void Player::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
    
    //�Փ˔���p�̃f�o�b�O�~����`��
    //debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

    //projectileManager.DrawDebugPrimitive();
    //衝突判定用のデバッグ円柱を描画
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

    
}

Player::~Player()
{
    delete hitEffect;
    delete barrier;

    if (model != nullptr)
    {
        delete model;
        model = nullptr;
    }
    
}



//更新処理
void Player::Update(float elapsedTime)
{
    switch (state)
    {
    case State::Idle:
        UpdateIdleState(elapsedTime);
        break;
    case State::Move:
        UpdateMoveState(elapsedTime);
        break;
    case State::Jump:
        UpdateJumpState(elapsedTime);
        break;
    case State::Attack:
        UpdateAttackState(elapsedTime);
        break;
    case State::Damage:
        UpdateDamageState(elapsedTime);
        break;
    case State::Death:
        UpdateDeathState(elapsedTime);
        break;
    case State::Barrier:
        UpdateBarrierState(elapsedTime);
    }

    UpdateTransform();

    //走力速度更新
    UpdateVelocity(elapsedTime);

    UpdateInvincibleTimer(elapsedTime);

   // projectileManager.Update(elapsedTime);

    CollisionPlayerVsEnemies();

    CollisionprojectilesVsEnemies();

    model->UpdateAnimation(elapsedTime);

    if (StageMain::Instance().GetIsRotation())
    {
        position.x += StageMain::Instance().transform.position.x;
        position.y += StageMain::Instance().transform.position.y;
        position.z += StageMain::Instance().transform.position.z;
        //position = {};
    }

    if (velocity.y < -50.0f)
    {
        position = {1,1,1};
    }

    model->UpdateTransform(transform);

    UpdateBarrier(elapsedTime);

    ChangeWeapon();
}




bool Player::InputMove(float elapsedTime)
{
    //進行ベクトル取得
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    Move(moveVec.x, moveVec.z, moveSpeed);

    Turn(elapsedTime, moveVec.x ,moveVec.z, turnSpeed);

    return moveVec.x != 0 || moveVec.z != 0;
}

bool Player::InputMoveSword(float elapsedTime)
{
    //�i�s�x�N�g���擾
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    Move(moveVec.x, moveVec.z, moveSpeed * 0.2f);

    Turn(elapsedTime, moveVec.x ,moveVec.z, turnSpeed * 0.2f);

    return moveVec.x != 0 || moveVec.z != 0;
}

bool Player::InputAttack()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    //if (gamePad.GetButtonDown() & GamePad::BTN_B)
    //マウス
    Mouse& mouse = Input::Instance().GetMouse();
    if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
    {
        return true;
    }
    return false;
}

//プレイヤーとエネミーとの衝突判定
void Player::CollisionPlayerVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //すべての敵と総当たりで衝突判定
    int enemyCount = enemyManager.GetEnemyCount();

    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        DirectX::XMFLOAT3 outPosition;


        if (Collision::IntersectCylinderVsCylinder(
            Player::GetPosition(),
            Player::GetRadius(),
            Player::GetHeight(),
            enemy->GetPosition(),
            enemy->GetRadius(),
            enemy->GetHeight(),
            outPosition))
        {
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
            DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
            DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
            DirectX::XMFLOAT3 normal;
            DirectX::XMStoreFloat3(&normal, N);

            if (normal.y > 0.8f)
            {
               //Jump(jumpSpeed * 0.5f);
            }
             else
            {
              enemy->SetPosition(outPosition);
            }
        }
    }
}

void Player::CollisionNodeVsEnemies(const char* nodeName, float nodeRadius)
{
    //ノード取得
    Model::Node* node = model->FindNode(nodeName);

    //ノード位置取得
    DirectX::XMMATRIX Nodetra = DirectX::XMLoadFloat4x4(&node->worldTransform);
    DirectX::XMVECTOR Nodepos = Nodetra.r[3];
    DirectX::XMFLOAT3 NodePosition;

    DirectX::XMStoreFloat3(&NodePosition, Nodepos);

    //指定のノードとすべての敵を当たり判定で衝突処理
    EnemyManager& enemyManager = EnemyManager::Instance();

    int enemyCount = enemyManager.GetEnemyCount();


    for (int i = 0; i < enemyCount; ++i)
    {
        // 敵の位置取得
        Enemy* enemy = enemyManager.GetEnemy(i);

            DirectX::XMFLOAT3 outPosition;

            if (Collision::IntersectSphereVsCylinder(
                NodePosition,
                nodeRadius,
                enemy->GetPosition(),
                enemy->GetRadius(),
                enemy->GetHeight(),
                outPosition))
            {
                enemy->SetPosition(outPosition);

                if (enemy->ApplyDamage(1))
                {
                    {
                        DirectX::XMFLOAT3 impulse{};

                        const float power = 10.0f;
                        const DirectX::XMFLOAT3& e = enemy->GetPosition();
                        const DirectX::XMFLOAT3& p = NodePosition;
                        float vx = e.x - p.x;
                        float vz = e.z - p.z;
                        float lengthXZ = sqrtf(vx * vx + vz * vz);
                        vx /= lengthXZ;
                        vz /= lengthXZ;

                        impulse.x = vx * power;
                        impulse.y = power * 0.5f;
                        impulse.z = vz * power;


                        enemy->AddImpulse(impulse);
                    }
                }

                {
                    DirectX::XMFLOAT3 e = enemy->GetPosition();
                    e.y += enemy->GetHeight() * 0.5f;
                    hitEffect->Play(e);
                }

            }
    }
}



void Player::TransitionIdleState()
{
    state = State::Idle;

    //�ҋ@�A�j���[�V�����Đ�
    switch (weapon)
    {
    case WeaponType::GreatSword:
        model->PlayAnimation(GreatSword_Idle, true);
        break;
    case WeaponType::Dagger:
        model->PlayAnimation(Dagger_Idle, true);
        break;
    }
}

void Player::UpdateIdleState(float elapsedTime)
{
    GamePad& gamepad = Input::Instance().GetGamePad();

    if (InputMove(elapsedTime))
    {
        TransitionMoveState();
    }

    //移動入力処理
    InputMove(elapsedTime);

    //ジャンプ入力処理
    if (InputJump())
    {
        TransitionJumpState();
    }

    //�e�ۓ��͏���
    //InputProjectile();


    //攻撃入力処理
    if (InputAttack())
    {
        TransitionAttackState();
    }

    if (ShieldCount > 0 && ShieldTimer <= 0)
    {
        if (gamepad.GetButtonDown() & GamePad::BTN_1)
        {
            ChieldSE->Play(false);
            TransitionBarrierState();
        }
    }
    
}

void Player::TransitionMoveState()
{
    state = State::Move;

    //����A�j���[�V�����Đ�
    switch (weapon)
    {
    case WeaponType::GreatSword:
        model->PlayAnimation(GreatSword_Run, true);
        break;
    case WeaponType::Dagger:
        model->PlayAnimation(Dagger_Run, true);
        break;
    }
}

void Player::UpdateMoveState(float elapsedTime)
{
    if (!InputMove(elapsedTime))
    {
        TransitionIdleState();
    }

    //移動入力処理
    InputMove(elapsedTime);

    //ジャンプ入力処理
    if (InputJump())
    {
        TransitionJumpState();
    }

    //�e�ۓ��͏���
    //InputProjectile();


    //攻撃入力処理
    if (InputAttack())
    {
        TransitionAttackState();
    }
}

void Player::TransitionJumpState()
{
    state = State::Jump;

    model->PlayAnimation(GreatSword_Jump, false);
}

void Player::UpdateJumpState(float elapsedTime)
{
    if (IsGround())
    {
        TransitionIdleState();
    }

    //�e�ۓ��͏���
    //InputProjectile();
}

void Player::TransitionAttackState()
{
    state = State::Attack;

    //�U���A�j���[�V�����Đ�
    switch (weapon)
    {
    case WeaponType::GreatSword:
        model->PlayAnimation(GreatSword_Attack, false);
        break;
    case WeaponType::Dagger:
        model->PlayAnimation(Dagger_Attack, false);
        break;
    }
    
    attackStage = 0;
}

void Player::UpdateAttackState(float elapsedTime)
{
    switch (weapon)
    {
    case WeaponType::GreatSword:
    {
        //任意のアニメーション再生区間でのみ衝突判定処理をする
        float animationTime = model->GetCurrentAnimationSeconds();
        attackCollisionFlag = (animationTime >= 0.8f && animationTime < 1.0f)
            || (animationTime >= 1.65f && animationTime < 1.85f)
            || (animationTime >= 2.55f && animationTime < 2.8f) ? true : false;

        InputMoveSword(elapsedTime);

        Mouse& mouse = Input::Instance().GetMouse();
        if (mouse.GetButtonUp() & Mouse::BTN_LEFT)
        {
            if (animationTime <= 1.0f)
            {
                AttackSE->Play(false);
                attackStage = 1;
            }
            else if (animationTime <= 1.85)
            {
                AttackSE->Play(false);
                attackStage = 2;
            }
            else
            {
                Finish_AttackSE->Play(false);
                attackStage = 3;
            }
        }

        switch (attackStage)
        {
        case 1:
            if (animationTime > 1.0f)
            {
                AttackSE->Stop();
            }
            if (animationTime > 1.25f)
            {
                TransitionIdleState();
            }
            break;
        case 2:
            if (animationTime > 1.85f)
            {
                AttackSE->Stop();
            }
            if (animationTime > 2.15f)
            {
                TransitionIdleState();
            }
            break;
        case 3:
            if (animationTime > 2.8f)
            {
                Finish_AttackSE->Stop();
            }
            if (!model->IsPlayAnimation())
            {
                TransitionIdleState();
            }
            break;
        }

    }
    break;

    case WeaponType::Dagger:
    {
        float animationTime = model->GetCurrentAnimationSeconds();
        attackCollisionFlag = (animationTime >= 0.5f && animationTime < 0.8f)
            || (animationTime >= 1.0f && animationTime < 1.6f)
            || (animationTime >= 2.3f && animationTime < 2.8f) ? true : false;

        InputMoveSword(elapsedTime);

        Mouse& mouse = Input::Instance().GetMouse();
        if (mouse.GetButtonUp() & Mouse::BTN_LEFT)
        {
            if (animationTime <= 0.8f)
            {
                KnifeSE->Play(false);
                attackStage = 1;
            }
            else
            {
                Knife_FinishSE->Play(false);
                attackStage = 2;
            }
        }

        switch (attackStage)
        {
        case 1:
            if (animationTime > 0.8)
            {
                KnifeSE->Stop();
            }
          if (animationTime > 1.0f)
            {
                attackCollisionFlag = false;
                TransitionIdleState();
            }
            break;
        case 2:
            if (animationTime > 1.6f)
            {
                Knife_FinishSE->Stop();
            }
            if (animationTime > 2.3f)
            {
                Knife_Finish2SE->Play(false);
            }
            if (animationTime > 2.8f)
            {
                Knife_Finish2SE->Stop();
            }
            if (!model->IsPlayAnimation())
            {
                TransitionIdleState();
            }
            break;
        }

    }
    break;
    }

}

void Player::TransitionDamageState()
{
    state = State::Damage;

    //�_���[�W�A�j���[�V�����Đ�
    switch (weapon)
    {
    case WeaponType::GreatSword:
        model->PlayAnimation(GreatSword_Damage, false);
        break;
    case WeaponType::Dagger:
        model->PlayAnimation(Dagger_Damage, false);
        break;
    }
}

void Player::UpdateDamageState(float elapsedTime)
{
    onDamage = false;
    Damage_PlayerSE->Play(false);
    if (health <= 0)
    {
        TransitionDamageState();
    }
    //ダメージアニメーションが終わったら待機ステートに遷移
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
        Damage_PlayerSE->Stop();
    }
}

void Player::TransitionDeathState()
{
    state = State::Death;

    //���S�A�j���[�V�����Đ�
    switch (weapon)
    {
    case WeaponType::GreatSword:
        model->PlayAnimation(GreatSword_Death, false);
        break;
    case WeaponType::Dagger:
        model->PlayAnimation(Dagger_Death, false);
        break;
    }
}

void Player::UpdateDeathState(float elapsedTime)
{
    Player_DeathSE->Play(false);
    if (!model->IsPlayAnimation())
    {
        SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
    }
}

void Player::TransitionBarrierState()
{
    state = State::Barrier;

    //��ǓW�J���ۂ��A�j���[�V�����Đ�
    switch (weapon)
    {
    case WeaponType::GreatSword:
        model->PlayAnimation(GreatSword_Shield, false);
        break;
    case WeaponType::Dagger:
        model->PlayAnimation(Dagger_Shield, false);
        break;
    }

    ShieldCount--;
}

void Player::UpdateBarrierState(float elapsedTime)
{
    float animationTime = model->GetCurrentAnimationSeconds();

    if (animationTime >= 0.5f)
    {
        ShieldTimer = 30;
        ShieldFlag = true;
    }

    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }
}



//void Player::InputProjectile()
//{
//    GamePad& gamePad = Input::Instance().GetGamePad();
//
//    if (gamePad.GetButtonDown() & GamePad::BTN_X)
//    {
//        DirectX::XMFLOAT3 dir;
//        dir.x = sinf(angle.y);
//        dir.y = 0.0f;
//        dir.z = cosf(angle.y);
//
//        DirectX::XMStoreFloat3(&dir, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&dir)));
//        
//        DirectX::XMFLOAT3 pos;
//        pos.x = Player::GetPosition().x;
//        pos.y = Player::GetPosition().y + Player::GetHeight() / 2;
//        pos.z = Player::GetPosition().z;
//
//        ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
//        projectile->Launch(dir, pos);
//        //projectileManager.Register(projectile);
//
//    }
//
//    if (gamePad.GetButtonDown() & GamePad::BTN_Y)
//    {
//        DirectX::XMFLOAT3 dir;
//        dir.x = sinf(angle.y);
//        dir.y = 0.0f;
//        dir.z = cosf(angle.y);
//
//        DirectX::XMFLOAT3 pos;
//        pos.x = position.x;
//        pos.y = position.y + height / 2;
//        pos.z = position.z;
//
//        DirectX::XMFLOAT3 target;
//        target.x = pos.x + dir.x * 1000.0f;
//        target.y = pos.y + dir.y * 1000.0f;
//        target.z = pos.z + dir.z * 1000.0f;
//
//        float dist = FLT_MAX;
//        EnemyManager& enemyManager = EnemyManager::Instance();
//        int enemyCount = enemyManager.GetEnemyCount();
//
//        for (int i = 0; i < enemyCount; ++i)
//        {
//            Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
//            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
//            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
//            DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);
//            DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(V);
//
//            float d;
//            DirectX::XMStoreFloat(&d, D);
//            if (d < dist)
//            {
//                dist = d;
//                target = enemy->GetPosition();
//                target.y += enemy->GetHeight() * 0.5f;
//            }
//        }
//
//        ProjectileHoming* projectile = new ProjectileHoming(&projectileManager);
//        projectile->Launch(dir, pos, target);
//    }
//    
//}


//ジャンプ入力
bool Player::InputJump()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
   if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        Jump(jumpSpeed);

        return true;
    }
    return false;
}

void Player::OnDamaged()
{
    //ダメージステートに遷移
    TransitionDamageState();
}

void Player::OnDead()
{
    //死亡ステートに遷移
    TransitionDeathState();
}

void Player::CollisionprojectilesVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //総当たり処理
    int projectileCount = projectileManager.GetProjectileCount();
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < projectileCount; ++i)
    {
        Projectile* projectile = projectileManager.GetProjectile(i);

        for (int j = 0; j < enemyCount; ++j)
        {
            Enemy* enemy = enemyManager.GetEnemy(j);

            //衝突処理
            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectSphereVsCylinder(
                projectile->GetPosition(),
                projectile->GetRadius(),
                enemy->GetPosition(),
                enemy->GetRadius(),
                enemy->GetHeight(),
                outPosition
            ))
            {
                if (enemy->ApplyDamage(1))
                {
                    {
                        DirectX::XMFLOAT3 impulse{};

                        const float power = 10.0f;
                        const DirectX::XMFLOAT3& e = enemy->GetPosition();
                        const DirectX::XMFLOAT3& p = projectile->GetPosition();
                        float vx = e.x - p.x;
                        float vz = e.z - p.z;
                        float lengthXZ = sqrtf(vx * vx + vz * vz);
                        vx /= lengthXZ;
                        vz /= lengthXZ;

                        impulse.x = vx * power;
                        impulse.y = power * 0.5f;
                        impulse.z = vz * power;
                        

                        enemy->AddImpulse(impulse);
                    }


                    projectile->Destroy();
                }

                {
                    DirectX::XMFLOAT3 e = enemy->GetPosition();
                    e.y += enemy->GetHeight() * 0.5f;
                    hitEffect->Play(e);
                }
           }
        }
    }
}

//void Player::UpdateBarrier()
void Player::UpdateBarrier(float elapsedTime)
{

    if (ShieldTimer <= 30 && barrierEffectHandle < 0 && ShieldFlag == true)
    {
        barrierEffectHandle = barrier->Play(position, 0.5f);
    }

    if (ShieldTimer <= 0 && ShieldFlag)
    {
        barrier->Stop(barrierEffectHandle);
        barrierEffectHandle = -1;
        ShieldFlag = false;
    }

    if (ShieldTimer <= 10 && ShieldFlag)
    {
        barrier->SetEffectColor(barrierEffectHandle,{255,0,0});
    }

    barrier->SetPosition(barrierEffectHandle, position);

    if (ShieldTimer > 0)
    {
        ShieldTimer -= elapsedTime;
    }

    if (ShieldTimer < 0)
    {
        ShieldTimer = 0;
    }
    
}

void Player::ChangeWeapon()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_2)
    {
        ChangeSE->Play(false);
        weapon = WeaponType::GreatSword;
        TransitionIdleState();
    }
    else if (gamePad.GetButtonDown() & GamePad::BTN_3)
    {
        ChangeSE->Play(false);
        weapon = WeaponType::Dagger;
        TransitionIdleState();
    }
}

//描画処理
void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);


    projectileManager.Render(dc, shader);
}

void Player::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
    {
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
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

            ImGui::InputFloat("invincible", &invincibleTimer);
        }
    }


    ImGui::End();
}

DirectX::XMFLOAT3 Player::GetMoveVec() const
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    Camera& camera = Camera::Instance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();
  
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);

    if (cameraRightLength > 0.0f)
    {
        cameraRightX /= cameraRightLength;
        cameraRightZ /= cameraRightLength;
    }

    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);

    if (cameraFrontLength > 0.0f)
    {
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;
    }

    DirectX::XMFLOAT3 vec;
    vec.x = ax * cameraRightX + ay * cameraFrontX;
    vec.z = ay * cameraFrontZ + ax * cameraRightZ;
    vec.y = 0;


    return vec;
}

