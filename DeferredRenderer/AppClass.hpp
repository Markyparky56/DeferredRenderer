#pragma once
const bool GFullScreen = true;
const bool GVsyncEnabled = true;
const float GScreenDepth = 1000.f;
const float GScreenNear = 0.1;

#include "InputClass.hpp"
#include "D3DClass.hpp"
#include "CameraClass.hpp"
#include "LightClass.hpp"
#include "ModelClass.hpp"
#include "OrthoWindowClass.hpp"
#include "DeferredBufferClass.hpp"
#include "DeferredShaderClass.hpp"
#include "LightShaderClass.hpp"

class AppClass
{
public:
    AppClass();
    ~AppClass();

    bool Init(HINSTANCE hInstance, HWND _hwnd, int _screenWidth, int _screenHeight);
    void Shutdown();
    bool Frame();

private:
    bool render();
    bool renderSceneToTexture();

    InputClass *input;
    D3DClass *D3D;
    CameraClass *camera;
    LightClass *light;
    ModelClass *model;
    OrthoWindowClass *fullScreenWindow;
    DeferredBuffersClass *deferredBuffers;
    DeferredShaderClass *deferredShader;
    LightShaderClass *lightShader;
};
