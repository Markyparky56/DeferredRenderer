#include "DeferredShaderClass.hpp"

DeferredShaderClass::DeferredShaderClass()
    : vertexShader(nullptr)
    , pixelShader(nullptr)
    , layout(nullptr)
    , samplerStateWrap(nullptr)
    , matrixBuffer(nullptr)
{
}

DeferredShaderClass::~DeferredShaderClass()
{
}

bool DeferredShaderClass::Init(ID3D11Device * device, HWND hwnd)
{
    return initShader(device, hwnd, L"shaders/Deferred_vs.hlsl", L"shaders/Deferred_ps.hlsl");
}

void DeferredShaderClass::Shutdown()
{
    shutdownShader();
}

bool DeferredShaderClass::Render(ID3D11DeviceContext * deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projMatrix, ID3D11ShaderResourceView * texture)
{
    bool result = setShaderParameters(deviceContext, worldMatrix, viewMatrix, projMatrix, texture);
    if (!result) return false;
    renderShader(deviceContext, indexCount);
    return true;
}

bool DeferredShaderClass::initShader(ID3D11Device * device, HWND hwnd, WCHAR * vs, WCHAR * ps)
{
    HRESULT result;
    ID3DBlob *errorMessage, *vertexShaderBuffer, *pixelShaderBuffer;
    D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
    unsigned int numElements;
    D3D11_SAMPLER_DESC samplerDesc;
    D3D11_BUFFER_DESC matrixBufferDesc;

    errorMessage = nullptr;
    vertexShaderBuffer = nullptr;
    pixelShaderBuffer = nullptr;

    result = D3DCompileFromFile(vs, nullptr, nullptr, "main", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
    if (FAILED(result))
    {
        if (errorMessage)
        {
            outputShaderErrorMessage(errorMessage, hwnd, vs);
        }
        else
        {
            MessageBox(hwnd, vs, L"Missing Shader File", MB_OK);
        }
        return false;
    }

    result = D3DCompileFromFile(ps, nullptr, nullptr, "main", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
    if (FAILED(result))
    {
        if (errorMessage)
        {
            outputShaderErrorMessage(errorMessage, hwnd, ps);
        }
        else
        {
            MessageBox(hwnd, ps, L"Missing Shader File", MB_OK);
        }
        return false;
    }

    result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, &vertexShader);
    if (FAILED(result))
    {
        return false;
    }
    result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, &pixelShader);
    if (FAILED(result))
    {
        return false;
    }

    // Vertex Input Layout Description
    polygonLayout[0].SemanticIndex = 0;
    polygonLayout[0].SemanticName = "POSITION";
    polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[0].InputSlot = 0;
    polygonLayout[0].AlignedByteOffset = 0;
    polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[0].InstanceDataStepRate = 0;

    polygonLayout[1].SemanticIndex = 0;
    polygonLayout[1].SemanticName = "TEXCOORD";
    polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
    polygonLayout[1].InputSlot = 0;
    polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[1].InstanceDataStepRate = 0;

    polygonLayout[2].SemanticIndex = 0;
    polygonLayout[2].SemanticName = "NORMAL";
    polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[2].InputSlot = 0;
    polygonLayout[2].AlignedByteOffset = 0;
    polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[2].InstanceDataStepRate = 0;

    numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

    result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &layout);

    if (FAILED(result))
    {
        return false;
    }

    vertexShaderBuffer->Release();
    vertexShaderBuffer = nullptr;
    pixelShaderBuffer->Release();
    pixelShaderBuffer = nullptr;

    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    result = device->CreateSamplerState(&samplerDesc, &samplerStateWrap);
    if (FAILED(result))
    {
        return false;
    }

    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBufferDesc.MiscFlags = 0;
    matrixBufferDesc.StructureByteStride = 0;

    result = device->CreateBuffer(&matrixBufferDesc, nullptr, &matrixBuffer);
    if (FAILED(result))
    {
        return false;
    }
    return true;
}

void DeferredShaderClass::shutdownShader()
{
    if (matrixBuffer)
    {
        matrixBuffer->Release();
        matrixBuffer = nullptr;
    }

    if (samplerStateWrap)
    {
        samplerStateWrap->Release();
        samplerStateWrap = nullptr;
    }

    if (layout)
    {
        layout->Release();
        layout = nullptr;
    }

    if (pixelShader)
    {
        pixelShader->Release();
        pixelShader = nullptr;
    }

    if (vertexShader)
    {
        vertexShader->Release();
        vertexShader = nullptr;
    }
}

void DeferredShaderClass::outputShaderErrorMessage(ID3DBlob * blob, HWND hwnd, WCHAR * shaderFilename)
{
    char *compileErrors;
    unsigned long bufferSize;
    ofstream fout;

    compileErrors = static_cast<char*>(blob->GetBufferPointer());
    bufferSize = blob->GetBufferSize();

    fout.open("shader-error.txt");
    for (int i = 0; i < bufferSize; i++)
    {
        fout << compileErrors[i];
    }
    fout.close();

    blob->Release();
    blob = nullptr;

    MessageBox(hwnd, L"Error compiling shader. Check shader-error.txt for message.", shaderFilename, MB_OK);
}

bool DeferredShaderClass::setShaderParameters(ID3D11DeviceContext * deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projMatrix, ID3D11ShaderResourceView * texture)
{
    HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    MatrixBufferType *dataPtr;

    worldMatrix = XMMatrixTranspose(worldMatrix);
    viewMatrix = XMMatrixTranspose(viewMatrix);
    projMatrix = XMMatrixTranspose(projMatrix);

    result = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result))
    {
        return false;
    }
    dataPtr = static_cast<MatrixBufferType*>(mappedResource.pData);

    dataPtr->world = worldMatrix;
    dataPtr->view = viewMatrix;
    dataPtr->projection = projMatrix;

    deviceContext->Unmap(matrixBuffer, 0);

    ID3D11Buffer *vsBuffers[] = { matrixBuffer };
    deviceContext->VSSetConstantBuffers(0, 1, vsBuffers);

    ID3D11ShaderResourceView *psSrvs[] = { texture };
    deviceContext->PSSetShaderResources(0, 1, psSrvs);

    return true;
}

void DeferredShaderClass::renderShader(ID3D11DeviceContext * deviceContext, int indexCount)
{
    deviceContext->IASetInputLayout(layout);
    deviceContext->VSSetShader(vertexShader, NULL, 0);
    deviceContext->PSSetShader(pixelShader, NULL, 0);

    deviceContext->PSSetSamplers(0, 1, &samplerStateWrap);
    
    deviceContext->DrawIndexed(indexCount, 0, 0);

    ID3D11ShaderResourceView *nullSrvs[] = { nullptr };
    deviceContext->PSSetShaderResources(0, 1, nullSrvs);
}
