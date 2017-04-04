// PBR Lighting Vertex Shader

cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct VertexInput
{
    float4 position : POSITION;
    float2 tex : TEXCOORD;
};

struct VertexToPixelConnector
{
    float4 worldPos : TEXCOORD0;
    float4 position : SV_Position;
    float2 tex : TEXCOORD;
};

VertexToPixelConnector main(VertexInput input)
{
    VertexToPixelConnector output;

    input.position.w = 1.0f;
    output.worldPos = mul(input.position, worldMatrix);
    output.position = mul(mul(mul(input.position, worldMatrix), viewMatrix), projectionMatrix);
    output.tex = input.tex;

    return output;
}
