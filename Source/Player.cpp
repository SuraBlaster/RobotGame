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

static Player* instance = nullptr;

Player& Player::Instance()
{
    return *instance;
}

//コンストラクタ
Player::Player()
{
    model = new Model("Data/Model/Jammo/Jammo.mdl");

    //インスタンスポインタ取得
    instance = this;

    //スケーリング
    scale.x = scale.y = scale.z = 0.01f;

    //エフェクト読み込み
    hitEffect = new Effect("Data/Effect/thunder.efk");
    barrier = new Effect("Data/Effect/Barrier.efk");

    //待機ステートへ遷移
    TransitionIdleState();
}

void Player::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
    
    //衝突判定用のデバッグ円柱を描画
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

    //projectileManager.DrawDebugPrimitive();

    //攻撃衝突用の左手ノードのデバッグ球を描画
    if (attackCollisionFlag)
    {
        Model::Node* leftHandBone = model->FindNode("mixamorig:LeftHand");
        debugRenderer->DrawSphere(DirectX::XMFLOAT3(
            leftHandBone->worldTransform._41,
            leftHandBone->worldTransform._42,
            leftHandBone->worldTransform._43),
            leftHandRadius,
            DirectX::XMFLOAT4(1, 0, 0, 1)
        );
    }
    
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
    case State::Land:
        UpdateLandState(elapsedTime);
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

    model->UpdateTransform(transform);

    GamePad& gamePad = Input::Instance().GetGamePad();

    if (gamePad.GetButtonDown() & GamePad::BTN_SPACE)
    {
        velocity.y+=1;
    } 
     if (gamePad.GetButtonDown() & GamePad::BTN_CONTROL)
    {
        velocity.y-=1;
    } 
    
    UpdateBarrier();

    //barrier->UpdateEffectColor(barrierEffectHandle, elapsedTime);
}




bool Player::InputMove(float elapsedTime)
{
    //進行ベクトル取得
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    Move(moveVec.x, moveVec.z,moveVec.y, moveSpeed);

    Turn(elapsedTime, moveVec.x ,moveVec.z, turnSpeed);

    return moveVec.x != 0 || moveVec.z != 0;
}

bool Player::InputAttack()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    if (gamePad.GetButtonDown() & GamePad::BTN_B)
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

                //小ジャンプ
                Jump(jumpSpeed * 0.5f);
        }
        else
        {
            enemy->SetPosition(outPosition);
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

    //待機アニメーション再生
    model->PlayAnimation(Anim_Idle, true);
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

    if (gamepad.GetButtonDown() & GamePad::BTN_1)
    {
        TransitionBarrierState();
    }
}

void Player::TransitionMoveState()
{
    state = State::Move;

    //走りアニメーション再生
    model->PlayAnimation(Anim_Running, true);
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


    //ジャンプアニメーション再生
    model->PlayAnimation(Anim_Jump, false);
}

void Player::UpdateJumpState(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    
    if (IsGround())
    {
        TransitionLandState();
    }

    if (InputJump())
    {
        //ジャンプアニメーション再生
        model->PlayAnimation(Anim_Jump_Flip, false);
    }

    //�e�ۓ��͏���
    //InputProjectile();
}

void Player::TransitionLandState()
{
    state = State::Land;

    //着地アニメーション再生
    model->PlayAnimation(Anim_Landing, false);
}

void Player::UpdateLandState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }
}

void Player::TransitionAttackState()
{
    state = State::Attack;

    //攻撃アニメーション再生
    model->PlayAnimation(Anim_Attack, false);
}

void Player::UpdateAttackState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }

    //任意のアニメーション再生区間でのみ衝突判定処理をする
    float animationTime = model->GetCurrentAnimationSeconds();
    attackCollisionFlag = animationTime >= 0.3f && animationTime < 0.4f ? true : false;

    if (attackCollisionFlag)
    {
        //左手ノードとエネミーの衝突処理
        CollisionNodeVsEnemies("mixamorig:LeftHand", leftHandRadius);
    }
    
}

void Player::TransitionDamageState()
{
    state = State::Damage;
    onDamage = true;
    //ダメージアニメーション再生
    model->PlayAnimation(Anim_GetHit1, false);
}

void Player::UpdateDamageState(float elapsedTime)
{
    onDamage = false;
    if (health <= 0)
    {
        TransitionDamageState();
    }
    //ダメージアニメーションが終わったら待機ステートに遷移
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }
}

void Player::TransitionDeathState()
{
    state = State::Death;

    //死亡アニメーション再生
    model->PlayAnimation(Anim_Death, false);
}

void Player::UpdateDeathState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        
    }
}

void Player::TransitionBarrierState()
{
    state = State::Barrier;

    //障壁展開っぽいアニメーション再生
    model->PlayAnimation(Anim_GetHit1, false);
    
}

void Player::UpdateBarrierState(float elapsedTime)
{
    float animationTime = model->GetCurrentAnimationSeconds();

    if (animationTime >= 0.5f)
    {
        firstFlag = true;
        barrierRimit = 5;
    }

    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }
}



void Player::InputProjectile()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    if (gamePad.GetButtonDown() & GamePad::BTN_X)
    {
        DirectX::XMFLOAT3 dir;
        dir.x = sinf(angle.y);
        dir.y = 0.0f;
        dir.z = cosf(angle.y);

        DirectX::XMStoreFloat3(&dir, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&dir)));
        
        DirectX::XMFLOAT3 pos;
        pos.x = Player::GetPosition().x;
        pos.y = Player::GetPosition().y + Player::GetHeight() / 2;
        pos.z = Player::GetPosition().z;

        ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
        projectile->Launch(dir, pos);
        //projectileManager.Register(projectile);

    }

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
        EnemyManager& enemyManager = EnemyManager::Instance();
        int enemyCount = enemyManager.GetEnemyCount();

        for (int i = 0; i < enemyCount; ++i)
        {
            Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
            DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);
            DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(V);

            float d;
            DirectX::XMStoreFloat(&d, D);
            if (d < dist)
            {
                dist = d;
                target = enemy->GetPosition();
                target.y += enemy->GetHeight() * 0.5f;
            }
        }

        ProjectileHoming* projectile = new ProjectileHoming(&projectileManager);
        projectile->Launch(dir, pos, target);
    }
    
}


//ジャンプ入力
bool Player::InputJump()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
   /* if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        if (jumpCount < jumpLimit)
        {
            jumpCount++;
            Jump(jumpSpeed);

            return true;
            
        }
        
    }*/
    return false;
}

//着地したときに呼ばれる
void Player::OnLanding()
{
    jumpCount = 0;

    if (state != State::Damage && state != State::Death)
    {
        TransitionLandState();
    }
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


void Player::UpdateBarrier()
{
    if (barrierRimit > 0 && barrierEffectHandle < 0)
    {
        barrierEffectHandle = barrier->Play(position, 0.5f);
        firstFlag = false;
    }

    if (barrierRimit <= 0)
    {
        barrier->Stop(barrierEffectHandle);
        barrierEffectHandle = -1;
    }

    if (barrierRimit <= 4)
    {
        barrier->SetEffectColor(barrierEffectHandle,{255,0,0});
    }

    


    barrier->SetPosition(barrierEffectHandle, position);
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

