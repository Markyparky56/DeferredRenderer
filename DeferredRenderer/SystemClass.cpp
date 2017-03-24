#include "SystemClass.hpp"

SystemClass::SystemClass()
    : app(nullptr)
{
}

SystemClass::~SystemClass()
{
}

bool SystemClass::Init()
{
    int screenWidth, screenHeight;
    bool result;

    screenWidth = screenHeight = 0;

    InitialiseWindows(screenWidth, screenHeight);

    app = MakeUnique<AppClass>();
    if (!app.IsValid())
    {
        return false;
    }

    result = app->Init(hInstance, hwnd, screenWidth, screenHeight);
    if (!result)
    {
        return false;
    }

    return true;
}

void SystemClass::Shutdown()
{
    if (!app.IsValid())
    {
        app->Shutdown();
    }

    ShutdownWindows();
}

void SystemClass::Run()
{
    MSG msg;
    bool done, result;

    ZeroMemory(&msg, sizeof(msg));

    // Loop until there is a quit message from the window or the user
    done = false;
    while (!done)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (msg.message == WM_QUIT)
        {
            done = true;
        }
        else
        {
            result = Frame();
            if (!result)
            {
                done = true;
            }
        }
    }
}

LRESULT SystemClass::MessageHandler(HWND _hwnd, UINT _umsg, WPARAM _wparam, LPARAM _lparam)
{
    return DefWindowProc(_hwnd, _umsg, _wparam, _lparam);
}

bool SystemClass::Frame()
{
    bool result = app->Frame();
    if (!result)
    {
        return false;
    }
    return true;
}

void SystemClass::InitialiseWindows(int & _screenWidth, int & _screenHeight)
{
    WNDCLASSEX wc;
    DEVMODE dmScreenSettings;
    int posX, posY;

    // Get an external pointer to this object
    AppHandle = this;

    // Get the instance of this application
    hInstance = GetModuleHandle(NULL);

    applicationName = L"Engine";

    // Setup the windows class
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hIconSm = wc.hIcon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    wc.lpszMenuName = NULL;
    wc.lpszClassName = applicationName;
    wc.cbSize = sizeof(WNDCLASSEX);

    // Register the window class
    RegisterClassEx(&wc);

    // Determine the resoltuion of the client's desktop
    _screenWidth = GetSystemMetrics(SM_CXSCREEN);
    _screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Setup the screen settings
    if (GFullScreen)
    {
        // If full screen set the screen to maximum size
        memset(&dmScreenSettings, 9, sizeof(dmScreenSettings));
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);
        dmScreenSettings.dmPelsWidth = static_cast<unsigned long>(_screenWidth);
        dmScreenSettings.dmPelsHeight = static_cast<unsigned long>(_screenHeight);
        dmScreenSettings.dmBitsPerPel = 32;
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

        // Set the position of the window to the top left corner
        posX = posY = 0;
    }
    else
    {
        // Windowed mode
        _screenWidth = 800;
        _screenHeight = 600;

        // Place the window in the middle of the screen
        posX = (GetSystemMetrics(SM_CXSCREEN) - _screenWidth) / 2;
        posY = (GetSystemMetrics(SM_CYSCREEN) - _screenHeight) / 2;
    }

    // Create the window with the screen settings and get the handle to it
    hwnd = CreateWindowEx(WS_EX_APPWINDOW, applicationName, applicationName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, posX, posY, _screenWidth, _screenHeight, NULL, NULL, hInstance, NULL);

    // Bring the window up on the screen and set it as main focus
    ShowWindow(hwnd, SW_SHOW);
    SetForegroundWindow(hwnd);
    SetFocus(hwnd);

    // Hide the mouse cursor
    ShowCursor(false);
}

void SystemClass::ShutdownWindows()
{
    // Show the cursor
    ShowCursor(true);

    // Fix the display setting if leaving full screen mode
    if (GFullScreen)
    {
        ChangeDisplaySettings(NULL, 0);
    }

    // Remove the window
    DestroyWindow(hwnd);
    hwnd = nullptr;

    // Remove the application instance
    UnregisterClass(applicationName, hInstance);
    hInstance = nullptr;

    // Release the pointer to this class
    AppHandle = nullptr;
}

LRESULT CALLBACK WndProc(HWND _hwnd, UINT _umsg, WPARAM _wparam, LPARAM _lparam)
{
    switch (_umsg)
    {
        // Check if the window is being destroyed or closed
    case WM_CLOSE:
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        return AppHandle->MessageHandler(_hwnd, _umsg, _wparam, _lparam);
    }
}
