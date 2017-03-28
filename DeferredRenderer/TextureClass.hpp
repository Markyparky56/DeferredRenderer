#pragma once
#include <d3d11.h>
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"
#include <fstream>
#include "UniquePtr.hpp"

using namespace DirectX;

class TextureClass
{
public:
    TextureClass();
    ~TextureClass();

    bool Init(ID3D11Device *device, WCHAR *textureFilename);
    void Shutdown();

    ID3D11ShaderResourceView *GetTexture();

private:
    bool doesFileExist(const WCHAR *filename);
    ID3D11ShaderResourceView *texture;
};
using pTextureClass = UniquePtr<TextureClass>;
