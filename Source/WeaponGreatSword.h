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
private:
    Model* model = nullptr;

};
