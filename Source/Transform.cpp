#include "Transform.h"
#include <imgui.h>

bool quaternionToRollPitchYaw(const DirectX::XMFLOAT4& q, float& pitch, float& yaw, float& roll);
bool matrixToRollPitchYaw(const DirectX::XMFLOAT4X4& m, float& pitch, float& yaw, float& roll);
DirectX::XMFLOAT3 getDirection(const DirectX::XMFLOAT4& rotation, const DirectX::XMFLOAT3& local_direction);

Transform::Transform():
    position(0.0f, 0.0f, 0.0f),
    rotation(0.0f, 0.0f, 0.0f, 1.0f),
    scale(1.0f, 1.0f, 1.0f),
    matrix(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f)
{
}

void Transform::drawDebugGUI(const char* header_name, bool default_open)
{

    //if (ImGui::CollapsingHeader(header_name, default_open ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None))
    if (ImGui::Begin(header_name))
    {
        ImGui::PushID(this);
        if (ImGui::Button("Reset"))
        {
            position = { 0.0f, 0.0f, 0.0f };
            rotation = {0.0f, 0.0f, 0.0f, 1.0f};
            scale = { 1.0f, 1.0f, 1.0f };
        }
        // 位置
        if (ImGui::DragFloat3("Position", &position.x, 0.5f))
        {
            updateMatrix();
        }
        // 回転
        DirectX::XMFLOAT3 angle = getAngle();
        angle.x = DirectX::XMConvertToDegrees(angle.x);
        angle.y = DirectX::XMConvertToDegrees(angle.y);
        angle.z = DirectX::XMConvertToDegrees(angle.z);
        if (ImGui::DragFloat3("Angle", &angle.x, 1.0f))
        {
            angle.x = DirectX::XMConvertToRadians(angle.x);
            angle.y = DirectX::XMConvertToRadians(angle.y);
            angle.z = DirectX::XMConvertToRadians(angle.z);
            setAngle(angle);
            updateMatrix();
        }
        if (ImGui::DragFloat4("Rotation", &rotation.x, 0.01f))
        {
            updateMatrix();
        }
        // スケール
        if (ImGui::DragFloat3("Scale", &scale.x, 0.01f))
        {
            updateMatrix();
        }

        ImGui::PopID();
    }
    ImGui::End();
}

void Transform::updateMatrix()
{
    DirectX::XMStoreFloat4(
        &rotation, DirectX::XMQuaternionNormalize(DirectX::XMLoadFloat4(&rotation)));
    DirectX::XMMATRIX Matrix = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&rotation));
    Matrix.r[0] = DirectX::XMVectorScale(Matrix.r[0], scale.x);
    Matrix.r[1] = DirectX::XMVectorScale(Matrix.r[1], scale.y);
    Matrix.r[2] = DirectX::XMVectorScale(Matrix.r[2], scale.z);
    Matrix.r[3] = DirectX::XMVectorSet(position.x, position.y, position.z, 1.0f);
    DirectX::XMStoreFloat4x4(&matrix, Matrix);
}

DirectX::XMFLOAT3 Transform::getAngle() const
{
    DirectX::XMFLOAT3 angle;
    quaternionToRollPitchYaw(rotation, angle.x, angle.y, angle.z);
    return angle;
    //return Matrix::quaternionToEuler(_rotation);
}

void Transform::setAngle(const DirectX::XMFLOAT3& angle)
{
    DirectX::XMStoreFloat4(
        &rotation , DirectX::XMQuaternionRotationRollPitchYaw(angle.x, angle.y, angle.z));
}

DirectX::XMFLOAT3 Transform::getRight() const
{
    return getDirection(rotation, { 1, 0, 0 });
}

DirectX::XMFLOAT3 Transform::getLeft() const
{
    return getDirection(rotation, { -1, 0, 0 });
}

DirectX::XMFLOAT3 Transform::getUp() const
{
    return getDirection(rotation, { 0, 1, 0 });
}

DirectX::XMFLOAT3 Transform::getDown() const
{
    return getDirection(rotation, { 0, -1, 0 });
}

DirectX::XMFLOAT3 Transform::getFront() const
{
    return getDirection(rotation, { 0, 0, 1 });
}

DirectX::XMFLOAT3 Transform::getBack() const
{
    return getDirection(rotation, { 0, 0, -1 });
}

bool quaternionToRollPitchYaw(const DirectX::XMFLOAT4& q, float& pitch, float& yaw, float& roll)
{
    DirectX::XMMATRIX M = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&q));
    DirectX::XMFLOAT4X4 m;
    DirectX::XMStoreFloat4x4(&m, M);
    return matrixToRollPitchYaw(m, pitch, yaw, roll);
}

bool matrixToRollPitchYaw(const DirectX::XMFLOAT4X4& m, float& pitch, float& yaw, float& roll)
{
    float x_radian = asinf(-m._32);
    pitch = x_radian;
    if (x_radian < DirectX::XM_PI / 2.0f)
    {
        if (x_radian > -DirectX::XM_PI / 2.0f)
        {
            roll = atan2f(m._12, m._22);
            yaw = atan2f(m._31, m._33);
            return true;
        }
        else
        {
            roll = (float)-atan2f(m._13, m._11);
            yaw = 0.0f;
            return false;
        }
    }
    else
    {
        roll = (float)atan2f(m._13, m._11);
        yaw = 0.0f;
        return false;
    }
}

DirectX::XMFLOAT3 getDirection(const DirectX::XMFLOAT4& rotation, const DirectX::XMFLOAT3& local_direction)
{
    DirectX::XMFLOAT3 out;
    DirectX::XMStoreFloat3(&out, DirectX::XMVector3Rotate(DirectX::XMLoadFloat3(&local_direction), DirectX::XMLoadFloat4(&rotation)));
    return out;
}