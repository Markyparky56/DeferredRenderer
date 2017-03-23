#include "AppClass.hpp"

AppClass::AppClass()
    : input(nullptr)
    , D3D(nullptr)
    , camera(nullptr)
    , light(nullptr)
    , model(nullptr)
    , fullScreenWindow(nullptr)
    , deferredBuffers(nullptr)
    , deferredShader(nullptr)
    , lightShader(nullptr)
{
}

AppClass::~AppClass()
{
}

bool AppClass::Init(HINSTANCE hInstance, HWND _hwnd, int _screenWidth, int _screenHeight)
{
    bool result;

    // Create the input object
    input = new InputClass;
    if (input)
    {
        return false;
    }

    result = input->Init(hInstance, _hwnd, _screenWidth, _screenHeight);
    if (!result)
    {
        MessageBox(_hwnd, L"Could not initialise the input object", L"Error!", MB_OK);
        return false;
    }

    // Create the Direct3D object
    D3D = new D3DClass;
    if (!D3D)
    {
        return false;
    }

    result = D3D->Init(_screenWidth, _screenHeight, GVsyncEnabled, _hwnd, GFullScreen, GScreenDepth, GScreenNear);
    if (!result)
    {
        MessageBox(_hwnd, L"Could not initialise DirectX 11", L"Error!", MB_OK);
        return false;
    }



    return true;
}

void AppClass::Shutdown()
{
}

bool AppClass::Frame()
{
    return false;
}

bool AppClass::render()
{
    return false;
}

bool AppClass::renderSceneToTexture()
{
    return false;
}
