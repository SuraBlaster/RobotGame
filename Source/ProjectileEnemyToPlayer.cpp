#include "ProjectileEnemyToPlayer.h"
#include <Mathf.h>

ProjectileEnemyToPlayer::ProjectileEnemyToPlayer(ProjectileManager* manager)
    :ProjectileEnemy(manager)
{
    model = new Model("Data/Model/Sword/Sword.mdl");

    //表示サイズを調整
    scale.x = scale.y = scale.z = 0.7f;

}

ProjectileEnemyToPlayer::~ProjectileEnemyToPlayer()
{
    delete model;
}

void ProjectileEnemyToPlayer::Update(float elapsedTime)
{
    //寿命処理
    lifeTimer -= elapsedTime;
    if (lifeTimer < 0)
    {
        Destroy();
    }

    float speed = this->speed * elapsedTime;
    position.x += direction.x * speed;
    position.y += direction.y * speed;
    position.z += direction.z * speed;
        
    //オブジェクト行列を更新
    UpdateTransform();

    //モデル行列更新
    model->UpdateTransform(transform);

}

void ProjectileEnemyToPlayer::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

void ProjectileEnemyToPlayer::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
{
    this->direction = direction;
    this->position = position;
}