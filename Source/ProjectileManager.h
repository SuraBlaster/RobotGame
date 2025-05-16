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

    //XVˆ—
    void Update(float elapsedTime);

    //•`‰æˆ—
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    //ƒfƒoƒbƒOƒvƒŠƒ~ƒeƒBƒu•`‰æ
    void DrawDebugPrimitive();

    //’eŠÛ“o˜^
    void Register(Projectile* projectile);

    //“G’eŠÛ“o˜^
    void RegisterEnemy(ProjectileEnemy* projectileEnemy);

    //’eŠÛ‘Síœ
    void Clear();

    //’eŠÛ”æ“¾
    int GetProjectileCount()const { return static_cast<int>(projectiles.size()); }

    // “G’eŠÛ”æ“¾
    int GetProjectileEnemyCount()const { return static_cast<int>(projectileEnemies.size()); }

    //’eŠÛæ“¾
    Projectile* GetProjectile(int index) { return projectiles.at(index); }

    //“G’eŠÛæ“¾
    ProjectileEnemy* GetProjectileEnemy(int index) { return projectileEnemies.at(index); }

    //Á‹
    void Remove(Projectile* projectile);

    //“G’eŠÛÁ‹
    void RemoveEnemy(ProjectileEnemy* projectileEnemy);
private:
    std::vector<Projectile*> projectiles;

    std::vector<ProjectileEnemy*> projectileEnemies;

    std::set<Projectile*> removes;

    std::set<ProjectileEnemy*> removeEnemies;
};
