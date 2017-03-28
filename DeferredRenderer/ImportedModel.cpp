#include "ImportedModel.hpp"

ImportedModel::ImportedModel()
{
}

ImportedModel::~ImportedModel()
{
}

bool ImportedModel::Init(ID3D11Device * device, std::string path)
{
  Assimp::Importer importer;
  // Import scene from file
  const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  // Check scene completed
  if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
  {
    return false;
  }

  // Get directory path
  directory = path.substr(0, path.find_last_of('/'));

  // Process nodes (meshes) in the scene recursively
  processNode(scene->mRootNode, scene);

  return true;
}

void ImportedModel::Shutdown()
{
  for (auto mesh = meshes.begin(); mesh != meshes.end(); mesh++)
  {
    mesh->ShutdownBuffers();
  }
}

void ImportedModel::Render()
{
  for (auto mesh = meshes.begin(); mesh != meshes.end(); mesh++)
  {
    //mesh->RenderBuffers()
  }
}

void ImportedModel::loadModel(std::string path)
{

}

void ImportedModel::processNode(ID3D11Device *device, aiNode * node, const aiScene * scene)
{
  for (int i = 0; i < node->mNumMeshes; i++)
  {
    // The node object only contains indices to index the actual objects in the scene
    // The scene contains all the data, node is just to keep stuff organised
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processMesh(device, mesh, scene));
  }

  // After we've processed all of the meshes (if any) we then recursively process each of the children
  for (unsigned int i = 0; i < node->mNumChildren; i++)
  {
    processNode(device, node->mChildren[i], scene);
  }
}

ImportedMesh ImportedModel::processMesh(ID3D11Device *device, aiMesh * mesh, const aiScene * scene)
{
  std::vector<ImportedMesh::Vertex> vertices;
  std::vector<unsigned long> indices;
  std::vector<TextureClass> textures;

  for (unsigned int i = 0; i < mesh->mNumVertices; i++)
  {
    ImportedMesh::Vertex vertex;
    XMFLOAT3 v;
    // Position
    v.x = mesh->mVertices[i].x;
    v.y = mesh->mVertices[i].y;
    v.z = mesh->mVertices[i].z;
    vertex.position = v;
    // Normal
    v.x = mesh->mNormals[i].x;
    v.y = mesh->mNormals[i].y;
    v.z = mesh->mNormals[i].z;
    vertex.normal = v;
    if (mesh->mTextureCoords[0]) // Check for texture coordinates
    {
      XMFLOAT2 t;
      t.x = mesh->mTextureCoords[0][i].x;
      t.y = mesh->mTextureCoords[0][i].y;
      vertex.texCoords = t;
    }
    else
    {
      vertex.texCoords = XMFLOAT2(0.f, 0.f);
    }
    vertices.push_back(vertex);
  }

  return ImportedMesh();
}

std::vector<pTextureClass> ImportedModel::loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName)
{
  return std::vector<pTextureClass>();
}
