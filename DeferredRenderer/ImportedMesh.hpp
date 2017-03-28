#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "TextureClass.hpp"
#include <vector>

using namespace DirectX;

class ImportedMesh
{
public:
  struct Vertex
  {
    XMFLOAT3 position;
    XMFLOAT3 normal;
    XMFLOAT2 texCoords;
  };
  
public:
  std::vector<Vertex> vertices;
  std::vector<unsigned long> indices;
  std::vector<TextureClass> textures;

  ImportedMesh();
  bool Init(ID3D11Device *device, std::vector<Vertex> &_vertices, std::vector<unsigned long> &_indices, std::vector<TextureClass> &_textures);
  void ShutdownBuffers();
  void RenderBuffers(ID3D11DeviceContext *deviceContext);
  
private:
  ID3D11Buffer *vertexBuffer, *indexBuffer;
  int indexCount;

  bool setupMesh(ID3D11Device *device);
};
