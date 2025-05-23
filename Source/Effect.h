#pragma once

#include <DirectXMath.h>
#include <Effekseer.h>

class Effect
{
public:
    Effect(const char* filename);
    ~Effect(){}

    //再生
    Effekseer::Handle Play(const DirectX::XMFLOAT3& position,float scale = 1.0f);

    //停止
    void Stop(Effekseer::Handle handle);

    //座標指定
    void SetPosition(Effekseer::Handle handle, const DirectX::XMFLOAT3& position);

    //スケール指定
    void SetScale(Effekseer::Handle handle, const DirectX::XMFLOAT3& scale);

    //色指定
    void Effect::SetEffectColor(Effekseer::Handle handle, const Effekseer::Color& color);

    void Effect::UpdateEffectColor(Effekseer::Handle handle, float time);

private:
    Effekseer::EffectRef effekseerEffect;
};

