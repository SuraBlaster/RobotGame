#pragma once

#include "Graphics/Model.h"
#include "Item.h"

//ゴールアイテム
class ItemCrystal : public Item
{
public:
    ItemCrystal();
    ~ItemCrystal() override;

    //更新処理
    void Update(float elapsedTime)override;

    //描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    void ItemCrystal::UpdateVerticalVelocity(float elapsedFrame)
    {
        //重力処理
        velocity.y = 0;
    }
private:

    //ノードとプレイヤーの衝突処理
    void CollisionNodeVsPlayer(const char* nodeName, float nodeRadius);

private:
    Model* model = nullptr;
    float turnSpeed = DirectX::XMConvertToRadians(1);
};
