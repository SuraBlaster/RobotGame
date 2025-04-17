#include "DefaultLit.hlsli"

Texture2D color_map : register(t0);
Texture2D normal_map : register(t1);

SamplerState color_sampler_state : register(s0);

Texture2D environment_map : register(t15); // Slot15 環境マップ
SamplerState environment_sampler_state : register(s15);

float3 IBL(float3 basecolor, float3 normal, float3 E)
{
    float roughness = 0.5;
    float metallic = 0.5;

    float3 color = 1;
    // 視線の反射
    float3 R = reflect(E, normal);
    R = normalize(R);
    // 視線の反射からUV
    float2 uv;
    //uv.y      上0.0|-------|1.0下
    //R.y       +1.0 |-------|-1.0
    //-R.y      -1.0 |-------|+1.0
    //-R.y*0.5  -0.5 |-------|+0.5
    uv.y = -R.y * 0.5 + 0.5;

    float3 R2 = R;
    R2.y = 0;
    R2 = normalize(R2);
    uv.x = -R2.z * 0.25 + 0.25; //北0.0|---|0.5南
    // 西半球の場合
    if (R.x < 0)
    {
        uv.x = 1.0 - uv.x;
    }

    // テクスチャからRGB取得
    float level = 11.0 * pow(roughness, 0.25);
    float4 env = environment_map.SampleLevel(environment_sampler_state, uv, level);
    // もっとも単純なトーンマップ(HDR→0.0～1.0に変換)
    env = env / (env + 300);

    // メタリック表現
    float4 rmax = environment_map.SampleLevel(environment_sampler_state, uv, 11);
    rmax = rmax / (rmax + 300);

    color = lerp(
        rmax.rgb * basecolor,
        env.rgb * basecolor,
        metallic);
    // カラーを無視した映り込み
    color += env.rgb * 0.3 * (1.0 - metallic);

    return color;
}


float4 main(VS_OUT pin) : SV_TARGET
{
    float4 color = float4(0, 0, 0, 1);

    float4 diffuse_color = color_map.Sample(color_sampler_state, pin.texcoord);
    float4 normal = normal_map.Sample(color_sampler_state, pin.texcoord);

    normal.xyz = (normal.xyz * 2.0);

    // ノーマルマップを変換
    float3 t = normalize(pin.tangent);
    float3 b = normalize(pin.binormal);
    float3 n = normalize(pin.normal);
    // 変換行列組み立て
    float3x3 m = { t, b, n };
    // 変換
    float3 N = mul(normal.xyz, m);
    N = normalize(N);

    // テスト 北=青 東=赤 上=緑
    //return float4(N, 1);


    // L = ライトの方向の正規化
    float3 L = normalize(lightDirection.xyz);

    float3 directional_diffuse = 0;
    {
        // ライトの当たり具合 = cos = 内積
        float power = saturate(dot(N, -L));
        // ハーフランバート
        // -1.0|------|+1.0 size=2.0
        //  0.0|------|+1.0 size=1.0
        //float power = dot(N, -L); //cos +1.0～-1.0 
        //power = power * 0.5 + 0.5;

        directional_diffuse = lightColor.rgb * power;
    }
    // 平行光適用
    color.rgb += diffuse_color.rgb * directional_diffuse;

    // スペキュラ
    // 視線 カメラ(camera_position) → ピクセル(world_position)
    float3 E = normalize(pin.world_position.xyz - cameraPosition.xyz);
    float3 directional_specular = 0;
    {
        // 視線の反射
        float3 R = reflect(L, N);
        // 視線の反射とライトがどれだけ重なっているか
        float power = max(dot(-E, R), 0);
        // 0.0～1.0のpow　0と1を変えずに間を調整出来る 
        power = pow(power, 30);
        directional_specular = lightColor.rgb * power;
    }
    // スペキュラ適用
    color.rgb += directional_specular;

    // IBL適用
    color.rgb += IBL(diffuse_color.rgb, N, E);


    return color;
}
