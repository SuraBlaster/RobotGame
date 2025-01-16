#include "Graphics/Graphics.h"
#include "EffectManager.h"

void EffectManager::Initialize()
{
    Graphics& graphics = Graphics::Instance();

    //Effekseerレンダラ作成
    effekseerRenderer = EffekseerRendererDX11::Renderer::Create(graphics.GetDevice(),
        graphics.GetDeviceContext(), 2048);

    //Effekseerマネージャー作成
    effekseerManager = Effekseer::Manager::Create(2048);

    //Effekseerレンダラの各種設定（特殊な設定をしない場合は以下の設定でOK）
    effekseerManager->SetSpriteRenderer(effekseerRenderer->CreateSpriteRenderer());
    effekseerManager->SetRibbonRenderer(effekseerRenderer->CreateRibbonRenderer());
    effekseerManager->SetRingRenderer(effekseerRenderer->CreateRingRenderer());
    effekseerManager->SetTrackRenderer(effekseerRenderer->CreateTrackRenderer());
    effekseerManager->SetModelRenderer(effekseerRenderer->CreateModelRenderer());

    //Effekseer内でのローダーの設定（特別な設定をしない場合はry）
    effekseerManager->SetTextureLoader(effekseerRenderer->CreateTextureLoader());
    effekseerManager->SetModelLoader(effekseerRenderer->CreateModelLoader());
    effekseerManager->SetMaterialLoader(effekseerRenderer->CreateMaterialLoader());

    //Effekseerを左手座標系で計算する
    effekseerManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
}

void EffectManager::Finalize()
{
    //スマポで破棄されるので何もしない
}

void EffectManager::Update(float elapsedTime)
{
    //エフェクト更新処理
    effekseerManager->Update(elapsedTime * 60.0f);
}

void EffectManager::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    //ビューとプロジェクション行列をEffekseerレンダラに設定
    effekseerRenderer->SetCameraMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&view));
    effekseerRenderer->SetProjectionMatrix(*reinterpret_cast<const
        Effekseer::Matrix44*>(&projection));

    //Effekseer描画開始
    effekseerRenderer->BeginRendering();

    //Effekseer描画実行
    effekseerManager->Draw();

    //Effekseer描画終了
    effekseerRenderer->EndRendering();
}

