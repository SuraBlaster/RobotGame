#pragma once
#include "Weapon.h" 
#include "Player.h"

class WeaponDagger : public Weapon
{
public:
    WeaponDagger();
    ~WeaponDagger()override;

    //更新処理
    void Update(float elapsedTime);

    //描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    void CollisionWeaponVsEnemies();

private:
    Model* model = nullptr;

    Player::WeaponType weapon;

    float weaponHitRadius = 0.7f;

    DirectX::XMFLOAT3					weaponHitOffset = { 0, 0, 5.0f };

};
