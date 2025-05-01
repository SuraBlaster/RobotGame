#include "Camera.h"
#include "Graphics/Graphics.h"
#include "Graphics/GpuResourceUtils.h"

Camera::Camera()
{
    // �|�X�g�G�t�F�N�g������
    GpuResourceUtils::CreateConstantBuffer(
        Graphics::Instance().GetDevice(),
        sizeof(CBPostEffectParam),
        CBPostEffect.GetAddressOf());

    // �p�����[�^������
    posteffect.contrast = 1.3f;
    posteffect.saturation = 0.7f;
    posteffect.ColorFilter = { 0.9f, 1.0f, 1.05f };
    posteffect.chromatic_aberration = 0.01f;

}

void Camera::SetPostEffect()
{
    ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();
    // posteffect�\���̂̓��e�ŃR���X�^���g�o�b�t�@�X�V
    dc->UpdateSubresource(CBPostEffect.Get(), 0, 0, &posteffect, 0, 0);

    // �s�N�Z���V�F�[�_�[Slot5�ɐݒ�
    dc->PSSetConstantBuffers(5, 1, CBPostEffect.GetAddressOf());

}


void Camera::SetLookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus, const
    DirectX::XMFLOAT3& up)
{
    DirectX::XMVECTOR Eye = DirectX::XMLoadFloat3(&eye);
    DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&focus);
    DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);
    DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye, Focus, Up);
    DirectX::XMStoreFloat4x4(&view, View);

    //�r���[���t�s�񉻂����[���h�s��ɖ߂�
    DirectX::XMMATRIX World = DirectX::XMMatrixInverse(nullptr, View);
    DirectX::XMFLOAT4X4 world;
    DirectX::XMStoreFloat4x4(&world, World);

    this->right.x = world._11;
    this->right.y = world._12;
    this->right.z = world._13;

    this->up.x = world._21;
    this->up.y = world._22;
    this->up.z = world._23;

    this->front.x = world._31;
    this->front.y = world._32;
    this->front.z = world._33;

    this->eye = eye;
    this->focus = focus;
}

void Camera::SetParspectiveFov(float  forY, float aspect, float nearZ, float farZ)
{
    DirectX::XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(forY, aspect, nearZ, farZ);
    DirectX::XMStoreFloat4x4(&projection, Projection);
}