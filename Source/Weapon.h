#pragma once
#include "Character.h"
#include "Graphics/Model.h"
#include <Graphics/Shader.h>

class Weapon : public Character
{
public:
    Weapon() {}
    ~Weapon() override {}

    //更新処理
    virtual void Update(float elapsedTime) = 0;

    //描画処理
    virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

    virtual void DrawDebugPrimitive();

    void Destroy();


};
