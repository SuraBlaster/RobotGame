#include "Map2.h"

//コンストラクタ
Map2::Map2()
{
    
    map2 = new Model("Data/Model/ExampleStage/map2(1).mdl");

    //model = new Model("Data/Model/ExampleStage/tile2.mdl");

   // position = {1,1,0};
    scale.x = scale.y = scale.z = 0.005f;

    nowDoor = 0;
}

//デストラクタ
Map2::~Map2()
{
    //ステージモデルを破棄
   
    delete map2;
}

//更新処理
void Map2::Update(float elapsedTime)
{
    UpdateTransform();

    map2->UpdateAnimation(elapsedTime);

    map2->UpdateTransform(transform);
}

void Map2::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //シェーダーにモデルを描画してもらう
    shader->Draw(dc, map2);
}

bool Map2::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, map2, hit);
}

void Map2::OpenDoor()
{
    switch(nowDoor)
    {
    case 0:
        map2->PlayAnimation(OPENDOOR0, false);
    case 1:
        map2->PlayAnimation(OPENDOOR1, false);
    case 2:
        map2->PlayAnimation(OPENDOOR2, false);
    case 3:
        map2->PlayAnimation(OPENDOOR3, false);
    case 4:
        map2->PlayAnimation(OPENDOOR4, false);
    case 5:
        map2->PlayAnimation(OPENDOOR5, false);
    case 6:
        map2->PlayAnimation(OPENDOOR6, false);
    case 7:
        map2->PlayAnimation(OPENDOOR7, false);
    case 8:
        map2->PlayAnimation(OPENDOOR8, false);
        

    }
   

}
void Map2::NextDoor()
{
    nowDoor++;
}