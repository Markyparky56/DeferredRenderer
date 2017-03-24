#pragma once
const bool GFullScreen = false;
const bool GVsyncEnabled = true;
const float GScreenDepth = 1000.f;
const float GScreenNear = 0.1f;

#include "InputClass.hpp"
#include "D3DClass.hpp"
#include "CameraClass.hpp"
#include "LightClass.hpp"
#include "ModelClass.hpp"
#include "OrthoWindowClass.hpp"
#include "DeferredBufferClass.hpp"
#include "DeferredShaderClass.hpp"
#include "LightShaderClass.hpp"

#include "UniquePtr.hpp"
using pInputClass = UniquePtr<InputClass>;
using pD3DClass = UniquePtr<D3DClass>;
using pCameraClass = UniquePtr<CameraClass>;
using pLightClass = UniquePtr<LightClass>;
using pModelClass = UniquePtr<ModelClass>;
using pOrthoWindowClass = UniquePtr<OrthoWindowClass>;
using pDeferredBuffersClass = UniquePtr<DeferredBuffersClass>;
using pDeferredShaderClass = UniquePtr<DeferredShaderClass>;
using pLightShaderClass = UniquePtr<LightShaderClass>;

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

    pInputClass input;
    pD3DClass D3D;
    pCameraClass camera;
    pLightClass light;
    pModelClass model;
    pOrthoWindowClass fullScreenWindow;
    pDeferredBuffersClass deferredBuffers;
    pDeferredShaderClass deferredShader;
    pLightShaderClass lightShader;
};
