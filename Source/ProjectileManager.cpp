#include "ProjectileManager.h"
#include <Graphics/Graphics.h>

ProjectileManager::ProjectileManager()
{
}

ProjectileManager::~ProjectileManager()
{
    Clear();
}

void ProjectileManager::Update(float elapsedTime)
{
    for (Projectile* projectile : projectiles)
    {
        projectile->Update(elapsedTime);
    }

    for (ProjectileEnemy* projectileEnemy : projectileEnemies)
    {
        projectileEnemy->Update(elapsedTime);
    }

    for (Projectile* projectile : removes)
    {
        std::vector<Projectile*>::iterator it = std::find(projectiles.begin(), projectiles.end(), projectile);

        if (it != projectiles.end())
        {
            projectiles.erase(it);
        }

        delete projectile;
    }
    removes.clear();

    for (ProjectileEnemy* projectileEnemy : removeEnemies)
    {
        std::vector<ProjectileEnemy*>::iterator it = std::find(projectileEnemies.begin(), projectileEnemies.end(), projectileEnemy);

        if (it != projectileEnemies.end())
        {
            projectileEnemies.erase(it);
        }

        delete projectileEnemy;
    }
    removeEnemies.clear();
}

void ProjectileManager::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    for (Projectile* projectile : projectiles)
    {
        projectile->Render(dc, shader);
    }

    for (ProjectileEnemy* projectileEnemy : projectileEnemies)
    {
        projectileEnemy->Render(dc, shader);
    }
}

void ProjectileManager::DrawDebugPrimitive()
{
    for (Projectile* projectile : projectiles)
    {
        projectile->DrawDebugPrimitive();
    }

    for (ProjectileEnemy* projectileEnemy : projectileEnemies)
    {
        projectileEnemy->DrawDebugPrimitive();
    }
}

void ProjectileManager::Register(Projectile* projectile)
{
    projectiles.emplace_back(projectile);
}

void ProjectileManager::RegisterEnemy(ProjectileEnemy* projectileEnemy)
{
    projectileEnemies.emplace_back(projectileEnemy);
}

void ProjectileManager::Clear()
{
    for (Projectile* projectile : projectiles)
    {
        delete projectile;
        projectile = nullptr;
    }
    projectiles.clear();

    for (ProjectileEnemy* projectileEnemy : projectileEnemies)
    {
        delete projectileEnemy;
        projectileEnemy = nullptr;
    }
    projectileEnemies.clear();
}

void ProjectileManager::Remove(Projectile* projectile)
{
    removes.insert(projectile);
}

void ProjectileManager::RemoveEnemy(ProjectileEnemy* projectileEnemy)
{
    removeEnemies.insert(projectileEnemy);
}
