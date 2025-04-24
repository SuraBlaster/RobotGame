#pragma once
#include <DirectXMath.h>

class Transform
{
public:
    Transform();

    void drawDebugGUI(const char* header_name, bool default_open = false);

    void updateMatrix();

    //オイラー角の取得
    DirectX::XMFLOAT3 getAngle() const;

    //オイラー角でセット
    void setAngle(const DirectX::XMFLOAT3& angle);

    const DirectX::XMFLOAT4X4& getMatrix() const { return matrix; }

    // 右方向取得
    DirectX::XMFLOAT3 getRight() const;

    // 左方向取得
    DirectX::XMFLOAT3 getLeft() const;

    // 上方向取得
    DirectX::XMFLOAT3 getUp() const;

    // 下方向取得
    DirectX::XMFLOAT3 getDown() const;

    // 前方向取得
    DirectX::XMFLOAT3 getFront() const;

    // 後方向取得
    DirectX::XMFLOAT3 getBack() const;

    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT4 rotation;
    DirectX::XMFLOAT3 scale;

private:
    DirectX::XMFLOAT4X4 matrix;
};