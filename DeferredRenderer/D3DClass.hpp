#pragma once
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class D3DClass
{
public:
    void* operator new(size_t i)
    {
        return _mm_malloc(i, 16);
    }

    void operator delete(void* p)
    {
        _mm_free(p);
    }

    D3DClass();
    ~D3DClass();

    bool Init(int _screenWidth
            , int _screenHeight
            , bool _vsync
            , HWND _hwnd
            , bool _fullscreen
            , float _screenDepth
            , float _screenNear);
    void Shutdown();

    void BeginScene(float red, float green, float blue, float alpha);
    void EndScene();

    ID3D11Device *GetDevice();
    ID3D11DeviceContext *GetDeviceContext();

    void GetProjectionMatrix(XMMATRIX &proj);
    void GetWorldMatrix(XMMATRIX &world);
    void GetOrthoMatrix(XMMATRIX &ortho);

    void GetVideoCardInfo(char *cardName, int &memory);

    void TurnZBufferOn();
    void TurnZBufferOff();
    void TurnOnAlphaBlending();
    void TurnOffAlphaBlending();
    void TurnOnCulling();
    void TurnOffCulling();

    void SetBackBufferRenderTarget();
    void ResetViewport();

private:
    bool vsyncEnabled;
    int videoCardMemory;
    char videoCardDescription[128];

    IDXGISwapChain *swapChain;
    ID3D11Device *device;
    ID3D11DeviceContext *deviceContext;
    ID3D11RenderTargetView *renderTargetView;
    ID3D11Texture2D *depthStencilBuffer;
    ID3D11DepthStencilState *depthStencilState;
    ID3D11DepthStencilView *depthStencilView;
    ID3D11RasterizerState *rasterState;
    ID3D11RasterizerState *rasterStateNoCulling;
    XMMATRIX projectionMatrix, worldMatrix, orthoMatrix;
    ID3D11DepthStencilState *depthDisabledStencilState;
    ID3D11BlendState *alphaEnabledBlendingState;
    ID3D11BlendState *alphaDisabledBlendingState;
    D3D11_VIEWPORT viewport;
};
