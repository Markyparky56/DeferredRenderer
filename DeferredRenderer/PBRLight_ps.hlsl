// PBR Lighting Pixel Shader

Texture2D abledoTexture      : register(t0);
Texture2D normalTexture     : register(t1);
Texture2D metallicTexture   : register(t2);
Texture2D roughnessTexture  : register(t3);
Texture2D aoTexture         : register(t4);

SamplerState SampleTypePoint : register(s0);

cbuffer LightBuffer
{
    float3 lightPos[4];
    float3 lightColour[4];
};

cbuffer CameraBuffer
{
    float3 camPos;
    float padding; 
};

static const float PI = 3.14159265359;

struct VertexToPixelConnector
{
    float4 worldPos : TEXCOORD0;
    float4 position : SV_Position;
    float2 tex : TEXCOORD;
};

// Cook-Torrance BRDF functions
float3 fresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float DistributionGGX(float3 N, float3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float numerator = a2;
    float denominator = (NdotH2 * (a2 - 1.0) + 1.0);
    denominator = PI * denominator * denominator;

    return numerator / denominator;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float numerator = NdotV;
    float denominator = NdotV * (1.0 - k) + k;

    return numerator / denominator;
}

float GeometrySmith(float3 N, float3 V, float L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

float4 main(VertexToPixelConnector input) : SV_Target
{
    float3 abledo;
    float3 normal;
    float metallic;
    float roughness;
    float ao;

    abledo = abledoTexture.Sample(SampleTypePoint, input.tex);
    normal = normalTexture.Sample(SampleTypePoint, input.tex);
    metallic = metallicTexture.Sample(SampleTypePoint, input.tex).x;
    roughness = roughnessTexture.Sample(SampleTypePoint, input.tex).x;
    ao = aoTexture.Sample(SampleTypePoint, input.tex).x;

    float3 N = normalize(normal);
    float3 V = normalize(camPos - input.worldPos.xyz);

    float3 Lo = float(0.0).xxx;
    for (int i = 0; i < 4; ++i) // For each light
    {
        float3 L = normalize(lightPos[i] - input.worldPos.xyz);
        float3 H = normalize(V + L);

        float distance = length(lightPos[i] - input.worldPos.xyz);
        float attenuation = 1.0 / (distance * distance);

        float3 radiance = lightColour[i] * attenuation;

        float3 F0 = float3(0.04);
        F0 = lerp(F0, abledo, metallic);
        float3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

        float NDF = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);

        float3 numerator = NDF * G * F;
        float denominator = (4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0)) + 0.001; // (0.001 avoids dividing by zero)
        float3 brdf = numerator / denominator;

        float3 kS = F; // Specular component
        float3 kD = float(1.0).xxx - kS; // Diffuse component, by conservation

        kD = mul(kD, (1.0 - metallic));

        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * abledo / PI + brdf) * radiance * NdotL;
    }

    float3 ambient = float(0.03).xxx * abledo * ao; // 0.03 ambient light should be replaced by some external modifier
    float3 colour = ambient + Lo;

    return colour;
}
