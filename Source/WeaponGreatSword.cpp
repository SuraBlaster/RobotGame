#include "WeaponGreatSword.h"
#include <Graphics/Graphics.h>
#include <Graphics/DebugRenderer.h>
#include <EnemyManager.h>
#include <Collision.h>
WeaponGreatSword::WeaponGreatSword()
{
    model = new Model("Data/Model/Sword/MagicSword.mdl");

    scale.x = scale.y = scale.z = 0.7f;
    angle.x = DirectX::XMConvertToRadians(0);
    angle.y = DirectX::XMConvertToRadians(-90);
    angle.z = DirectX::XMConvertToRadians(-10);
}

WeaponGreatSword::~WeaponGreatSword()
{
    delete model;
    model = nullptr;
}

void WeaponGreatSword::Update(float elapsedTime)
{
    Player& player = Player::Instance();

    const char* rightHandName = "mixamorig:RightHandThumb2";

    player.UpdateTransform();

    UpdateTransform();

    //キャラクターモデルから右手ノードを検索する
    for (const Model::Node& node : player.GetModel()->GetNodes())
    {
        if (std::strcmp(node.name, rightHandName) == 0)
        {
            //右手ノードの行列と武器のローカル座標から武器のワールド座標を求める
            DirectX::XMMATRIX HandGlobalMatrix = DirectX::XMLoadFloat4x4(&node.worldTransform);

            DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);
            Transform = Transform * HandGlobalMatrix;
            DirectX::XMStoreFloat4x4(&transform, Transform);
            break;
        }

    }

    model->UpdateTransform(transform);
        
    if (player.GetWeapon() == Player::WeaponType::GreatSword)
    {
        if (player.GetAttackFlag())
        {
            CollisionWeaponVsEnemies();
        }

        scale.x = scale.y = scale.z = 0.7f;
    }
    else
    {
        scale.x = scale.y = scale.z = 0.0f;
    }

    
    
}

void WeaponGreatSword::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    DirectX::XMMATRIX weaponMatrix = DirectX::XMLoadFloat4x4(&transform);
    DirectX::XMVECTOR offset = DirectX::XMLoadFloat3(&weaponHitOffset);
    DirectX::XMVECTOR hitPosition = DirectX::XMVector3Transform(offset, weaponMatrix);

    DirectX::XMFLOAT3 weaponHitPosition;
    DirectX::XMStoreFloat3(&weaponHitPosition, hitPosition);

    Player& player = Player::Instance();
    if (player.GetAttackFlag() && player.GetWeapon() == Player::WeaponType::GreatSword)
    {
        debugRenderer->DrawSphere(weaponHitPosition, weaponHitRadius, { 1,0,1,1 });
    }
    

    shader->Draw(dc, model);
}


//プレイヤーとエネミーとの衝突判定
void WeaponGreatSword::CollisionWeaponVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    DirectX::XMMATRIX weaponMatrix = DirectX::XMLoadFloat4x4(&transform);
    DirectX::XMVECTOR offset = DirectX::XMLoadFloat3(&weaponHitOffset);
    DirectX::XMVECTOR hitPosition = DirectX::XMVector3Transform(offset, weaponMatrix);

    DirectX::XMFLOAT3 weaponHitPosition;
    DirectX::XMStoreFloat3(&weaponHitPosition, hitPosition);


    //すべての敵と総当たりで衝突判定
    int enemyCount = enemyManager.GetEnemyCount();

    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        DirectX::XMFLOAT3 outPosition;

        if (Collision::IntersectSphereVsCylinder(
            weaponHitPosition,
            weaponHitRadius,
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

                    const float power = 5.0f;
                    const DirectX::XMFLOAT3& e = enemy->GetPosition();
                    const DirectX::XMFLOAT3& p = weaponHitPosition;
                    float vx = e.x - p.x;
                    float vz = e.z - p.z;
                    float lengthXZ = sqrtf(vx * vx + vz * vz);
                    vx /= lengthXZ;
                    vz /= lengthXZ;

                    impulse.x = vx * power * 5.0f;
                    impulse.y = power * 0.1f;
                    impulse.z = vz * power * 5.0f;


                    enemy->AddImpulse(impulse);
                }
            }

            {
                DirectX::XMFLOAT3 e = enemy->GetPosition();
                e.y += enemy->GetHeight() * 0.5f;
            }
        }
    }
}