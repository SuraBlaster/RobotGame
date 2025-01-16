#include "StageMain.h"



//コンストラクタ
StageMain::StageMain()
{
    model = new Model("Data/Model/ExampleStage/ExampleStage.mdl");
}

//デストラクタ
StageMain::~StageMain()
{
    //ステージモデルを破棄
    delete model;
}

//更新処理
void StageMain::Update(float elapsedTime)
{
    //やること名塩
}

void StageMain::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //シェーダーにモデルを描画してもらう
    shader->Draw(dc, model);
}

bool StageMain::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}
