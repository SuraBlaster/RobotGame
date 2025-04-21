#include "StageMain.h"
#include <algorithm>



//�R���X�g���N�^
StageMain::StageMain()
{
    model = new Model("Data/Model/ExampleStage/ExampleStage.mdl");
    currentGravityDir = GetGravityVector(GravityDirection::Gravity_Down);
}

//�f�X�g���N�^
StageMain::~StageMain()
{
    //�X�e�[�W���f����j��
    delete model;
}

//�X�V����
void StageMain::Update(float elapsedTime)
{
    //��邱�Ɩ���
}

void StageMain::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //�V�F�[�_�[�Ƀ��f����`�悵�Ă��炤
    shader->Draw(dc, model);
}

bool StageMain::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}

DirectX::XMVECTOR StageMain::GetGravityVector(GravityDirection dir)
{
    switch (dir) {
    case Down:      return DirectX::XMVectorSet(0, -1, 0, 0);
    case Up:        return DirectX::XMVectorSet(0, 1, 0, 0);
    case West:      return DirectX::XMVectorSet(-1, 0, 0, 0);
    case East:      return DirectX::XMVectorSet(1, 0, 0, 0);
    case North:     return DirectX::XMVectorSet(0, 0, 1, 0);
    case South:     return DirectX::XMVectorSet(0, 0, -1, 0);
    }
    return DirectX::XMVectorSet(0, -1, 0, 0);
}

void StageMain::RotationStage(float elapsedTime)
{
    DirectX::XMVECTOR oldGravity = currentGravityDir;
    DirectX::XMVECTOR newGravity = GetGravityVector(selectedDirection);

    DirectX::XMVECTOR rotationAxis = DirectX::XMVector3Cross(oldGravity, newGravity);
    float dot = DirectX::XMVectorGetX(DirectX::XMVector3Dot(oldGravity, newGravity));
    dot = max(min(dot, 1.0f), -1.0f); // ���S�̂���Clamp

    // 180�x���]�̏ꍇ�̓N���X�ς��[���ɂȂ�̂ŕ␳���K�v
    if (DirectX::XMVector3Equal(rotationAxis, DirectX::XMVectorZero())) {
        // �d�͂����S�ɋt�i��: Down��Up�j�Ȃ̂ŁA�K���Ȑ�������180�x��]
        DirectX::XMVECTOR fallbackAxis = DirectX::XMVectorSet(1, 0, 0, 0); // Y���ɋ߂����X���Ƃ�
        rotationAxis = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(oldGravity, fallbackAxis));
    }

    float angle = acosf(dot);
    DirectX::XMVECTOR rotationQuat = DirectX::XMQuaternionRotationAxis(DirectX::XMVector3Normalize(rotationAxis), angle);


    // �X�e�[�W��]
    DirectX::XMVECTOR stageQuat = currentStageRotation;

    // �������ĐV�����p����
    DirectX::XMVECTOR targetStageQuat = DirectX::XMQuaternionMultiply(rotationQuat, stageQuat);
    targetStageQuat = DirectX::XMQuaternionNormalize(targetStageQuat);

    // ��ԁF���X�ɉ񂷁i��: 1�b�����ĉ�]�j
    float t = std::clamp(elapsedTime / rotateDuration, 0.0f, 1.0f);
    currentStageRotation = DirectX::XMQuaternionSlerp(stageQuat, targetStageQuat, t);
}

void StageMain::ChangeGravity(HitResult hit)
{
    DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);
    DirectX::XMFLOAT3 normal;
    DirectX::XMStoreFloat3(&normal, DirectX::XMVector3Normalize(Normal));

    if (normal.x >= 0.8f)
    {
        selectedDirection = East;
    }
    else if (normal.x <= -0.8f)
    {
        selectedDirection = West;
    }

    if (normal.y >= 0.8f)
    {
        selectedDirection = Up;
    }
    else if (normal.y <= -0.8f)
    {
        selectedDirection = Down;
    }

    if (normal.z >= 0.8f)
    {
        selectedDirection = North;
    }
    else if (normal.z <= -0.8f)
    {
        selectedDirection = South;
    }
}

