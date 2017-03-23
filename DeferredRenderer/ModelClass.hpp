#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>

#include "TextureClass.hpp"

using namespace std;
using namespace DirectX;

class ModelClass
{
private:
    struct VertexType
    {
        XMFLOAT3 position;
        XMFLOAT2 texture;
        XMFLOAT3 normal;
    };

    struct ModelType
    {
        float x, y, z;
        float tU, tV;
        float nX, nY, nZ;
    };

public:
    ModelClass();
    ~ModelClass();

    bool Init(ID3D11Device *device, char *modelFilename, WCHAR *textureFilename);
    void Shutdown();
    void Render(ID3D11DeviceContext *deviceContext);

    int GetIndexCount();
    ID3D11ShaderResourceView *GetTexture();

private:
    bool InitBuffers(ID3D11Device *device);
    void ShutdownBuffers();
    void RenderBuffers(ID3D11DeviceContext *deviceContext);

    bool LoadTexture(ID3D11Device *device, WCHAR *textureFilename);
    void ReleaseTexture();

    bool LoadModel(char *modelFilename);
    void ReleaseModel();

    ID3D11Buffer *vertexBuffer, *indexBuffer;
    int vertexCount, indexCount;
    TextureClass *texture;
    ModelType *model;
};
