#include "Map1.h"

//コンストラクタ
Map1::Map1()
{

    map1 = new Model("Data/Model/ExampleStage/map1.mdl");

    //model = new Model("Data/Model/ExampleStage/tile2.mdl");

    position = {0,9,0};
    scale.x = scale.y = scale.z = 0.01f;
}

//デストラクタ
Map1::~Map1()
{
    //ステージモデルを破棄

    delete map1;
}

//更新処理
void Map1::Update(float elapsedTime)
{
    UpdateTransform();

    map1->UpdateAnimation(elapsedTime);

    map1->UpdateTransform(transform);

    
}

void Map1::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //シェーダーにモデルを描画してもらう
    shader->Draw(dc, map1);
}

bool Map1::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, map1, hit);
}

void Map1::OpenDoor()
{
    map1->PlayAnimation(OPENDOOR,false);

}