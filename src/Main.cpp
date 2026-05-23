#include "Application/Application.h"
#include "Platform/Win32API.h"

int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int)
{
    try
    {
        Application app;
        app.Run();
    }
    catch (const std::exception& e)
    {
        MessageBoxA(nullptr, e.what(), "Critical Error", MB_ICONERROR | MB_OK);
        return -1;
    }
    return 0;
}
