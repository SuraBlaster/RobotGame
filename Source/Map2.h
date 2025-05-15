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
private:
    
    Model* map2 = nullptr;
};