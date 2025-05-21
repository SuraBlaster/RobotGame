#pragma once

#include "Graphics/Model.h"
#include "Stage.h"
#include "SceneSelect.h"

class SelectStage1 : public Stage
{
public:
    SelectStage1();
    ~SelectStage1()override;


    //更新処理
    void Update(float elapsedTime)override;

    //描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    //レイキャスト
    bool RayCast(const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end, HitResult& hit)override;

    void GetMouseToStage(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view,
        const DirectX::XMFLOAT4X4& projection);
private:
    Model* model = nullptr;
    float turnSpeed;
    bool CursorFlag = false;
    
     
};