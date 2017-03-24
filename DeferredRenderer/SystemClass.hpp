#pragma once
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "AppClass.hpp"

using pAppClass = UniquePtr<AppClass>;

class SystemClass
{
public:
    SystemClass();
    ~SystemClass();

    bool Init();
    void Shutdown();
    void Run();

    LRESULT CALLBACK MessageHandler(HWND _hwnd, UINT _umsg, WPARAM _wparam, LPARAM _lparam);

private:
    bool Frame();
    void InitialiseWindows(int &_screenWidth, int &_screenHeight);
    void ShutdownWindows();

    LPCWSTR applicationName;
    HINSTANCE hInstance;
    HWND hwnd;
    pAppClass app;
};

static LRESULT CALLBACK WndProc(HWND _hwnd, UINT _umsg, WPARAM _wparam, LPARAM _lparam);
static SystemClass *AppHandle = nullptr;
