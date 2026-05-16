#include "Core/Application.h"
#include <iostream>
#include <stdexcept>

int main()
{
    try
    {
        Application app;
        return app.Run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Fatal Exception Occurred: " << e.what() << std::endl;
        return -1;
    }
}