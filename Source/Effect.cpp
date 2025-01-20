#include "Graphics/Graphics.h"
#include "Effect.h"
#include "EffectManager.h"
#include <mutex>

Effect::Effect(const char* filename)
{
    std::lock_guard<std::mutex> lock(Graphics::Instance().GetMutex());

    //Effekseerのリソースを読み込む
    //Effekseerに合うように文字コードに変換
    char16_t utf16Filename[256];
    Effekseer::ConvertUtf8ToUtf16(utf16Filename, 256, filename);

    //Effekseer::Managerを取得
    Effekseer::ManagerRef effekseerManager = EffectManager::Instance().GetEffekseerManager();

    //Effekseerエフェクトを読み込み
    effekseerEffect = Effekseer::Effect::Create(effekseerManager, (EFK_CHAR*)utf16Filename);

}

Effekseer::Handle Effect::Play(const DirectX::XMFLOAT3& position, float scale)
{
    Effekseer::ManagerRef effekseerManager = EffectManager::Instance().GetEffekseerManager();

    Effekseer::Handle handle = effekseerManager->Play(effekseerEffect, position.x, position.y, position.z);

    effekseerManager->SetScale(handle, scale, scale, scale);
    return handle;
}

void Effect::Stop(Effekseer::Handle handle)
{
    Effekseer::ManagerRef effekseerManager = EffectManager::Instance().GetEffekseerManager();

    effekseerManager->StopEffect(handle);
}

void Effect::SetPosition(Effekseer::Handle handle, const DirectX::XMFLOAT3& position)
{
    Effekseer::ManagerRef effekseerManager = EffectManager::Instance().GetEffekseerManager();

    effekseerManager->SetLocation(handle, position.x, position.y, position.z);
}

void Effect::SetScale(Effekseer::Handle handle, const DirectX::XMFLOAT3& scale)
{
    Effekseer::ManagerRef effekseerManager = EffectManager::Instance().GetEffekseerManager();

    effekseerManager->SetScale(handle, scale.x, scale.y, scale.z);
}

void Effect::SetEffectColor(Effekseer::Handle handle, const Effekseer::Color& color) 
{
    Effekseer::ManagerRef effekseerManager = EffectManager::Instance().GetEffekseerManager();

    if (handle >= 0) 
    {
        effekseerManager->SetAllColor(handle, color); // エフェクト全体の色を設定
    }
}

void Effect::UpdateEffectColor(Effekseer::Handle handle, float time) 
{

    Effekseer::ManagerRef effekseerManager = EffectManager::Instance().GetEffekseerManager();

    // 時間に応じて色を変更 (虹色のループ)
    int red = static_cast<int>((sin(time) + 1.0f) * 127.5f);
    int green = static_cast<int>((sin(time + 2.0f) + 1.0f) * 127.5f);
    int blue = static_cast<int>((sin(time + 4.0f) + 1.0f) * 127.5f);

    Effekseer::Color dynamicColor(red, green, blue, 255); // RGB動的変化
    Effect::SetEffectColor(handle, dynamicColor);
}
