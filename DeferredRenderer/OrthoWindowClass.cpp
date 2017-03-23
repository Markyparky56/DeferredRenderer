#include "OrthoWindowClass.hpp"

OrthoWindowClass::OrthoWindowClass()
    : vertexBuffer(nullptr)
    , indexBuffer(nullptr)
{
}

OrthoWindowClass::~OrthoWindowClass()
{
}

bool OrthoWindowClass::Init(ID3D11Device * device, int _windowWidth, int _windowHeight)
{
    bool result;

    result = InitBuffers(device, _windowWidth, _windowHeight);
    if (!result)
    {
        return false;
    }

    return true;
}

void OrthoWindowClass::Shutdown()
{
    ShutdownBuffers();
}

void OrthoWindowClass::Render(ID3D11DeviceContext * deviceContext)
{
    RenderBuffers(deviceContext);
}

int OrthoWindowClass::GetIndexCount()
{
    return indexCount;
}

bool OrthoWindowClass::InitBuffers(ID3D11Device * device, int _windowWidth, int _windowHeight)
{
    float left, right, top, bottom;
    VertexType *vertices;
    unsigned long* indices;
    D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
    HRESULT result;

    // Calculate the screen coordinates of the left side of the window
    left = static_cast<float>((_windowWidth / 2)*-1);

    // Same for the right side of the window
    right = left + static_cast<float>(_windowWidth);

    // Calculate the screen coordinates of the top of the window
    top = static_cast<float>(_windowHeight / 2);

    // Same for the bottom of the window
    bottom = top - static_cast<float>(_windowHeight);

    // Set the number of vertices in the array
    vertexCount = 6;

    // Set the number of indices in the index array
    indexCount = vertexCount;

    // Create the vertex array
    vertices = new VertexType[vertexCount];
    if (!vertices)
    {
        return false;
    }

    // Create the index array
    indices = new unsigned long[indexCount];
    if (!indices)
    {
        return false;
    }

    // Load the vertex array with data
    // First triangle
    vertices[0].position = XMFLOAT3(left, top, 0.f); // Top Left
    vertices[0].texture = XMFLOAT2(0.f, 0.f);

    vertices[1].position = XMFLOAT3(right, bottom, 0.f); // Bottom Right
    vertices[1].texture = XMFLOAT2(1.f, 1.f);

    vertices[2].position = XMFLOAT3(left, bottom, 0.f); // Bottom Left
    vertices[2].texture = XMFLOAT2(0.f, 1.f);

    // Second triangle
    vertices[3].position = XMFLOAT3(left, top, 0.f); // Top Left
    vertices[3].texture = XMFLOAT2(0.f, 0.f);

    vertices[4].position = XMFLOAT3(right, top, 0.f); // Top Right
    vertices[4].texture = XMFLOAT2(1.f, 0.f);

    vertices[5].position = XMFLOAT3(right, bottom, 0.f); // Bottom Right
    vertices[5].texture = XMFLOAT2(1.f, 1.f);

    // Load the index array with data
    for (int i = 0; i < indexCount; i++)
    {
        indices[i] = i;
    }

    // Setup the description of the vertex buffer
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    // Point the subresource structure at the vertex data
    vertexData.pSysMem = vertices;
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    // Create the vertex buffer
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
    if (FAILED(result))
    {
        return false;
    }

    // Setup the description of the index buffer
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    // Point the subresource structure at the index data
    indexData.pSysMem = indices;
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
    if (FAILED(result))
    {
        return false;
    }

    delete[] vertices;
    vertices = nullptr;

    delete[] indices;
    indices = nullptr;

    return true;
}

void OrthoWindowClass::ShutdownBuffers()
{
    if (indexBuffer)
    {
        indexBuffer->Release();
        indexBuffer = nullptr;
    }

    if (vertexBuffer)
    {
        indexBuffer->Release();
        indexBuffer = nullptr;
    }
}

void OrthoWindowClass::RenderBuffers(ID3D11DeviceContext * deviceContext)
{
    unsigned int stride, offset;

    stride = sizeof(VertexType);
    offset = 0;

    // Set the vertex buffer to active so it can be rendered
    deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

    // Set the index buffer to active too
    deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered (triangle list)
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
