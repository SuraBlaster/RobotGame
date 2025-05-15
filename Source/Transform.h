#pragma once
#include <DirectXMath.h>

class Transform
{
public:
    Transform();

    void drawDebugGUI(const char* header_name, bool default_open = false);

    void updateMatrix();

    //�I�C���[�p�̎擾
    DirectX::XMFLOAT3 getAngle() const;

    //�I�C���[�p�ŃZ�b�g
    void setAngle(const DirectX::XMFLOAT3& angle);

    const DirectX::XMFLOAT4X4& getMatrix() const { return matrix; }

    // �E�����擾
    DirectX::XMFLOAT3 getRight() const;

    // �������擾
    DirectX::XMFLOAT3 getLeft() const;

    // ������擾
    DirectX::XMFLOAT3 getUp() const;

    // �������擾
    DirectX::XMFLOAT3 getDown() const;

    // �O�����擾
    DirectX::XMFLOAT3 getFront() const;

    // ������擾
    DirectX::XMFLOAT3 getBack() const;

    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT4 rotation;
    DirectX::XMFLOAT3 scale;

private:
    DirectX::XMFLOAT4X4 matrix;
};