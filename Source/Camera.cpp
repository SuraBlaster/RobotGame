#include "Camera.h"
#include "Graphics/Graphics.h"
#include "Graphics/GpuResourceUtils.h"

Camera::Camera()
{
    // ポストエフェクト初期化
    GpuResourceUtils::CreateConstantBuffer(
        Graphics::Instance().GetDevice(),
        sizeof(CBPostEffectParam),
        CBPostEffect.GetAddressOf());

    // パラメータ初期化
    posteffect.contrast = 1.3f;
    posteffect.saturation = 0.7f;
    posteffect.ColorFilter = { 0.9f, 1.0f, 1.05f };
    posteffect.chromatic_aberration = 0.01f;

}

void Camera::SetPostEffect()
{
    ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();
    // posteffect構造体の内容でコンスタントバッファ更新
    dc->UpdateSubresource(CBPostEffect.Get(), 0, 0, &posteffect, 0, 0);

    // ピクセルシェーダーSlot5に設定
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

    //ビューを逆行列化しワールド行列に戻す
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