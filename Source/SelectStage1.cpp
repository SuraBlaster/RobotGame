#include "SelectStage1.h"
#include "StageManager.h"
#include "SceneSelect.h"

//コンストラクタ
SelectStage1::SelectStage1()
{
    model = new Model("Data/Model/SelectStage/SelectMap1.mdl");

    scale.x = scale.y = scale.z = 0.005f;
}

//デストラクタ
SelectStage1::~SelectStage1()
{
    //ステージモデルを破棄
    delete model;
}

//更新処理
void SelectStage1::Update(float elapsedTime)
{
    SceneSelect::Stage stage = StageManager::Instance().GetStage();
    ButtonFlag = StageManager::Instance().GetButtonFlag();

    switch (stage)
    {
    case SceneSelect::Stage::Stage1:
        if (ButtonFlag)
        {
            turnSpeed = DirectX::XMConvertToRadians(3);
        }
        else
        {
            turnSpeed = DirectX::XMConvertToRadians(1);
        }
        
        scale.x = scale.y = scale.z = 0.006f;
        break;
    case SceneSelect::Stage::Stage2:
        turnSpeed = DirectX::XMConvertToRadians(1);
        scale.x = scale.y = scale.z = 0.005f;
        break;

    }
    
    position.y += sinf(angle.y * 0.5f) * 0.025f;
    angle.y += turnSpeed;

    UpdateTransform();

    model->UpdateTransform(transform);
}

void SelectStage1::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //シェーダーにモデルを描画してもらう
    shader->Draw(dc, model);
}

bool SelectStage1::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}