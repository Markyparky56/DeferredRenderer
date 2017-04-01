#include "TextureClass.hpp"

TextureClass::TextureClass()
    : texture(nullptr)
{
}

TextureClass::~TextureClass()
{
}

bool TextureClass::Init(ID3D11Device * device, WCHAR * textureFilename, TextureType _type)
{
    type = _type;

    HRESULT result;

    if (!textureFilename)
    {
        textureFilename = L"..res/DefaultDiffuse.png";
    }
    if (!doesFileExist(textureFilename))
    {
        textureFilename = L"../res/DefaultDiffuse.png";
    }

    // Check file extension for correct loading function
    std::wstring fn(textureFilename);
    std::string::size_type idx;
    std::wstring ext;

    idx = fn.rfind('.');

    if (idx != std::string::npos)
    {
        ext = fn.substr(idx + 1);
    }
    else
    {
        // No extension found, assume it's not a DDS file
    }

    // Load the texture
    if (ext == L"dds")
    {
        result = CreateDDSTextureFromFile(device, textureFilename, NULL, &texture);
        if (FAILED(result))
        {
            MessageBox(NULL, L"Error while loading DDS texture", L"Error!", MB_OK);
            return false;
        }
    }
    else
    {
        result = CreateWICTextureFromFile(device, textureFilename, NULL, &texture);
        if (FAILED(result))
        {
            MessageBox(NULL, L"Error while loading texture", L"Error!", MB_OK);
            return false;
        }
    }


    return true;
}

void TextureClass::Shutdown()
{
    if (texture)
    {
        texture->Release();
        texture = nullptr;
    }
}

ID3D11ShaderResourceView * TextureClass::GetTexture()
{
    return texture;
}

bool TextureClass::doesFileExist(const WCHAR * filename)
{
    return std::ifstream(filename).good();
}
