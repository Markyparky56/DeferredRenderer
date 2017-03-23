#include "D3DClass.hpp"

D3DClass::D3DClass()
    : swapChain(nullptr)
    , device(nullptr)
    , deviceContext(nullptr)
    , renderTargetView(nullptr)
    , depthStencilBuffer(nullptr)
    , depthStencilState(nullptr)
    , depthStencilView(nullptr)
    , rasterState(nullptr)
    , rasterStateNoCulling(nullptr)
    , depthDisabledStencilState(nullptr)
    , alphaEnabledBlendingState(nullptr)
    , alphaDisabledBlendingState(nullptr)
{
}

D3DClass::~D3DClass()
{
}

bool D3DClass::Init(int _screenWidth
                  , int _screenHeight
                  , bool _vsync
                  , HWND _hwnd
                  , bool _fullscreen
                  , float _screenDepth
                  , float _screenNear)
{
    HRESULT result;
    IDXGIFactory *factory;
    IDXGIAdapter *adapter;
    IDXGIOutput *adapterOutput;
    unsigned int numModes, numerator, denominator, stringLength;
    DXGI_MODE_DESC *displayModeList;
    DXGI_ADAPTER_DESC adapterDesc;
    int error;
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    D3D_FEATURE_LEVEL featureLevel;
    ID3D11Texture2D *backBufferPtr;
    D3D11_TEXTURE2D_DESC depthBufferDesc;
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    D3D11_RASTERIZER_DESC rasterDesc;
    float fieldOfView, screenAspect;
    D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
    D3D11_BLEND_DESC blendStateDescription;

    // Store vsync setting
    vsyncEnabled = _vsync;

    // Create a DirectX Graphics Interface Factory
    result = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory));
    if (FAILED(result))
    {
        return false;
    }

    // Usethe factory to create an adapter for the primary graphics interface (the video card)
    result = factory->EnumAdapters(0, &adapter);
    if (FAILED(result))
    {
        return false;
    }

    // Enumerate the primary adapter output (the montior)
    result = adapter->EnumOutputs(0, &adapterOutput);
    if (FAILED(result))
    {
        return false;
    }

    // Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the monitor
    result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
    if (FAILED(result))
    {
        return false;
    }

    // Create a list to hold all the possible display modes for this monitor/video card combination
    displayModeList = new DXGI_MODE_DESC[numModes];
    if (!displayModeList)
    {
        return false;
    }

    // Fill the display mode list structures (same function call but with out new list)
    result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
    if (FAILED(result))
    {
        return false;
    }

    // Search the display modes and find one which matches the specified screen width and height
    for (int i = 0; i < numModes; i++)
    {
        if (displayModeList[i].Width == static_cast<unsigned int>(_screenWidth)
         && displayModeList[i].Height == static_cast<unsigned int>(_screenHeight))
        {
            numerator = displayModeList[i].RefreshRate.Numerator;
            denominator = displayModeList[i].RefreshRate.Denominator;
        }
    }

    // Get the video card description
    result = adapter->GetDesc(&adapterDesc);
    if (FAILED(result))
    {
        return false;
    }

    // Store the dedicated VRAM in megabytes
    videoCardMemory = static_cast<int>(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

    // Convert the name of the video card to a char array
    error = wcstombs_s(&stringLength, videoCardDescription, 128, adapterDesc.Description, 128);
    if (error != 0)
    {
        return false;
    }

    delete[] displayModeList;
    displayModeList = nullptr;

    adapterOutput->Release();
    adapterOutput = nullptr;

    adapter->Release();
    adapter = nullptr;

    factory->Release();
    factory = nullptr;

    // Init swap chain description
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

    // Single back buffer
    swapChainDesc.BufferCount = 1;

    // Screen size
    swapChainDesc.BufferDesc.Width = _screenWidth;
    swapChainDesc.BufferDesc.Height = _screenHeight;

    // Regular 32-bit
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    if (vsyncEnabled)
    {
        swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
    }
    else
    {
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    }

    // Set back buffer usage
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

    // Set the handle for the window to render to
    swapChainDesc.OutputWindow = _hwnd;

    // Turn multisampling off
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;

    // Set to full screen or windowed mode
    swapChainDesc.Windowed = !_fullscreen;

    // Set the scan line order and scalling to unspecified
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    // Discard the back buffer's content after presenting
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    // No advanced flags
    swapChainDesc.Flags = 0;
    
    // Set feature level to Dx11
    featureLevel = D3D_FEATURE_LEVEL_11_0;

    // Create the swap chain, Direct3D device and Direct3D device context
    result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, NULL, &deviceContext);
    if (FAILED(result))
    {
        return false;
    }

    // Get the pointer to the back buffer
    result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&backBufferPtr));
    if (FAILED(result))
    {
        return false;
    }

    // Create the render target view with the back buffer pointer
    result = device->CreateRenderTargetView(backBufferPtr, NULL, &renderTargetView);
    if (FAILED(result))
    {
        return false;
    }

    backBufferPtr->Release();
    backBufferPtr = nullptr;

    // Initialise the description of the depth buffer
    ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

    depthBufferDesc.Width = _screenWidth;
    depthBufferDesc.Height = _screenHeight;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;

    // Create the texture for the depth buffer
    result = device->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);
    if (FAILED(result))
    {
        return false;
    }

    // Initialise the description of the stencil state
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;

    // Stencil ops if pixel is front-facing
    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Stencil operations if pixel is back-facing
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Create the depth stencil state
    result = device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
    if (FAILED(result))
    {
        return false;
    }

    deviceContext->OMSetDepthStencilState(depthStencilState, 1);

    // Initialise the depth stencil view
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

    // Bind the render target view and depth stencil buffer
    deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

    // Setup the raster description which will determine how and what polygons will be drawn
    rasterDesc.AntialiasedLineEnable = false;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.DepthClipEnable = true;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.FrontCounterClockwise = false;
    rasterDesc.MultisampleEnable = false;
    rasterDesc.ScissorEnable = false;
    rasterDesc.SlopeScaledDepthBias = 0.0f;

    // Create the raster state
    result = device->CreateRasterizerState(&rasterDesc, &rasterState);
    if (FAILED(result))
    {
        return false;
    }

    deviceContext->RSSetState(rasterState);

    // Setup the raster description which turns off back face culling

    rasterDesc.CullMode = D3D11_CULL_NONE;
    
    result = device->CreateRasterizerState(&rasterDesc, &rasterStateNoCulling);
    if (FAILED(result))
    {
        return false;
    }

    // Setup the viewport for rendering
    viewport.Width = static_cast<float>(_screenWidth);
    viewport.Height = static_cast<float>(_screenHeight);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;

    // Create the viewport
    deviceContext->RSSetViewports(1, &viewport);

    // Setup the projection matrix
    fieldOfView = static_cast<float>(XM_PI) / 4.f;
    screenAspect = static_cast<float>(_screenWidth) / static_cast<float>(_screenHeight);

    // Create the projection matrix
    projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, _screenNear, _screenDepth);

    // Init the world matrix to the identity matrix
    worldMatrix = XMMatrixIdentity();

    // Create an orthographic projection matrix for 2D rendering
    orthoMatrix = XMMatrixOrthographicLH(static_cast<float>(_screenWidth), static_cast<float>(_screenHeight), _screenNear, _screenDepth);

    // Create a second depth stencil state with the Z Buffer Off for 2D rendering
    depthDisabledStencilDesc = depthStencilDesc;
    depthDisabledStencilDesc.DepthEnable = false;

    result = device->CreateDepthStencilState(&depthDisabledStencilDesc, &depthDisabledStencilState);
    if (FAILED(result))
    {
        return false;
    }

    // Initialise the blend state description
    ZeroMemory(&blendStateDescription, sizeof(blendStateDescription));

    blendStateDescription.AlphaToCoverageEnable = TRUE;
    blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
    blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

    result = device->CreateBlendState(&blendStateDescription, &alphaEnabledBlendingState);
    if (FAILED(result))
    {
        return false;
    }

    blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

    result = device->CreateBlendState(&blendStateDescription, &alphaDisabledBlendingState);
    if (FAILED(result))
    {
        return false;
    }

    return true;
}

void D3DClass::Shutdown()
{
    if (swapChain)
    {
        swapChain->SetFullscreenState(false, NULL);
    }

    if (alphaEnabledBlendingState)
    {
        alphaEnabledBlendingState->Release();
        alphaEnabledBlendingState = nullptr;
    }

    if (alphaDisabledBlendingState)
    {
        alphaDisabledBlendingState->Release();
        alphaDisabledBlendingState = nullptr;
    }

    if (depthDisabledStencilState)
    {
        depthDisabledStencilState->Release();
        depthDisabledStencilState = nullptr;
    }

    if (rasterStateNoCulling)
    {
        rasterStateNoCulling->Release();
        rasterStateNoCulling = nullptr;
    }

    if (rasterState)
    {
        rasterState->Release();
        rasterState = nullptr;
    }

    if (depthStencilView)
    {
        depthStencilView->Release();
        depthStencilView = nullptr;
    }

    if (depthStencilState)
    {
        depthStencilState->Release();
        depthStencilState = nullptr;
    }

    if (depthStencilBuffer)
    {
        depthStencilBuffer->Release();
        depthStencilBuffer = nullptr;
    }

    if (renderTargetView)
    {
        renderTargetView->Release();
        renderTargetView = nullptr;
    }

    if (deviceContext)
    {
        deviceContext->Release();
        deviceContext = nullptr;
    }

    if (device)
    {
        device->Release();
        device = nullptr;
    }

    if (swapChain)
    {
        swapChain->Release();
        swapChain = nullptr;
    }
}

void D3DClass::BeginScene(float red, float green, float blue, float alpha)
{
    float colour[4] = { red, green, blue, alpha };

    deviceContext->ClearRenderTargetView(renderTargetView, colour);
    deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.f, 0);
}

void D3DClass::EndScene()
{
    // Present the back buffer to the scene since rendering is complete
    if (vsyncEnabled)
    {
        // Lock the screen refresh rate
        swapChain->Present(1, 0);
    }
    else
    {
        // Present as fast as possible
        swapChain->Present(0, 0);
    }
}

ID3D11Device *D3DClass::GetDevice()
{
    return device;
}

ID3D11DeviceContext *D3DClass::GetDeviceContext()
{
    return deviceContext;
}

void D3DClass::GetProjectionMatrix(XMMATRIX &proj)
{
    proj = projectionMatrix;
}

void D3DClass::GetWorldMatrix(XMMATRIX &world)
{
    world = worldMatrix;
}

void D3DClass::GetOrthoMatrix(XMMATRIX &ortho)
{
    ortho = orthoMatrix;
}

void D3DClass::GetVideoCardInfo(char *cardName, int &memory)
{
    strcpy_s(cardName, 128, videoCardDescription);
    memory = videoCardMemory;
}

void D3DClass::TurnZBufferOn()
{
    deviceContext->OMSetDepthStencilState(depthStencilState, 1);
}

void D3DClass::TurnZBufferOff()
{
    deviceContext->OMSetDepthStencilState(depthDisabledStencilState, 1);
}

void D3DClass::TurnOnAlphaBlending()
{
    // Setup the blend factor
    float blendFactor[4] = { 0.f, 0.f, 0.f, 0.f };
    // Turn on alpha blending
    deviceContext->OMSetBlendState(alphaEnabledBlendingState, blendFactor, 0xFFFFFFFF);
}

void D3DClass::TurnOffAlphaBlending()
{
    // Setup the blend factor
    float blendFactor[4] = { 0.f, 0.f, 0.f, 0.f };
    // Turn off the alpha blending
    deviceContext->OMSetBlendState(alphaDisabledBlendingState, blendFactor, 0xFFFFFFFF);
}

void D3DClass::TurnOnCulling()
{
    deviceContext->RSSetState(rasterState);
}

void D3DClass::TurnOffCulling()
{
    deviceContext->RSSetState(rasterStateNoCulling);
}

void D3DClass::SetBackBufferRenderTarget()
{
    // Bind the default render target view and depth stencil buffer to the output render pipeline
    deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
}

void D3DClass::ResetViewport()
{
    deviceContext->RSSetViewports(1, &viewport);
}
