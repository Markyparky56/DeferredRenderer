#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class OrthoWindowClass
{
private:
    struct VertexType
    {
        XMFLOAT3 position;
        XMFLOAT2 texture;
    };

public:
    OrthoWindowClass();
    ~OrthoWindowClass();

    bool Init(ID3D11Device *device, int _windowWidth, int _windowHeight);
    void Shutdown();
    void Render(ID3D11DeviceContext *deviceContext);

    int GetIndexCount();

private:
    bool InitBuffers(ID3D11Device *device, int _windowWidth, int _windowHeight);
    void ShutdownBuffers();
    void RenderBuffers(ID3D11DeviceContext *deviceContext);

    ID3D11Buffer *vertexBuffer, *indexBuffer;
    int vertexCount, indexCount;
};

