#pragma once
// Defines
const int BufferCount = 2;

#include <d3d11.h>
#include <DirectXMath.h>

class DeferredBuffersClass
{
public:
    DeferredBuffersClass();
    ~DeferredBuffersClass();

    bool Init(ID3D11Device *device
            , int texWidth
            , int texHeight
            , float screenDepth
            , float screenNear);
    void Shutdown();

    void SetRenderTargets(ID3D11DeviceContext *deviceContext);
    void ClearRenderTargets(ID3D11DeviceContext *deviceContext
                          , float red
                          , float green
                          , float blue
                          , float alpha);

    ID3D11ShaderResourceView *GetShaderResourceView(int view);

private:
    int textureWidth, textureHeight;

    ID3D11Texture2D *renderTargetTextureArray[BufferCount];
    ID3D11RenderTargetView *renderTargetViewArray[BufferCount];
    ID3D11ShaderResourceView *shaderResourceViewArray[BufferCount];
    ID3D11Texture2D *depthStencilBuffer;
    ID3D11DepthStencilView *depthStencilView;
    D3D11_VIEWPORT viewport;
};
