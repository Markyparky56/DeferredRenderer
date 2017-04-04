#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include "ImportedMesh.hpp"
#include "TextureClass.hpp"
#include <string>
#include <vector>
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "CharToWString.hpp"

class ImportedModel
{
public:
  ImportedModel();
  ~ImportedModel();
  bool Init(ID3D11Device *device, std::string path);
  void Shutdown();
  void Render();

private:
  // Model Data
  std::vector<ImportedMesh> meshes;
  std::vector<std::wstring> loadedTextures;
  std::string directory;

  void loadModel(std::string path);
  void processNode(ID3D11Device *device, aiNode *node, const aiScene *scene);
  ImportedMesh processMesh(ID3D11Device *device, aiMesh *mesh, const aiScene *scene);
  std::vector<TextureClass> loadMaterialTextures(ID3D11Device *device, aiMaterial *mat, aiTextureType type, TextureType texType);
};
