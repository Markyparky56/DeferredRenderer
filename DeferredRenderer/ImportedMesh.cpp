#include "ImportedMesh.hpp"

ImportedMesh::ImportedMesh()
{
}

bool ImportedMesh::Init(ID3D11Device *device, std::vector<Vertex> &_vertices, std::vector<unsigned long> &_indices, std::vector<TextureClass> &_textures)
{
  vertices = _vertices;
  indices = _indices;
  textures = _textures;

  indexCount = static_cast<int>(indices.size());

  return true;
}

void ImportedMesh::ShutdownBuffers()
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

void ImportedMesh::RenderBuffers(ID3D11DeviceContext * deviceContext)
{
  unsigned int stride, offset;

  stride = sizeof(Vertex);
  offset = 0;

  deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
  deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
  deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool ImportedMesh::setupMesh(ID3D11Device *device)
{
  // Setup vertex and index buffers
  D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
  D3D11_SUBRESOURCE_DATA vertexData, indexData;
  HRESULT result;

  if (vertices.empty())
  {
    return false;
  }

  if (indices.empty())
  {
    return false;
  }

  vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  vertexBufferDesc.ByteWidth = sizeof(Vertex) * static_cast<int>(vertices.size());
  vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vertexBufferDesc.CPUAccessFlags = 0;
  vertexBufferDesc.MiscFlags = 0;
  vertexBufferDesc.StructureByteStride = 0;

  vertexData.pSysMem = vertices.data();
  vertexData.SysMemPitch = 0;
  vertexData.SysMemSlicePitch = 0;

  result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
  if (FAILED(result))
  {
    return false;
  }

  indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  indexBufferDesc.ByteWidth = sizeof(unsigned long) * static_cast<int>(indices.size());
  indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  indexBufferDesc.CPUAccessFlags = 0;
  indexBufferDesc.MiscFlags = 0;
  indexBufferDesc.StructureByteStride = 0;

  indexData.pSysMem = indices.data();
  indexData.SysMemPitch = 0;
  indexData.SysMemSlicePitch = 0;

  result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
  if (FAILED(result))
  {
    return false;
  }

  return true;
}
