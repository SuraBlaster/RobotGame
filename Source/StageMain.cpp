#include "StageMain.h"
#include <algorithm>
#include "Camera.h"
#include "Input/Mouse.h"
#include "Input/Input.h"
#include "Player.h"
#include "Graphics/Graphics.h"

static StageMain* instance = nullptr;

//コンストラクタ
StageMain::StageMain()
{
    instance = this;
    model = new Model("Data/Model/ExampleStage/testmap.mdl");
    transform.scale.x *= 1.5f;
    transform.scale.y *= 1.5f;
    transform.scale.z *= 1.5f;
    /*transform.scale.x *= 0.01f;
    transform.scale.y *= 0.01f;
    transform.scale.z *= 0.01f;*/
}

//デストラクタ
StageMain::~StageMain()
{
    //ステージモデルを破棄
    delete model;
}

StageMain& StageMain::Instance()
{
    return *instance;
}

//更新処理
void StageMain::Update(float elapsedTime)
{
    transform.updateMatrix();
    //if (selectedDirection != Down)

    if (isRotationAnimation)
        RotationStage(elapsedTime);

    Mouse& mouse = Input::Instance().GetMouse();
    if (mouse.GetWheel() > 0.0f)
    {
        StageMain::Instance().ChangeGravity();
    }
    //const DirectX::XMFLOAT4X4 transformIdentity = { 1,0,0,0 ,0,1,0,0 ,0,0,1,0 ,0,0,0,1 };
    model->UpdateTransform(transform.getMatrix());
}

void StageMain::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //model->UpdateTransform(transform.getMatrix());

    transform.drawDebugGUI("StageMain");

    //シェーダーにモデルを描画してもらう
    shader->Draw(dc, model);


    if (isAnten)
    {
        antenTimer++;
        Graphics& graphics = Graphics::Instance();
        ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
        ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

        alpha = antenTimer / 60;
        if (alpha != 0.0f)
        { 
            alpha = 1.0f;
            if (antenTimer >= 150.0f)
            {
                antenTimer = 0.0f;
                isAnten = false;
            }
            
            isRotationAnimation = true;
        }
    }
}
bool StageMain::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}

void StageMain::RotationStage(float elapsedTime)
{
    constexpr float radian90 = DirectX::XMConvertToRadians(90);
    DirectX::XMVECTOR q = {};

    switch (selectedDirection) {
    case YMinus:
    {
        q = DirectX::XMQuaternionRotationRollPitchYaw(0, 0, 0);
        if (Player::Instance().IsGround())
            isRotationAnimation = false;
        else
            isRotationAnimation = true;

        break;
    }
    case YPuls:
    {
        q = DirectX::XMQuaternionRotationRollPitchYaw(0, radian90 * 2, 0);
        if (Player::Instance().IsGround())
            isRotationAnimation = false;
        else
            isRotationAnimation = true;
        break;
    }
    case XMinus:
    {
        q = DirectX::XMQuaternionRotationRollPitchYaw(0, 0, -radian90);
        if (Player::Instance().IsGround())
            isRotationAnimation = false;
        else
            isRotationAnimation = true;
        break;
    }
    case XPuls:
    {
        q = DirectX::XMQuaternionRotationRollPitchYaw(0, 0, radian90);
        if (Player::Instance().IsGround())
            isRotationAnimation = false;
        else
            isRotationAnimation = true;
        break;
    }
    case ZMinus:
    {
        q = DirectX::XMQuaternionRotationRollPitchYaw(-radian90, 0, 0);
        if (Player::Instance().IsGround())
            isRotationAnimation = false;
        else
            isRotationAnimation = true;
        break;
    }
    case ZPuls:
    {
        q = DirectX::XMQuaternionRotationRollPitchYaw(radian90, 0, 0);
        if (Player::Instance().IsGround())
            isRotationAnimation = false;
        else
            isRotationAnimation = true;
        break;
    }
    }

    DirectX::XMStoreFloat4(&transform.rotation, q);
}

void StageMain::ChangeGravity()
{
    using namespace DirectX;
    if (isRotationAnimation) return;

    Camera& camera = Camera::Instance();
    XMFLOAT3 front;
   XMStoreFloat3(&front, XMVector3Normalize(XMLoadFloat3(&camera.GetFront())));

    XMFLOAT3 start = camera.GetEye();
    XMFLOAT3 end;
    DirectX::XMStoreFloat3(&end, DirectX::XMVectorScale(DirectX::XMLoadFloat3(&front), 100.0f));
    HitResult hit;

    if (RayCast(start, end, hit))
    {
        int oldSelectDirection = selectedDirection;

        XMFLOAT3 vec;
        XMStoreFloat3(&vec, XMVectorSubtract(XMLoadFloat3(&hit.position), XMLoadFloat3(&Player::Instance().GetPosition())));

        DirectX::XMFLOAT3 absVec;
        absVec.x = std::fabs(vec.x);
        absVec.y = std::fabs(vec.y);
        absVec.z = std::fabs(vec.z);

        float maxAbs = absVec.x;
        int maxIndex = 0;
        float originalValue = vec.x;

        if (absVec.y > maxAbs)
        {
            maxAbs = absVec.y;
            maxIndex = 1;
            originalValue = vec.y;
        }
        if (absVec.z > maxAbs)
        {
            maxAbs = absVec.z;
            maxIndex = 2;
            originalValue = vec.z;
        }

        if (maxAbs + originalValue == 0.0f)
        {
            maxIndex += 3;
        }

        switch (maxIndex)
        {
        case 0: selectedDirection = XPuls; break;
        case 1: selectedDirection = YPuls; break;
        case 2: selectedDirection = ZPuls; break;
        case 3: selectedDirection = XMinus; break;
        case 4: selectedDirection = YMinus; break;
        case 5: selectedDirection = ZMinus; break;
        }

        if (oldSelectDirection == selectedDirection)
        {
            selectedDirection = YPuls;
        }
        isAnten = true;
    }
}
