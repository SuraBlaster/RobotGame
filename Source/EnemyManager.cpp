#include "EnemyManager.h"
#include "Collision.h"
#include <Graphics/Graphics.h>

void EnemyManager::Update(float elapsedTime)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Update(elapsedTime);
    }

    for (Enemy* enemy : removes)
    {
        //vectorを削除する場合はイテレーターで削除する
        std::vector<Enemy*>::iterator it = std::find(enemies.begin(), enemies.end(), enemy);

        if (it != enemies.end())
        {
            enemies.erase(it);
        }

        delete enemy;
    }
    removes.clear();

    //敵同士の衝突処理
    CollisionEnemyVsEnemies();

}

void EnemyManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Render(context, shader);
    }
}

void EnemyManager::Register(Enemy* enemy)
{
    enemies.emplace_back(enemy);
}

void EnemyManager::Clear()
{
    for (Enemy* enemy : enemies)
    {
        delete enemy;
        enemy = nullptr;
    }
    enemies.clear();
}

void EnemyManager::Remove(Enemy* enemy)
{
    removes.insert(enemy);
}

void EnemyManager::DrawDebugPrimitive()
{
    size_t enemyCount = enemies.size();

    for (int i = 0; i < enemyCount;i++)
    {
        this->enemies[i]->DrawDebugPrimitive();
    }
    
}

void EnemyManager::CollisionEnemyVsEnemies()
{
    size_t enemyCount = enemies.size();

    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemyA = enemies.at(i);

        for (int j = i + 1; j < enemyCount; ++j)
        {
            Enemy* enemyB = enemies.at(j);


            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectCylinderVsCylinder(
                enemyA->GetPosition(),
                enemyA->GetRadius(),
                enemyA->GetHeight(),
                enemyB->GetPosition(),
                enemyB->GetRadius(),
                enemyB->GetHeight(),
                outPosition
            ))
            {
                enemyB->SetPosition(outPosition);
            }
        }
    }

}
