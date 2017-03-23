#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <fstream>
using namespace std;
using namespace DirectX;

class DeferredShaderClass
{
protected:
    struct MatrixBufferType
    {
        XMMATRIX world;
        XMMATRIX view;
        XMMATRIX projection;
    };

public:
    DeferredShaderClass();
    ~DeferredShaderClass();

    bool Init(ID3D11Device *device, HWND hwnd);
    void Shutdown();
    bool Render(ID3D11DeviceContext *deviceContext
              , int indexCount
              , XMMATRIX worldMatrix
              , XMMATRIX viewMatrix
              , XMMATRIX projMatrix
              , ID3D11ShaderResourceView *texture);

private:
    bool initShader(ID3D11Device *device, HWND hwnd, WCHAR *vs, WCHAR *ps);
    void shutdownShader();
    void outputShaderErrorMessage(ID3DBlob *blob, HWND hwnd, WCHAR *shaderFilename);

    bool setShaderParameters(ID3D11DeviceContext *deviceContext
                           , XMMATRIX worldMatrix
                           , XMMATRIX viewMatrix
                           , XMMATRIX projMatrix
                           , ID3D11ShaderResourceView *texture);
    void renderShader(ID3D11DeviceContext *deviceContext, int indexCount);

    ID3D11VertexShader *vertexShader;
    ID3D11PixelShader *pixelShader;
    ID3D11InputLayout *layout;
    ID3D11SamplerState *samplerStateWrap;
    ID3D11Buffer *matrixBuffer;
};
