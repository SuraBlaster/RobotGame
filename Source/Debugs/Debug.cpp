
#include "Debugs/Debug.h"
#include "Input/Input.h"
#include "Camera.h"
#include "imgui.h"

Debug::Debug()
{
    cameraController = std::make_unique<CameraController>();
}

bool Debug::Update(float elapsedTime)
{
    if (isDebugMode)
    {
        DirectX::XMStoreFloat3(&target,
            DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&target),
                DirectX::XMVectorScale(DirectX::XMLoadFloat3(&GetMoveVec()), elapsedTime * speed)));

        cameraController->SetTarget(target);
        cameraController->UpdateNormal(elapsedTime, 0.01f);
    }

    return isDebugMode;
}

bool Debug::Update(float elapsedTime, DirectX::XMFLOAT3 beforeTarget)
{
    if (isDebugMode)
    {
        DirectX::XMStoreFloat3(&target,
            DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&target),
                DirectX::XMVectorScale(DirectX::XMLoadFloat3(&GetMoveVec()), elapsedTime * speed)));

        cameraController->SetTarget(target);
        cameraController->UpdateNormal(elapsedTime, 0.01f);
    }
    else
    {
        target = beforeTarget;
    }

    return isDebugMode;
}

void Debug::RenderGui()
{

    ImGui::SetNextWindowPos(ImVec2(1069, 34), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(183, 130), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Debugs", nullptr, ImGuiWindowFlags_None))
    {
        if (!isDebugMode)
        {
            if (ImGui::Button("DebugModeOn"))
            {
                isDebugMode = true;
            }
        }
        else
        {
            //if (ImGui::ColorButton("DebugModeOff",{1.0f,0.0f,0.0f,0.4f}))
            //�����ꎞ�I�ɐF��ς���
            //���push������pop���Ȃ��Ɠ{���鏉���l�͂P
            ImGui::PushStyleColor(ImGuiCol_Button, { 1.0f,0.0f,0.0f,0.4f });
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 1.0f,0.6f,0.6f,1.0f });
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 1.0f,0.2f,0.2f,1.0f });
            if (ImGui::Button("DebugModeOff"))
            {
                isDebugMode = false;
            }
            ImGui::PopStyleColor(3);
        }
        if (isDebugMode)
        {
            if (ImGui::Button("SpeedDown"))
            {
                speed -= 5.0f;
            }

            ImGui::DragFloat("Speed", &speed, 0.01f);

            if (ImGui::Button("SpeedUp"))
            {
                speed += 5.0f;
            }
        }
    }
    ImGui::End();

}

DirectX::XMFLOAT3 Debug::GetMoveVec()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    //�J���������ƃX�e�B�b�N�̓��͒l�ɂ���Đi�s�������v�Z����
    Camera& camera = Camera::Instance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

    //�ړ��x�N�g����XZ���ʂɐ����ȃx�N�g���ɂȂ�悤�ɂ���
    //�J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightLength = sqrtf((cameraRightX * cameraRightX) + (cameraRightZ * cameraRightZ));
    if (cameraRightLength > 0.0f)
    {
        //�P�ʃx�N�g����
        cameraRightX /= cameraRightLength;
        cameraRightZ /= cameraRightLength;
    }

    //�J�����O�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = sqrtf((cameraFrontX * cameraFrontX) + (cameraFrontZ * cameraFrontZ));

    if (cameraFrontLength > 0.0f)
    {
        //�P�ʃx�N�g����
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;
    }

    //�i�s�x�N�g�����v�Z����
    DirectX::XMFLOAT3 vec;
    vec.x = (ax * cameraRightX) + (-ay * cameraRightZ);
    vec.z = (-ax * cameraFrontX) + (ay * cameraFrontZ);
    vec.y = 0.0f;

    if (gamePad.GetButton() & GamePad::BTN_A)
        vec.y = 0.8f;
    if (gamePad.GetButton() & GamePad::BTN_B)
        vec.y = -0.8f;


    return vec;
}
