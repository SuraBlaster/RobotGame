#pragma once

#include "Graphics/Model.h"
#include "Stage.h"

class Map2 : public Stage
{
public:
    Map2();
    ~Map2()override;


    //�X�V����
    void Update(float elapsedTime)override;

    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    //���C�L���X�g
    bool RayCast(const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end, HitResult& hit)override;

    void OpenDoor();

    void NextDoor();
private:
    
    Model* map2 = nullptr;

    int nowDoor;

    enum Animation
    {
        OPENDOOR0,
        OPENDOOR1,
        OPENDOOR2,
        OPENDOOR3,
        OPENDOOR4,
        OPENDOOR5,
        OPENDOOR6,
        OPENDOOR7,
        OPENDOOR8,
        OPENDOOR9,
        OPENDOOR10,
        OPENDOOR11,
        OPENDOOR12,
    };
};