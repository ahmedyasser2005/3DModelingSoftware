#ifndef APP_ENTRY_POINT
#define APP_ENTRY_POINT
#endif

#include "Application/Application.h"

int main()
{
    try
    {
        Application app;
        app.Run();
    }
    catch (const std::exception& e)
    {
        MessageBoxA(nullptr, e.what(), "Critical Error Failure", MB_ICONERROR | MB_OK);
        return -1;
    }

    return 0;
}