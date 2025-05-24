#include "Map2.h"

//�R���X�g���N�^
Map2::Map2()
{
    
    map2 = new Model("Data/Model/ExampleStage/map2.mdl");

    //model = new Model("Data/Model/ExampleStage/tile2.mdl");

   position = {80,1,0};
    scale.x = scale.y = scale.z = 0.005f;
    

    nowDoor = 0;
}

//�f�X�g���N�^
Map2::~Map2()
{
    //�X�e�[�W���f����j��
   
    delete map2;
}

//�X�V����
void Map2::Update(float elapsedTime)
{
    UpdateTransform();

    map2->UpdateAnimation(elapsedTime);

    map2->UpdateTransform(transform);
}

void Map2::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //�V�F�[�_�[�Ƀ��f����`�悵�Ă��炤
    shader->Draw(dc, map2);
}

bool Map2::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, map2, hit);
}

void Map2::OpenDoor1()
{
   
        map2->PlayAnimation(OPENDOOR5, false);
   
}
void Map2::OpenDoor2()
{
        map2->PlayAnimation(OPENDOOR9, false);
}

