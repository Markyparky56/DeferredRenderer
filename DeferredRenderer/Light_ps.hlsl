Texture2D colourTexture : register(t0);
Texture2D normalTexture : register(t1);

SamplerState SampleTypePoint : register(s0);

cbuffer LightBuffer
{
    float3 lightDirection;
    float padding;
};

struct VertexToPixelConnector
{
    float4 position : SV_Position;
    float2 tex : TEXCOORD;
};

float4 main(VertexToPixelConnector input) : SV_Target
{
    float4 colours;
    float4 normals;
    float3 lightDir;
    float lightIntensity;
    float4 outputColour;

    // Get data from the textures rendered earlier
    colours = colourTexture.Sample(SampleTypePoint, input.tex);

    normals = normalTexture.Sample(SampleTypePoint, input.tex);

    // Directional lighting calc
    lightDir = -lightDirection;
    lightIntensity = saturate(dot(normals.xyz, lightDir));
    outputColour = saturate(colours * lightIntensity);
    return outputColour;
}
