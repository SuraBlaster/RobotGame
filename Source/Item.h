#pragma once

#include "Graphics/Shader.h"
#include "Character.h"

class Item : public Character
{
public:
    Item() {}
    ~Item() override {}

    //XVˆ—
    virtual void Update(float elapsedTime) = 0;

    //•`‰æˆ—
    virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;



    void Destroy();

};
