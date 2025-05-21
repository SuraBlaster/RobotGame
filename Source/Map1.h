
#pragma once

#include "Graphics/Model.h"
#include "Stage.h"

class Map1 : public Stage
{
public:
    Map1();
    ~Map1()override;


    //�X�V����
    void Update(float elapsedTime)override;

    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    //���C�L���X�g
    bool RayCast(const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end, HitResult& hit)override;

    void OpenDoor();
private:

    Model* map1 = nullptr;

    bool HitFlag;

    enum Animation
    {
        OPENDOOR
    };
};