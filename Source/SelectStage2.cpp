#include "SelectStage2.h"
#include "StageManager.h"
#include <Graphics/Graphics.h>
#include <Input/Input.h>
#include <Graphics/Graphics.h>
#include <Camera.h>


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
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();

    RenderContext rc;

    //カメラ初期設定
    Camera& camera = Camera::Instance();
    rc.view = camera.GetView();
    rc.projection = camera.GetProjection();

    GetMouseToStage(dc, rc.view, rc.projection);

    SceneSelect::Stage stage = StageManager::Instance().GetStage();

    CursorFlag = StageManager::Instance().GetCursorFlag2();

    if (CursorFlag)
    {
        turnSpeed += DirectX::XMConvertToRadians(3); 
    }
    else
    {
        turnSpeed += DirectX::XMConvertToRadians(1);
    }

    angle.y += turnSpeed;

    position.y += sinf(angle.y * 0.5f) * 0.025f;

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

void SelectStage2::GetMouseToStage(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    // ビューポート取得
    D3D11_VIEWPORT viewport;
    UINT numViewports = 1;
    dc->RSGetViewports(&numViewports, &viewport);

    // 行列変換の準備
    DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
    DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
    DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

    // マウス入力取得
    Mouse& mouse = Input::Instance().GetMouse();

    // マウススクリーン座標（z=0 → ニアクリップ面）
    DirectX::XMFLOAT3 screenPosition = {
        static_cast<float>(mouse.GetPositionX()),
        static_cast<float>(mouse.GetPositionY()),
        0.0f
    };

    // ニア面 → ワールド座標
    DirectX::XMVECTOR nearPoint = DirectX::XMVector3Unproject(
        DirectX::XMLoadFloat3(&screenPosition),
        viewport.TopLeftX, viewport.TopLeftY,
        viewport.Width, viewport.Height,
        0.0f, 1.0f,
        Projection, View, World
    );

    // ファー面（z = 1.0）→ ワールド座標
    screenPosition.z = 1.0f;
    DirectX::XMVECTOR farPoint = DirectX::XMVector3Unproject(
        DirectX::XMLoadFloat3(&screenPosition),
        viewport.TopLeftX, viewport.TopLeftY,
        viewport.Width, viewport.Height,
        0.0f, 1.0f,
        Projection, View, World
    );

    // レイの始点と方向
    DirectX::XMFLOAT3 start;
    DirectX::XMStoreFloat3(&start, nearPoint);

    DirectX::XMFLOAT3 endTemp;
    DirectX::XMStoreFloat3(&endTemp, farPoint);
    DirectX::XMFLOAT3 direction = {
        endTemp.x - start.x,
        endTemp.y - start.y,
        endTemp.z - start.z
    };

    // レイ方向を正規化し、長さを1000に延長
    DirectX::XMVECTOR dirVec = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&direction));
    DirectX::XMVECTOR startVec = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR extendedEndVec = DirectX::XMVectorAdd(startVec, DirectX::XMVectorScale(dirVec, 10000.0f));

    DirectX::XMFLOAT3 end;
    DirectX::XMStoreFloat3(&end, extendedEndVec);

    // ヒット判定
    HitResult hit;
    if (RayCast(start, end, hit))
    {
        // 当たった時の処理（例：スケール変更）
        scale.y = scale.z = 0.007f;
        scale.x = 0.0014f;

        if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
        {
            StageManager::Instance().SetCursorFlag2(true);
        }

    }
    else
    {
        turnSpeed += DirectX::XMConvertToRadians(1);
        scale.y = scale.z = 0.006f;
        scale.x = 0.0013f;
    }

}