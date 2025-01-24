#include "WeaponManager.h"

void WeaponManager::Update(float elapsedTime)
{
    for (Weapon* weapon : weapons)
    {
        weapon->Update(elapsedTime);
    }

    for (Weapon* weapon : removes)
    {
        //vectorを削除する場合はイテレーターで削除する
        std::vector<Weapon*>::iterator it = std::find(weapons.begin(), weapons.end(), weapon);

        if (it != weapons.end())
        {
            weapons.erase(it);
        }

        delete weapon;
    }
    removes.clear();

}

void WeaponManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
    for (Weapon* enemy : weapons)
    {
        enemy->Render(context, shader);
    }
}

void WeaponManager::Register(Weapon* weapon)
{
    weapons.emplace_back(weapon);
}

void WeaponManager::Clear()
{
    for (Weapon* enemy : weapons)
    {
        delete enemy;
        enemy = nullptr;
    }
    weapons.clear();
}

void WeaponManager::Remove(Weapon* weapon)
{
    removes.insert(weapon);
}

void WeaponManager::DrawDebugPrimitive()
{
    size_t enemyCount = weapons.size();

    for (int i = 0; i < enemyCount; i++)
    {
        this->weapons[i]->DrawDebugPrimitive();
    }

}


