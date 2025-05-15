#include "SelectStage2.h"
#include "StageManager.h"

//コンストラクタ
SelectStage2::SelectStage2()
{
    model = new Model("Data/Model/SelectStage/SelectMap2.mdl");

    scale.y = scale.z = 0.006f;
    scale.x = 0.0013f;
}

//デストラクタ
SelectStage2::~SelectStage2()
{
    //ステージモデルを破棄
    delete model;
}

//更新処理
void SelectStage2::Update(float elapsedTime)
{
    SceneSelect::Stage stage = StageManager::Instance().GetStage();
    ButtonFlag = StageManager::Instance().GetButtonFlag();

    switch (stage)
    {
    case SceneSelect::Stage::Stage1:
        turnSpeed = DirectX::XMConvertToRadians(1);
        scale.y = scale.z = 0.006f;
        scale.x = 0.0013f;
        break;
    case SceneSelect::Stage::Stage2:
        if (ButtonFlag)
        {
            turnSpeed = DirectX::XMConvertToRadians(3);
        }
        else
        {
            turnSpeed = DirectX::XMConvertToRadians(1);
        }
        
        scale.y = scale.z = 0.007f;
        scale.x = 0.0014f;
        break;

    }

    position.y += sinf(angle.y * 0.5f) * 0.025f;
    angle.y += turnSpeed;

    UpdateTransform();

    model->UpdateTransform(transform);
}

void SelectStage2::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //シェーダーにモデルを描画してもらう
    shader->Draw(dc, model);
}

bool SelectStage2::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}