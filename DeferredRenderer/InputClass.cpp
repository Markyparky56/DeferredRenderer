#include "InputClass.hpp"

InputClass::InputClass()
    : directInput(nullptr)
    , keyboard(nullptr)
    , mouse(nullptr)
{
}

InputClass::~InputClass()
{
}

bool InputClass::Init(HINSTANCE hInstance, HWND _hwnd, int _screenWidth, int _screenHeight)
{
    HRESULT result;

    // Store screen size which is required for positioning the mouse cursor
    screenHeight = _screenHeight;
    screenWidth = _screenWidth;

    // Initialise the location of the mouse of the screen
    mouseX = 0;
    mouseY = 0;

    // Initialise the main direct input interface
    result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void**>(&directInput), NULL);
    if (FAILED(result))
    {
        return false;
    }

    // Initialise the direct input interface for the keyboard
    result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
    if (FAILED(result))
    {
        return false;
    }

    // Set the data format
    result = keyboard->SetDataFormat(&c_dfDIKeyboard);
    if (FAILED(result))
    {
        return false;
    }

    // Set the cooperative level of the keyboard to share with other programs
    result = keyboard->SetCooperativeLevel(_hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    if (FAILED(result))
    {
        return false;
    }

    // Accquire the keyboard
    result = keyboard->Acquire();
    if (FAILED(result))
    {
        return false;
    }

    // Initialise the direct input interface for the mouse
    result = directInput->CreateDevice(GUID_SysMouse, &mouse, NULL);
    if (FAILED(result))
    {
        return false;
    }

    // Set the data format for the mouse using the pre-defined mouse data format
    result = mouse->SetDataFormat(&c_dfDIMouse);
    if (FAILED(result))
    {
        return false;
    }

    // Set the cooperative level of the mouse to share with other programs
    result = mouse->SetCooperativeLevel(_hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    if (FAILED(result))
    {
        return false;
    }

    // Accquire the mouse
    result = mouse->Acquire();
    if (FAILED(result))
    {
        return false;
    }

    return true;
}

void InputClass::Shutdown()
{
    if (mouse)
    {
        mouse->Unacquire();
        mouse->Release();
        mouse = nullptr;
    }

    if (keyboard)
    {
        keyboard->Unacquire();
        keyboard->Release();
        keyboard = nullptr;
    }

    if (directInput)
    {
        directInput->Release();
        directInput = nullptr;
    }
}

bool InputClass::Frame()
{
    bool result;

    result = ReadKeyboard();
    if (!result)
    {
        return false;
    }

    result = ReadMouse();
    if(!result)
    {
        return false;
    }

    // Process the changes in the mouse and the keyboard
    ProcessInput();

    return true;
}

void InputClass::GetMouseLocation(int & _mouseX, int & _mouseY)
{
    _mouseX = mouseX;
    _mouseY = mouseY;
}

bool InputClass::IsEscapePressed()
{
    if (keyboardState[DIK_ESCAPE] & 0x80)
    {
        return true;
    }
    return false;
}

bool InputClass::IsLeftPressed()
{
    if (keyboardState[DIK_LEFT] & 0x80)
    {
        return true;
    }
    return false;
}

bool InputClass::IsRightPressed()
{
    if (keyboardState[DIK_RIGHT] & 0x80)
    {
        return true;
    }
    return false;
}

bool InputClass::IsUpPressed()
{
    if (keyboardState[DIK_UP] & 0x80)
    {
        return true;
    }
    return false;
}

bool InputClass::IsDownPressed()
{
    if (keyboardState[DIK_DOWN] & 0x80)
    {
        return true;
    }
    return false;
}

bool InputClass::IsAPressed()
{
    if (keyboardState[DIK_A] & 0x80)
    {
        return true;
    }
    return false;
}

bool InputClass::IsZPressed()
{
    if (keyboardState[DIK_Z] & 0x80)
    {
        return true;
    }
    return false;
}

bool InputClass::IsPgUpPressed()
{
    if (keyboardState[DIK_PGUP] & 0x80)
    {
        return true;
    }
    return false;
}

bool InputClass::IsPgDownPressed()
{
    if (keyboardState[DIK_PGDN] & 0x80)
    {
        return true;
    }
    return false;
}

bool InputClass::IsKeyPressed(unsigned char _key)
{
    if (keyboardState[_key] & 0x80)
    {
        return true;
    }
    return false;
}

bool InputClass::ReadKeyboard()
{
    HRESULT result;

    result = keyboard->GetDeviceState(sizeof(keyboardState), reinterpret_cast<LPVOID>(&keyboardState));
    if (FAILED(result))
    {
        // We might have lost focus or not accquired the keyboard
        if ((result == DIERR_INPUTLOST) || (result = DIERR_NOTACQUIRED))
        {
            keyboard->Acquire();
        }
        else
        {
            return false;
        }
    }

    return true;
}

bool InputClass::ReadMouse()
{
    HRESULT result;

    result = mouse->GetDeviceState(sizeof(DIMOUSESTATE), reinterpret_cast<LPVOID>(&mouseState));
    if (FAILED(result))
    {
        // We might have lost focus or not accquired the mouse
        if ((result == DIERR_INPUTLOST) || (result = DIERR_NOTACQUIRED))
        {
            mouse->Acquire();
        }
        else
        {
            return false;
        }
    }

    return true;
}

void InputClass::ProcessInput()
{
    // Update the mouse cursor location based on the change since the last frame
    mouseX += mouseState.lX;
    mouseY += mouseState.lY;

    // Clamp the mouse inside the screen
    if (mouseX < 0) { mouseX = 0; }
    if (mouseY < 0) { mouseY = 0; }
    if (mouseX > screenWidth) { mouseX = screenWidth; }
    if (mouseY > screenHeight) { mouseY = screenHeight; }
}
