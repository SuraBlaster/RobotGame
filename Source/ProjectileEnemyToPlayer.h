#pragma once

#include "Graphics/Model.h"
#include "ProjectileEnemy.h"

//’¼i’eŠÛ
class ProjectileEnemyToPlayer : public ProjectileEnemy
{
public:
    ProjectileEnemyToPlayer(ProjectileManager* manager);
    ~ProjectileEnemyToPlayer();

    //XVˆ—
    void Update(float elapsedTime)override;

    //•`‰æˆ—
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    //”­Ë
    void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position);

private:
    Model* model = nullptr;
    float speed = 5.0f;

    float lifeTimer = 10.0f;
    float stopTimer = 1.0f;


};

