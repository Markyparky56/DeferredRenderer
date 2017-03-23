#pragma once
#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>

class InputClass
{
public:
    InputClass();
    ~InputClass();

    bool Init(HINSTANCE hInstance, HWND _hwnd, int _screenWidth, int _screenHeight);
    void Shutdown();
    bool Frame();

    void GetMouseLocation(int &_mouseX, int &_mouseY);

    bool IsEscapePressed();
    bool IsLeftPressed();
    bool IsRightPressed();
    bool IsUpPressed();
    bool IsDownPressed();
    bool IsAPressed();
    bool IsZPressed();
    bool IsPgUpPressed();
    bool IsPgDownPressed();
    bool IsKeyPressed(unsigned char _key);

private:
    bool ReadKeyboard();
    bool ReadMouse();
    void ProcessInput();

    IDirectInput8 *directInput;
    IDirectInputDevice8 *keyboard;
    IDirectInputDevice8 *mouse;

    unsigned char keyboardState[256];
    DIMOUSESTATE mouseState;

    int screenWidth, screenHeight;
    int mouseX, mouseY;
};