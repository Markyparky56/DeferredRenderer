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
    input = MakeUnique<InputClass>();
    if (!input.IsValid())
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
    D3D = MakeUnique<D3DClass>();
    if (!input.IsValid())
    {
        return false;
    }

    result = D3D->Init(_screenWidth, _screenHeight, GVsyncEnabled, _hwnd, GFullScreen, GScreenDepth, GScreenNear);
    if (!result)
    {
        MessageBox(_hwnd, L"Could not initialise DirectX 11", L"Error!", MB_OK);
        return false;
    }

    // Create the camera object
    camera = MakeUnique<CameraClass>();
    if (!camera.IsValid())
    {
        return false;
    }

    // Set initial position and build matrices
    camera->SetPosition(0.f, 0.f, -10.f);
    camera->Render();
    camera->RenderBaseViewMatrix();

    // Create a light object
    // TODO: Rework for lots of lights
    light = MakeUnique<LightClass>();
    if (!light.IsValid())
    {
        return false;
    }

    // Initialise the light
    light->SetDiffuseColour(1.f, 1.f, 1.f, 1.f);
    light->SetDirection(0.f, 0.f, 1.f);

    // Create the model object
    model = MakeUnique<ModelClass>();
    if (!model.IsValid())
    {
        return false;
    }

    // Init the model
    result = model->Init(D3D->GetDevice(), "res/cube.txt", L"res/seafloor.dds");
    if (!result)
    {
        MessageBox(_hwnd, L"Could not initialise the model object", L"Error!", MB_OK);
        return false;
    }

    // Create the full screen ortho window
    fullScreenWindow = MakeUnique<OrthoWindowClass>();
    if (!fullScreenWindow.IsValid())
    {
        return false;
    }

    result = fullScreenWindow->Init(D3D->GetDevice(), _screenWidth, _screenHeight);
    if (!result)
    {
        MessageBox(_hwnd, L"Could not initialise the full screen ortho window object", L"Error!", MB_OK);
        return false;
    }

    // Create the deferred buffers object
    deferredBuffers = MakeUnique<DeferredBuffersClass>();
    if (!deferredBuffers.IsValid())
    {
        return false;
    }

    // Init the defered buffers object
    result = deferredBuffers->Init(D3D->GetDevice(), _screenWidth, _screenHeight, GScreenDepth, GScreenNear);
    if (!result)
    {
        MessageBox(_hwnd, L"Could not initialise the deferred buffers object", L"Error!", MB_OK);
        return false;
    }

    // Create the deferred shader object
    deferredShader = MakeUnique<DeferredShaderClass>();
    if (!deferredShader.IsValid())
    {
        return false;
    }

    result = deferredShader->Init(D3D->GetDevice(), _hwnd);
    if (!result)
    {
        MessageBox(_hwnd, L"Could not initialise the deferred shader object", L"Error!", MB_OK);
        return false;
    }

    // Create the light shader object
    lightShader = MakeUnique<LightShaderClass>();
    if (!lightShader.IsValid())
    {
        return false;
    }

    result = lightShader->Init(D3D->GetDevice(), _hwnd);
    if (!result)
    {
        MessageBox(_hwnd, L"Could not initialise the light shader object", L"Error", MB_OK);
    }

    return true;
}

void AppClass::Shutdown()
{
    if (!lightShader.IsValid())
    {
        lightShader->Shutdown();
    }

    if (!deferredShader.IsValid())
    {
        deferredShader->Shutdown();
    }

    if (!deferredBuffers.IsValid())
    {
        deferredBuffers->Shutdown();
    }

    if (!fullScreenWindow.IsValid())
    {
        fullScreenWindow->Shutdown();
    }

    if (!model.IsValid())
    {
        model->Shutdown();
    }

    if (!input.IsValid())
    {
        input->Shutdown();
    }

    if (!D3D.IsValid())
    {
        D3D->Shutdown();
    }
}

bool AppClass::Frame()
{
    bool result;

    // Read user input
    result = input->Frame();
    if (!result)
    {
        return false;
    }

    // Check if the user pressed escape and wants to exit the application
    if (input->IsEscapePressed() == true)
    {
        return false;
    }

    // Render the graphics
    result = render();
    if (!result)
    {
        return false;
    }

    return true;
}

bool AppClass::render()
{
    bool result;
    XMMATRIX worldMatrix, baseViewMatrix, orthoMatrix;

    // Render the scene to the deferred buffers
    result = renderSceneToTexture();
    if (!result)
    {
        return false;
    }

    // Clear the scene
    D3D->BeginScene(1.f, 0.f, 1.f, 1.f);

    // Get matrixes
    D3D->GetWorldMatrix(worldMatrix);
    camera->GetBaseViewMatrix(baseViewMatrix);
    D3D->GetOrthoMatrix(orthoMatrix);

    // Turn off Z buffer
    D3D->TurnZBufferOff();
    
    // Send the ortho window
    fullScreenWindow->Render(D3D->GetDeviceContext());

    // Render the full screen using the deferred lighting and the information we rendered to the deferred buffers
    lightShader->Render(D3D->GetDeviceContext()
                      , fullScreenWindow->GetIndexCount()
                      , worldMatrix, baseViewMatrix, orthoMatrix
                      , deferredBuffers->GetShaderResourceView(0)
                      , deferredBuffers->GetShaderResourceView(1)
                      , light->GetDirection());

    // Turn the Z Buffer back on
    D3D->TurnZBufferOn();

    // Present
    D3D->EndScene();

    return true;
}

bool AppClass::renderSceneToTexture()
{
    XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

    // Set the deferred buffers as the render target
    deferredBuffers->SetRenderTargets(D3D->GetDeviceContext());

    // Clear buffers
    deferredBuffers->ClearRenderTargets(D3D->GetDeviceContext(), 0.f, 0.f, 0.f, 1.f);

    // Get matrices
    D3D->GetWorldMatrix(worldMatrix);
    camera->GetViewMatrix(viewMatrix);
    D3D->GetProjectionMatrix(projectionMatrix);

    // Update the rotation variable each frame
    static float rotation = 0.0f;
    rotation += static_cast<float>(XM_PI) * 0.01f;
    if (rotation > 360.f)
    {
        rotation -= 360.f;
    }

    // Rotate the world matrix by the rotation value so that the cube will spin
    //worldMatrix = XMMatrixRotationY(rotation);
    XMMATRIX rotMatrix = XMMatrixRotationY(rotation);

    // Send the model
    model->Render(D3D->GetDeviceContext());

    // Render the model using the deferred shader
    deferredShader->Render(D3D->GetDeviceContext()
                         , model->GetIndexCount()
                         , worldMatrix * rotMatrix, viewMatrix, projectionMatrix
                         , model->GetTexture());

    // Reset the render target back to the back buffer
    D3D->SetBackBufferRenderTarget();

    // Reset the viewport
    D3D->ResetViewport();

    return true;
}
