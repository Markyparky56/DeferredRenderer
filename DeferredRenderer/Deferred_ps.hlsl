Texture2D shaderTextue : register(t0);
SamplerState SampleTypeWrap : register(s0);

struct VertexToPixelConnector
{
    float4 position : SV_Position;
    float2 tex : TEXCORD0;
    float3 normal : NORMAL;
};

struct PixelOutput
{
    float4 colour : SV_Target0;
    float4 normal : SV_Target1;
};

PixelOutput main(VertexToPixelConnector input) : SV_TARGET
{
    PixelOutput output;

    output.colour = shaderTextue.Sample(SampleTypeWrap, input.tex);
    output.normal = float4(input.normal, 1.0f);

    return output;
}
