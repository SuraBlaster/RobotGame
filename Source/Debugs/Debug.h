#pragma once

#include "DirectXMath.h"
#include "CameraController.h"
#include <memory>

class Debug
{
public:
    static Debug& Instance()
    {
        static Debug instance;
        return instance;
    }
    bool GetIsDebugMode() { return isDebugMode; }
    bool Update(float elapsedTime);
    //デバッグモードの場合true
    bool Update(float elapsedTime, DirectX::XMFLOAT3 beforeTarget);
    void RenderGui();

private:
    Debug();
    ~Debug() {}
    DirectX::XMFLOAT3 GetMoveVec();
    bool isDebugMode = false;
    float speed = 20.0f;
    DirectX::XMFLOAT3 target = { 0,0,0 };
    std::unique_ptr<CameraController> cameraController;
};
