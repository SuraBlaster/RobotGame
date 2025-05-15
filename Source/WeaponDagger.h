#pragma once
#include "Weapon.h" 
#include "Player.h"

class WeaponDagger : public Weapon
{
public:
    WeaponDagger();
    ~WeaponDagger()override;

    //XVˆ—
    void Update(float elapsedTime);

    //•`‰æˆ—
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    void CollisionWeaponVsEnemies();

private:
    Model* model = nullptr;

    Player::WeaponType weapon;

    float weaponHitRadius = 0.7f;

    DirectX::XMFLOAT3					weaponHitOffset = { 0, 0, 5.0f };

};
