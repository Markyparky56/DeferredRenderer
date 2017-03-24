cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
}

struct VertexInput
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct VertexToPixelConnector
{
    float4 position : SV_Position;
    float2 tex : TEXCORD0;
    float3 normal : NORMAL;
};

VertexToPixelConnector main(VertexInput input)
{
    VertexToPixelConnector output;

    input.position.w = 1.0f;

    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    output.position = mul(mul(mul(input.position, worldMatrix), viewMatrix), projectionMatrix);    
    output.tex = input.tex;    
    output.normal = mul(input.normal, (float3x3)worldMatrix);    
    output.normal = normalize(output.normal);

    return output;
}
