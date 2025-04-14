
#include "CameraController.h"
#include "Camera.h"
#include "Stage.h"
#include "Debugs/Debug.h"
#include "Input/Input.h"
#include "imgui.h"

void CameraController::Update(float elapsedTime)
{
    Mouse& mouse = Input::Instance().GetMouse();
    float ax = static_cast<float>(mouse.GetPositionX() - mouse.GetOldPositionX());
    float ay = static_cast<float>(mouse.GetPositionY() - mouse.GetOldPositionY());

    if (isZeroClear)
    {
        ax = ay = 0;
        isZeroClear = false;
    }

    //X,y���̉�]
    float sensitive = 0.001f;
    angle.y -= static_cast<float>(ax) * sensitive;
    angle.x -= static_cast<float>(ay) * sensitive;

    if (angle.x < maxAngleX)
    {
        angle.x = maxAngleX;
    }
    if (angle.x > minAngleX)
    {
        angle.x = minAngleX;
    }

    if (angle.y < -DirectX::XM_PI)
    {
        angle.y += DirectX::XM_2PI;
    }
    if (angle.y > DirectX::XM_PI)
    {
        angle.y -= DirectX::XM_2PI;
    }

    //�J�����̉�]�l����]�s��ɕϊ�
    DirectX::XMMATRIX transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);1

    //��]�s�񂩂�O�����x�N�g�������o��
    DirectX::XMVECTOR Front = transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    //���ӓ_����x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
    DirectX::XMFLOAT3 eye;
    eye.x = target.x + front.x * range;
    eye.y = target.y + front.y * range;
    eye.z = target.z + front.z * -range;

    //�J�������C�L���X�g
    if (!Debug::Instance().GetIsDebugMode())
    {
        DirectX::XMFLOAT3 start = target;
        DirectX::XMFLOAT3 end = { eye.x + front.x,eye.y + front.y,eye.z - front.z };
        HitResult hit;
        if (Stage::Instance().RayCast(start, end, hit))
        {
            range -= DirectX::XMVectorGetX(DirectX::XMVector3Length(
                DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&hit.position), DirectX::XMLoadFloat3(&end))));
            if (range < 0.001f)
            {
                range = 0.001f;
            }
            eye.x = target.x + front.x * range;
            eye.y = target.y + front.y * range;
            eye.z = target.z + front.z * -range;
        }
        else
        {
            range = beforRange;
        }
    }
    Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}

void CameraController::UpdateNormal(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    float ax = gamePad.GetAxisRX();
    float ay = gamePad.GetAxisRY();
    //X,y���̉�]
    float speed = normalRollSpeed * elapsedTime;
    angle.y -= ax * speed;
    angle.x += ay * speed;

    if (angle.x < maxAngleX)
    {
        angle.x = maxAngleX;
    }
    if (angle.x > minAngleX)
    {
        angle.x = minAngleX;
    }

    if (angle.y < -DirectX::XM_PI)
    {
        angle.y += DirectX::XM_2PI;
    }
    if (angle.y > DirectX::XM_PI)
    {
        angle.y -= DirectX::XM_2PI;
    }

    //�J�����̉�]�l����]�s��ɕϊ�
    DirectX::XMMATRIX transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    //��]�s�񂩂�O�����x�N�g�������o��
    DirectX::XMVECTOR Front = transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    //���ӓ_����x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
    DirectX::XMFLOAT3 eye;
    eye.x = target.x + front.x * range;
    eye.y = target.y + front.y * range;
    eye.z = target.z + front.z * -range;

    //�J�������C�L���X�g
    if (!Debug::Instance().GetIsDebugMode())
    {
        DirectX::XMFLOAT3 start = target;
        DirectX::XMFLOAT3 end = { eye.x + front.x,eye.y + front.y,eye.z - front.z };
        HitResult hit;
        if (Stage::Instance().RayCast(start, end, hit))
        {
            range -= DirectX::XMVectorGetX(DirectX::XMVector3Length(
                DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&hit.position), DirectX::XMLoadFloat3(&end))));
            //�J�������߂Â��߂��Ȃ��悤��
            if (range < 0.001f)
            {
                range = 0.001f;
            }
            eye.x = target.x + front.x * range;
            eye.y = target.y + front.y * range;
            eye.z = target.z + front.z * -range;
        }
        else
        {
            range = beforRange;
        }
    }

    Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}

void CameraController::UpdateNormal(float elapsedTime, float range)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    float ax = gamePad.GetAxisRX();
    float ay = gamePad.GetAxisRY();
    //X,y���̉�]
    float speed = normalRollSpeed * elapsedTime;
    angle.y -= ax * speed;
    angle.x += ay * speed;

    if (angle.x < maxAngleX)
    {
        angle.x = maxAngleX;
    }
    if (angle.x > minAngleX)
    {
        angle.x = minAngleX;
    }

    if (angle.y < -DirectX::XM_PI)
    {
        angle.y += DirectX::XM_2PI;
    }
    if (angle.y > DirectX::XM_PI)
    {
        angle.y -= DirectX::XM_2PI;
    }

    //�J�����̉�]�l����]�s��ɕϊ�
    DirectX::XMMATRIX transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    //��]�s�񂩂�O�����x�N�g�������o��
    DirectX::XMVECTOR Front = transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    //���ӓ_����x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
    DirectX::XMFLOAT3 eye;
    eye.x = target.x + front.x * range;
    eye.y = target.y + front.y * range;
    eye.z = target.z + front.z * -range;

    //�J�������C�L���X�g
    if (!Debug::Instance().GetIsDebugMode())
    {
        DirectX::XMFLOAT3 start = target;
        DirectX::XMFLOAT3 end = { eye.x + front.x,eye.y + front.y,eye.z - front.z };
        HitResult hit;
        if (Stage::Instance().RayCast(start, end, hit))
        {
            range -= DirectX::XMVectorGetX(DirectX::XMVector3Length(
                DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&hit.position), DirectX::XMLoadFloat3(&end))));
            //�J�������߂Â��߂��Ȃ��悤��
            if (range < 0.001f)
            {
                range = 0.001f;
            }
            eye.x = target.x + front.x * range;
            eye.y = target.y + front.y * range;
            eye.z = target.z + front.z * -range;
        }
        else
        {
            range = beforRange;
        }
    }

    Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}

void CameraController::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(961, 201), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("CameraContollor", nullptr, ImGuiWindowFlags_None))
    {
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            //�ʒu
            //ImGui::InputFloat3("Target", &target.x);
            ImGui::SliderFloat3("Target", &target.x, -100, +100);
            //��]
            DirectX::XMFLOAT3 a;
            a.x = DirectX::XMConvertToDegrees(angle.x);
            a.y = DirectX::XMConvertToDegrees(angle.y);
            a.z = DirectX::XMConvertToDegrees(angle.z);
            ImGui::InputFloat3("angle", &a.x);
            angle.x = DirectX::XMConvertToRadians(a.x);
            angle.y = DirectX::XMConvertToRadians(a.y);
            angle.z = DirectX::XMConvertToRadians(a.z);

            //�X�P�[��
            //ImGui::InputFloat3("scale", &scale.x);
        }
    }
    ImGui::End();
}
