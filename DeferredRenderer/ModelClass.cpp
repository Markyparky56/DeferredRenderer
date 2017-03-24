#include "ModelClass.hpp"

ModelClass::ModelClass()
    : vertexBuffer(nullptr)
    , indexBuffer(nullptr)
    , texture(nullptr)
    , model(nullptr)
{
}

ModelClass::~ModelClass()
{
}

bool ModelClass::Init(ID3D11Device * device, char * modelFilename, WCHAR * textureFilename)
{
    bool result;

    // Load model data
    result = LoadModel(modelFilename);
    if (!result)
    {
        return false;
    }

    // Init vertex and index buffers
    result = InitBuffers(device);
    if (!result)
    {
        return false;
    }

    // Load texture
    result = LoadTexture(device, textureFilename);
    if (!result)
    {
        return false;
    }

    return true;
}

void ModelClass::Shutdown()
{
    ReleaseTexture();
    ShutdownBuffers();
    ReleaseModel();
}

void ModelClass::Render(ID3D11DeviceContext * deviceContext)
{
    RenderBuffers(deviceContext);
}

int ModelClass::GetIndexCount()
{
    return indexCount;
}

ID3D11ShaderResourceView * ModelClass::GetTexture()
{
    return texture->GetTexture();
}

bool ModelClass::InitBuffers(ID3D11Device * device)
{
    VertexType *vertices;
    unsigned long *indices;
    D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
    HRESULT result;

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

    // Load the vertex array and index array with data
    for (int i = 0; i < vertexCount; i++)
    {
        vertices[i].position = XMFLOAT3(model[i].x, model[i].y, model[i].z);
        vertices[i].texture = XMFLOAT2(model[i].tU, model[i].tV);
        vertices[i].normal = XMFLOAT3(model[i].nX, model[i].nY, model[i].nZ);

        indices[i] = i;
    }

    // Setup the description of the static vertex buffer
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

void ModelClass::ShutdownBuffers()
{
    if (indexBuffer)
    {
        indexBuffer->Release();
        indexBuffer = nullptr;
    }

    if (vertexBuffer)
    {
        vertexBuffer->Release();
        vertexBuffer = nullptr;
    }
}

void ModelClass::RenderBuffers(ID3D11DeviceContext * deviceContext)
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

bool ModelClass::LoadTexture(ID3D11Device * device, WCHAR * textureFilename)
{
    bool result;

    texture = new TextureClass;
    if (!texture)
    {
        return false;
    }

    // Init the texture object
    result = texture->Init(device, textureFilename);
    if (!result)
    {
        return false;
    }

    return true;
}

void ModelClass::ReleaseTexture()
{
    if (texture)
    {
        texture->Shutdown();
        delete texture;
        texture = nullptr;
    }
}

// The RasterTek model file is a bit basic...
bool ModelClass::LoadModel(char * modelFilename)
{
    ifstream fin;
    char input;

    // Open the model file
    fin.open(modelFilename);

    if (fin.fail())
    {
        return false;
    }

    // Read up to the vertex count
    fin.get(input);
    while (input != ':')
    {
        fin.get(input);
    }

    // Read the vertex count
    fin >> vertexCount;

    // Vertices == Indices
    indexCount = vertexCount;

    model = new ModelType[vertexCount];
    if (!model)
    {
        return false;
    }

    // Read up to the beginning of the data
    fin.get(input);
    while (input != ':')
    {
        fin.get(input);
    }
    fin.get(input);
    fin.get(input);

    // Read in vertex data
    for (int i = 0; i < vertexCount; i++)
    {
        fin >> model[i].x >> model[i].y >> model[i].z;
        fin >> model[i].tU >> model[i].tV;
        fin >> model[i].nX >> model[i].nY >> model[i].nZ;
    }

    fin.close();


    return true;
}

void ModelClass::ReleaseModel()
{
    if (model)
    {
        delete[] model;
        model = nullptr;
    }
}
