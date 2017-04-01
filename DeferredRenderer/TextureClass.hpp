#pragma once
#include <d3d11.h>
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"
#include <fstream>
#include "UniquePtr.hpp"

enum class TextureType 
{ 
    Diffuse,
    Abledo,
    Specular,
    Normal,
    Parallax,
    Roughness,
    Metallic,
    AmbientOcclusion,
    LightMap,
    CubeMap
};

using namespace DirectX;

class TextureClass
{
public:
    TextureClass();
    ~TextureClass();

    bool Init(ID3D11Device *device, WCHAR *textureFilename, TextureType _type);
    void Shutdown();

    ID3D11ShaderResourceView *GetTexture();
    TextureType GetType() { return type; }

private:
    bool doesFileExist(const WCHAR *filename);
    ID3D11ShaderResourceView *texture;
    TextureType type;
    std::wstring path;
};
using pTextureClass = UniquePtr<TextureClass>;
