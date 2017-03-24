#include "DeferredBufferClass.hpp"

DeferredBuffersClass::DeferredBuffersClass()
{
    for (int i = 0; i < BufferCount; i++)
    {
        renderTargetTextureArray[i] = nullptr;
        renderTargetViewArray[i] = nullptr;
        shaderResourceViewArray[i] = nullptr;
    }

    depthStencilBuffer = nullptr;
    depthStencilView = nullptr;
}

DeferredBuffersClass::~DeferredBuffersClass()
{
}

bool DeferredBuffersClass::Init(ID3D11Device * device, int texWidth, int texHeight, float screenDepth, float screenNear)
{
    D3D11_TEXTURE2D_DESC textureDesc;
    HRESULT result;
    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
    D3D11_TEXTURE2D_DESC depthBufferDesc;
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

    textureWidth = texWidth;
    textureHeight = texHeight;

    ZeroMemory(&textureDesc, sizeof(textureDesc));

    textureDesc.Width = textureWidth;
    textureDesc.Height = textureHeight;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    // Create the render target textures
    for (int i = 0; i < BufferCount; i++)
    {
        result = device->CreateTexture2D(&textureDesc, nullptr, &renderTargetTextureArray[i]);
        if (FAILED(result))
        {
            return false;
        }
    }
    
    renderTargetViewDesc.Format = textureDesc.Format;
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    renderTargetViewDesc.Texture2D.MipSlice = 0;

    // Create the render target views
    for (int i = 0; i < BufferCount; i++)
    {
        result = device->CreateRenderTargetView(renderTargetTextureArray[i], &renderTargetViewDesc, &renderTargetViewArray[i]);
        if (FAILED(result))
        {
            return false;
        }
    }

    shaderResourceViewDesc.Format = textureDesc.Format;
    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
    shaderResourceViewDesc.Texture2D.MipLevels = 1;

    // Create the shader resource views
    for (int i = 0; i < BufferCount; i++)
    {
        result = device->CreateShaderResourceView(renderTargetTextureArray[i], &shaderResourceViewDesc, &shaderResourceViewArray[i]);
        if (FAILED(result))
        {
            return false;
        }
    }

    ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

    depthBufferDesc.Width = textureWidth;
    depthBufferDesc.Height = textureHeight;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;

    // Create the depth buffer
    result = device->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);
    if (FAILED(result))
    {
        return false;
    }

    ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    // Create the depth stencil view
    result = device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);
    if (FAILED(result))
    {
        return false;
    }

    viewport.Width = static_cast<float>(textureWidth);
    viewport.Height = static_cast<float>(textureHeight);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;

    return true;
}

void DeferredBuffersClass::Shutdown()
{
    if (depthStencilView)
    {
        depthStencilView->Release();
        depthStencilView = nullptr;
    }

    if(depthStencilBuffer)
    {
        depthStencilBuffer->Release();
        depthStencilBuffer = nullptr;
    }

    for (int i = 0; i < BufferCount; i++)
    {
        if (shaderResourceViewArray[i])
        {
            shaderResourceViewArray[i]->Release();
            shaderResourceViewArray[i] = nullptr;
        }
        if (renderTargetTextureArray[i])
        {
            renderTargetTextureArray[i]->Release();
            renderTargetTextureArray[i] = nullptr;
        }
        if (renderTargetViewArray[i])
        {
            renderTargetViewArray[i]->Release();
            renderTargetViewArray[i] = nullptr;
        }
    }
}

void DeferredBuffersClass::SetRenderTargets(ID3D11DeviceContext * deviceContext)
{
    deviceContext->OMSetRenderTargets(BufferCount, renderTargetViewArray, depthStencilView);
    deviceContext->RSSetViewports(1, &viewport);
}

void DeferredBuffersClass::ClearRenderTargets(ID3D11DeviceContext * deviceContext, float red, float green, float blue, float alpha)
{
    float colour[4];

    colour[0] = red;
    colour[1] = green;
    colour[2] = blue;
    colour[3] = alpha;

    for (int i = 0; i < BufferCount; i++)
    {
        deviceContext->ClearRenderTargetView(renderTargetViewArray[i], colour);
    }

    deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.f, 0);
}

ID3D11ShaderResourceView * DeferredBuffersClass::GetShaderResourceView(int view)
{
    return shaderResourceViewArray[view];
}
