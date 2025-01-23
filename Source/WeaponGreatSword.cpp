#include "WeaponGreatSword.h"
#include <Player.h>

WeaponGreatSword::WeaponGreatSword()
{
    model = new Model("Data/Model/Sword/MagicSword.mdl");

    scale.x = scale.y = scale.z = 0.7;
    angle.x = 0;
    angle.y = DirectX::XMConvertToRadians(-90);
    angle.z = DirectX::XMConvertToRadians(-10);
}

WeaponGreatSword::~WeaponGreatSword()
{
    delete model;
    model = nullptr;
}

void WeaponGreatSword::Update(float elapsedTime)
{
    Player& player = Player::Instance();

    const char* rightHandName = "mixamorig:RightHandThumb2";

    player.UpdateTransform();

    UpdateTransform();

    //�L�����N�^�[���f������E��m�[�h����������
    for (const Model::Node& node : player.GetModel()->GetNodes())
    {
        if (std::strcmp(node.name, rightHandName) == 0)
        {
            //�E��m�[�h�̍s��ƕ���̃��[�J�����W���畐��̃��[���h���W�����߂�
            DirectX::XMMATRIX HandGlobalMatrix = DirectX::XMLoadFloat4x4(&node.worldTransform);

            DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);
            Transform = Transform * HandGlobalMatrix;
            DirectX::XMStoreFloat4x4(&transform, Transform);
            break;
        }

    }

    model->UpdateTransform(transform);
}

void WeaponGreatSword::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}
