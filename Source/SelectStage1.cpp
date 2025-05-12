#include "SelectStage1.h"



//�R���X�g���N�^
SelectStage1::SelectStage1()
{
    model = new Model("Data/Model/SelectStage/SelectMap1.mdl");

    scale.x = scale.y = scale.z = 0.005f;
}

//�f�X�g���N�^
SelectStage1::~SelectStage1()
{
    //�X�e�[�W���f����j��
    delete model;
}

//�X�V����
void SelectStage1::Update(float elapsedTime)
{
    position.y += sinf(angle.y * 0.5f) * 0.0005f;
    angle.y += turnSpeed;

    UpdateTransform();

    model->UpdateTransform(transform);
}

void SelectStage1::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //�V�F�[�_�[�Ƀ��f����`�悵�Ă��炤
    shader->Draw(dc, model);
}

bool SelectStage1::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}