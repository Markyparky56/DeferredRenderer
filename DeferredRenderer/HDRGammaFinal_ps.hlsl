// HDR and Gamma Correction Pixel Shader
// Applies a HDR tone mapping through exposure, 
// and gamma corrects the colour value

Texture2D texture : register(t0);
SamplerState SampleTypePoint : register(s0);

cbuffer HDRGammaBuffer
{
    float exposure; // HDR modifier, allows for dynamic scaling if so desired
    float gamma; // Gamma modifier, not all monitors are created equally
    float2 padding;
};

struct VertexToPixelConnector
{
    float4 position : SV_Position;
    float2 tex : TEXCOORD;
};

float4 main(VertexToPixelConnector input) : SV_Target
{
    // This value will be in high dynamic range (not clamped/scaled between 0 and 1)
    float3 colourValue = texture.Sample(SampleTypePoint, input.tex); 
    // Use Exposure tone mapping to bring our value within range
    float3 toneMappedValue = float(1.0).xxx - exp(-colourValue * exposure); 
    // Gamma correct our value
    float3 gammaCorrectedValue = pow(toneMappedValue, float(1.0 / gamma).xxx); 
    return float4(gammaCorrectedValue, 1.0);

}
