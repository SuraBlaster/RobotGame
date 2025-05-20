#pragma once

#include <vector>
#include "Projectile.h"
#include <set>
#include <ProjectileEnemy.h>

class ProjectileManager
{
public:
    ProjectileManager();
    ~ProjectileManager();

    //�X�V����
    void Update(float elapsedTime);

    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    //�f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive();

    //�e�ۓo�^
    void Register(Projectile* projectile);

    //�G�e�ۓo�^
    void RegisterEnemy(ProjectileEnemy* projectileEnemy);

    //�e�ۑS�폜
    void Clear();

    //�e�ې��擾
    int GetProjectileCount()const { return static_cast<int>(projectiles.size()); }

    // �G�e�ې��擾
    int GetProjectileEnemyCount()const { return static_cast<int>(projectileEnemies.size()); }

    //�e�ێ擾
    Projectile* GetProjectile(int index) { return projectiles.at(index); }

    //�G�e�ێ擾
    ProjectileEnemy* GetProjectileEnemy(int index) { return projectileEnemies.at(index); }

    //����
    void Remove(Projectile* projectile);

    //�G�e�ۏ���
    void RemoveEnemy(ProjectileEnemy* projectileEnemy);
private:
    std::vector<Projectile*> projectiles;

    std::vector<ProjectileEnemy*> projectileEnemies;

    std::set<Projectile*> removes;

    std::set<ProjectileEnemy*> removeEnemies;
};
