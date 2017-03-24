#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <fstream>

using namespace std;
using namespace DirectX;

class LightShaderClass
{
protected:
    struct MatrixBufferType
    {
        XMMATRIX world;
        XMMATRIX view;
        XMMATRIX projection;
    };

    struct LightBufferType
    {
        XMFLOAT3 lightDirection;
        float padding;
    };

public:
    LightShaderClass();
    ~LightShaderClass();

    bool Init(ID3D11Device *device, HWND hwnd);
    void Shutdown();
    bool Render(ID3D11DeviceContext *deviceContext
              , int indexCount
              , const XMMATRIX &worldMatrix
              , const XMMATRIX &viewMatrix
              , const XMMATRIX &projMatrix
              , ID3D11ShaderResourceView *colourTexture
              , ID3D11ShaderResourceView *normalTexture
              , const XMFLOAT3 &lightDir);

private:
    bool initShader(ID3D11Device *device, HWND hwnd, WCHAR *vs, WCHAR *ps);
    void shutdownShader();
    void outputShaderErrorMessage(ID3DBlob *blob, HWND hwnd, WCHAR *shaderFilename);

    bool setShaderParameters(ID3D11DeviceContext *deviceContext
                           , const XMMATRIX &worldMatrix
                           , const XMMATRIX &viewMatrix
                           , const XMMATRIX &projMatrix
                           , ID3D11ShaderResourceView *colourTexture
                           , ID3D11ShaderResourceView *normalTexture
                           , const XMFLOAT3 &lightDir);
    void renderShader(ID3D11DeviceContext *deviceContext, int indexCount);

    ID3D11VertexShader *vertexShader;
    ID3D11PixelShader *pixelShader;
    ID3D11InputLayout *layout;
    ID3D11SamplerState *samplerState;
    ID3D11Buffer *matrixBuffer;
    ID3D11Buffer *lightBuffer;
};
