#include "Application.h"

#include "Window.h"

Application::Application()
{
    //

    startUpFlag = TRUE;
}

Application::~Application()
{
    startUpFlag = FALSE;
}

void Application::run()
{
    if (startUpFlag != TRUE)
    {
        return;
    }

    window.loop(); // ОСНОВНОЙ ЦИКЛ
}
