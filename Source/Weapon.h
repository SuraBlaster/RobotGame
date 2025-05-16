#pragma once
#include "Character.h"
#include "Graphics/Model.h"
#include <Graphics/Shader.h>

class Weapon : public Character
{
public:
    Weapon() {}
    ~Weapon() override {}

    //XVˆ—
    virtual void Update(float elapsedTime) = 0;

    //•`‰æˆ—
    virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

    virtual void DrawDebugPrimitive();

    void Destroy();

    void Weapon::UpdateVerticalVelocity(float elapsedFrame)
    {
        //d—Íˆ—
        velocity.y += gravity * elapsedFrame;
    }
};
