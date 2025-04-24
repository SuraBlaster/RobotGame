#include "StageMain.h"
#include <algorithm>
#include "Camera.h"

//�R���X�g���N�^
StageMain::StageMain()
{
    model = new Model("Data/Model/ExampleStage/ExampleStage.mdl");
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
    transform.updateMatrix();
    //if (selectedDirection != Down)
    RotationStage(elapsedTime);
    //const DirectX::XMFLOAT4X4 transformIdentity = { 1,0,0,0 ,0,1,0,0 ,0,0,1,0 ,0,0,0,1 };
    model->UpdateTransform(transform.getMatrix());
}

void StageMain::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    model->UpdateTransform(transform.getMatrix());

    transform.drawDebugGUI("StageMain");

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
    case West:      return DirectX::XMVectorSet(-1, 0, 0, 0);
    case East:      return DirectX::XMVectorSet(1, 0, 0, 0);
    case North:     return DirectX::XMVectorSet(0, 0, 1, 0);
    case South:     return DirectX::XMVectorSet(0, 0, -1, 0);
    }
    return DirectX::XMVectorSet(0, -1, 0, 0);
}

void StageMain::RotationStage(float elapsedTime)
{
//    DirectX::XMVECTOR oldGravity = DirectX::XMLoadFloat4(&transform.rotation);
//    DirectX::XMVECTOR newGravity = GetGravityVector(selectedDirection);
//
//    DirectX::XMVECTOR rotationAxis = DirectX::XMVector3Cross(oldGravity, newGravity);
//    float dot = DirectX::XMVectorGetX(DirectX::XMVector3Dot(oldGravity, newGravity));
//    dot = max(min(dot, 1.0f), -1.0f); // ���S�̂���Clamp
//
//    //// 180�x���]�̏ꍇ�̓N���X�ς��[���ɂȂ�̂ŕ␳���K�v
//    //if (DirectX::XMVector3Equal(rotationAxis, DirectX::XMVectorZero())) {
//    //    // �d�͂����S�ɋt�i��: Down��Up�j�Ȃ̂ŁA�K���Ȑ�������180�x��]
//    //    DirectX::XMVECTOR fallbackAxis = DirectX::XMVectorSet(1, 0, 0, 0); // Y���ɋ߂����X���Ƃ�
//    //    rotationAxis = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(oldGravity, fallbackAxis));
//    //}
//
//    float angle = acosf(dot);
//    DirectX::XMVECTOR rotationQuat = DirectX::XMQuaternionRotationAxis(DirectX::XMVector3Normalize(rotationAxis), angle);
//
//
//    // �X�e�[�W��]
//    DirectX::XMVECTOR stageQuat = oldGravity;
//
//    // �������ĐV�����p����
//    DirectX::XMVECTOR targetStageQuat = DirectX::XMQuaternionMultiply(rotationQuat, stageQuat);
//    targetStageQuat = DirectX::XMQuaternionNormalize(targetStageQuat);
//
//    // ��ԁF���X�ɉ񂷁i��: 1�b�����ĉ�]�j
//    timer = std::clamp(elapsedTime / rotateDuration, 0.0f, 1.0f);
//    DirectX::XMStoreFloat4(&transform.rotation, DirectX::XMQuaternionSlerp(stageQuat, targetStageQuat, timer));

    DirectX::XMVECTOR up, front, right;

    up  = DirectX::XMLoadFloat3(&transform.getUp());
    front = DirectX::XMLoadFloat3(&transform.getFront());
    right = DirectX::XMLoadFloat3(&transform.getRight());
    DirectX::XMVECTOR Rotation = DirectX::XMLoadFloat4(&transform.rotation);

    switch (selectedDirection) {
    case Down:
    {
        return;
    }
    case West:
    {
        DirectX::XMVECTOR q = DirectX::XMQuaternionRotationAxis(front, 3 * PIDIV180 * elapsedTime);
        Rotation = DirectX::XMQuaternionMultiply(Rotation, q);
    }
    case East:
    {
        DirectX::XMVECTOR q = DirectX::XMQuaternionRotationAxis(front, -3 * PIDIV180 * elapsedTime);
        Rotation = DirectX::XMQuaternionMultiply(Rotation, q);
    }
    case North:
    {
        DirectX::XMVECTOR q = DirectX::XMQuaternionRotationAxis(right, -3 * PIDIV180 * elapsedTime);
        Rotation = DirectX::XMQuaternionMultiply(Rotation, q);
    }
    case South:
    {
        DirectX::XMVECTOR q = DirectX::XMQuaternionRotationAxis(right, 3 * PIDIV180 * elapsedTime);
        Rotation = DirectX::XMQuaternionMultiply(Rotation, q);
    }
    }

    DirectX::XMStoreFloat4(&transform.rotation, Rotation);
}

void StageMain::ChangeGravity(HitResult hit)
{
    Camera& camera = Camera::Instance();
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&camera.GetFront())));

    if (std::abs(front.x) < std::abs(front.z))
    {
        if (front.z > 0.0f)
        {
            selectedDirection = North;
        }
        else
        {
            selectedDirection = South;
        }
    }
    else
    {
        if (front.x > 0.0f)
        {
            selectedDirection = East;
        }
        else
        {
            selectedDirection = West;
        }
    }

    //if (normal.x >= 0.8f)
    //{
    //    selectedDirection = West;
    //}
    //else if (normal.x <= -0.8f)
    //{
    //    selectedDirection = East;
    //}

    //if (normal.y >= 0.8f)
    //{
    //    selectedDirection = Down;
    //}
    //else if (normal.y <= -0.8f)
    //{
    //    selectedDirection = Up;
    //}

    //if (normal.z >= 0.8f)
    //{
    //    selectedDirection = South;
    //}
    //else if (normal.z <= -0.8f)
    //{
    //    selectedDirection = East;
    //}
}

