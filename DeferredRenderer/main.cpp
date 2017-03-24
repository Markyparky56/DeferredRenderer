#include "SystemClass.hpp"
#include "UniqueObj.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
    UniqueObj<SystemClass> system;
    bool result;

    // Init and run
    result = system->Init();
    if (result)
    {
        system->Run();
    }

    // Shutdown and exit
    system->Shutdown();
    return 0;
}