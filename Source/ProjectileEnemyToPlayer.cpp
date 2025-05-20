#include "ProjectileEnemyToPlayer.h"
#include <Mathf.h>

ProjectileEnemyToPlayer::ProjectileEnemyToPlayer(ProjectileManager* manager)
    :ProjectileEnemy(manager)
{
    model = new Model("Data/Model/Sword/Sword.mdl");

    //�\���T�C�Y�𒲐�
    scale.x = scale.y = scale.z = 0.7f;

}

ProjectileEnemyToPlayer::~ProjectileEnemyToPlayer()
{
    delete model;
}

void ProjectileEnemyToPlayer::Update(float elapsedTime)
{
    //��������
    lifeTimer -= elapsedTime;
    if (lifeTimer < 0)
    {
        Destroy();
    }

    float speed = this->speed * elapsedTime;
    position.x += direction.x * speed;
    position.y += direction.y * speed;
    position.z += direction.z * speed;
        
    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();

    //���f���s��X�V
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