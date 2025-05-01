//PostEffectPS
cbuffer CBPostEffect : register(b5)
{
    float _contrast;
    float _saturation;
    float _chromatic_aberration;
    float dummy; //4��؂�p

    float3 _ColorFilter;
    float dummy2; //4��؂�p
}

struct VS_OUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

Texture2D spriteTexture : register(t0);
SamplerState spriteSampler : register(s0);

// �R���g���X�g(���Â̍�)
float3 Contrast(float3 color)
{
    //float _contrast = 1.3;
    color -= 0.5; //��������ǂꂾ���Â����邢�H
    color *= _contrast; //��������̖��Â�contrast�{
    color += 0.5;
    return color;
}

// �ʓx
float3 Saturation(float3 color)
{
    //float _saturation = 0.5;
    float gray = 0.299 * color.r
        + 0.587 * color.g
        + 0.114 * color.b;
    color -= gray; //�O���[����́}
    color *= _saturation;
    color += gray;
    return color;
}

//�J���[�t�B���^�[
float3 ColorFilter(float3 color)
{
    //float3 _ColorFilter = { 0.9, 1.0, 1.05 };
    color *= _ColorFilter;
    return color;
}

float3 ChromaticAberration(float2 uv)
{
    //float _chromatic_aberration = 0.02;
    // ���S����̍����l�������Y��
    float2 v = (0.5 - uv) * _chromatic_aberration; //���炷����
    float r = spriteTexture.Sample(spriteSampler, uv).r;
    float g = spriteTexture.Sample(spriteSampler, uv + v).g;
    float b = spriteTexture.Sample(spriteSampler, uv + v * 2.0).b;

    return float3(r, g, b);
}

// �s�N�Z���V�F�[�_�[�G���g���|�C���g
float4 main(VS_OUT pin) : SV_TARGET
{
    float4 tex = spriteTexture.Sample(spriteSampler, pin.texcoord);
    //tex.rgb = ChromaticAberration(pin.texcoord);
    tex.a = 1.0;

    float3 color = tex.rgb;
    color = Contrast(color);
    color = Saturation(color);
    color = ColorFilter(color);
    return float4(color, 1.0f);
}