#pragma once
#include "Weapon.h" 

class WeaponGreatSword : public Weapon
{
public:
    WeaponGreatSword();
    ~WeaponGreatSword()override;

    //XVˆ—
    void Update(float elapsedTime);

    //•`‰æˆ—
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    void CollisionWeaponVsEnemies();

private:
    Model* model = nullptr;

    float weaponHitRadius = 0.7f;

    DirectX::XMFLOAT3					weaponHitOffset = { 0, 0, 130.0f };

};
