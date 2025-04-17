#include "DefaultLit.hlsli"

Texture2D color_map : register(t0);
Texture2D normal_map : register(t1);

SamplerState color_sampler_state : register(s0);

Texture2D environment_map : register(t15); // Slot15 ���}�b�v
SamplerState environment_sampler_state : register(s15);

float3 IBL(float3 basecolor, float3 normal, float3 E)
{
    float roughness = 0.5;
    float metallic = 0.5;

    float3 color = 1;
    // �����̔���
    float3 R = reflect(E, normal);
    R = normalize(R);
    // �����̔��˂���UV
    float2 uv;
    //uv.y      ��0.0|-------|1.0��
    //R.y       +1.0 |-------|-1.0
    //-R.y      -1.0 |-------|+1.0
    //-R.y*0.5  -0.5 |-------|+0.5
    uv.y = -R.y * 0.5 + 0.5;

    float3 R2 = R;
    R2.y = 0;
    R2 = normalize(R2);
    uv.x = -R2.z * 0.25 + 0.25; //�k0.0|---|0.5��
    // �������̏ꍇ
    if (R.x < 0)
    {
        uv.x = 1.0 - uv.x;
    }

    // �e�N�X�`������RGB�擾
    float level = 11.0 * pow(roughness, 0.25);
    float4 env = environment_map.SampleLevel(environment_sampler_state, uv, level);
    // �����Ƃ��P���ȃg�[���}�b�v(HDR��0.0�`1.0�ɕϊ�)
    env = env / (env + 300);

    // ���^���b�N�\��
    float4 rmax = environment_map.SampleLevel(environment_sampler_state, uv, 11);
    rmax = rmax / (rmax + 300);

    color = lerp(
        rmax.rgb * basecolor,
        env.rgb * basecolor,
        metallic);
    // �J���[�𖳎������f�荞��
    color += env.rgb * 0.3 * (1.0 - metallic);

    return color;
}


float4 main(VS_OUT pin) : SV_TARGET
{
    float4 color = float4(0, 0, 0, 1);

    float4 diffuse_color = color_map.Sample(color_sampler_state, pin.texcoord);
    float4 normal = normal_map.Sample(color_sampler_state, pin.texcoord);

    normal.xyz = (normal.xyz * 2.0);

    // �m�[�}���}�b�v��ϊ�
    float3 t = normalize(pin.tangent);
    float3 b = normalize(pin.binormal);
    float3 n = normalize(pin.normal);
    // �ϊ��s��g�ݗ���
    float3x3 m = { t, b, n };
    // �ϊ�
    float3 N = mul(normal.xyz, m);
    N = normalize(N);

    // �e�X�g �k=�� ��=�� ��=��
    //return float4(N, 1);


    // L = ���C�g�̕����̐��K��
    float3 L = normalize(lightDirection.xyz);

    float3 directional_diffuse = 0;
    {
        // ���C�g�̓����� = cos = ����
        float power = saturate(dot(N, -L));
        // �n�[�t�����o�[�g
        // -1.0|------|+1.0 size=2.0
        //  0.0|------|+1.0 size=1.0
        //float power = dot(N, -L); //cos +1.0�`-1.0 
        //power = power * 0.5 + 0.5;

        directional_diffuse = lightColor.rgb * power;
    }
    // ���s���K�p
    color.rgb += diffuse_color.rgb * directional_diffuse;

    // �X�y�L����
    // ���� �J����(camera_position) �� �s�N�Z��(world_position)
    float3 E = normalize(pin.world_position.xyz - cameraPosition.xyz);
    float3 directional_specular = 0;
    {
        // �����̔���
        float3 R = reflect(L, N);
        // �����̔��˂ƃ��C�g���ǂꂾ���d�Ȃ��Ă��邩
        float power = max(dot(-E, R), 0);
        // 0.0�`1.0��pow�@0��1��ς����ɊԂ𒲐��o���� 
        power = pow(power, 30);
        directional_specular = lightColor.rgb * power;
    }
    // �X�y�L�����K�p
    color.rgb += directional_specular;

    // IBL�K�p
    color.rgb += IBL(diffuse_color.rgb, N, E);


    return color;
}
