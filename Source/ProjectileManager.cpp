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
}

void ProjectileManager::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    for (Projectile* projectile : projectiles)
    {
        projectile->Render(dc,shader);
    }
}

void ProjectileManager::DrawDebugPrimitive()
{
    projectiles[0]->DrawDebugPrimitive();
}

void ProjectileManager::Register(Projectile* projectile)
{
    projectiles.emplace_back(projectile);
}

void ProjectileManager::Clear()
{
    for (Projectile* projectile : projectiles)
    {
        delete projectile;
        projectile = nullptr;
    }
    projectiles.clear();
}

void ProjectileManager::Remove(Projectile* projectile)
{
    removes.insert(projectile);
}
