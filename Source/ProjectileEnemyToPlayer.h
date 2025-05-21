#pragma once

#include "Graphics/Model.h"
#include "ProjectileEnemy.h"

//���i�e��
class ProjectileEnemyToPlayer : public ProjectileEnemy
{
public:
    ProjectileEnemyToPlayer(ProjectileManager* manager);
    ~ProjectileEnemyToPlayer();

    //�X�V����
    void Update(float elapsedTime)override;

    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    //����
    void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position);

private:
    Model* model = nullptr;
    float speed = 5.0f;

    float lifeTimer = 10.0f;
    float stopTimer = 1.0f;


};

