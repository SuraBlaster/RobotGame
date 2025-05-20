#pragma once

#include "Graphics/Model.h"
#include "Item.h"

//�S�[���A�C�e��
class ItemCrystal : public Item
{
public:
    ItemCrystal();
    ~ItemCrystal() override;

    //�X�V����
    void Update(float elapsedTime)override;

    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    void ItemCrystal::UpdateVerticalVelocity(float elapsedFrame)
    {
        //�d�͏���
        velocity.y = 0;
    }
private:

    //�m�[�h�ƃv���C���[�̏Փˏ���
    void CollisionNodeVsPlayer(const char* nodeName, float nodeRadius);

private:
    Model* model = nullptr;
    float turnSpeed = DirectX::XMConvertToRadians(1);
};
