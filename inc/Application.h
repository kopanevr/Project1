#pragma once

#include "Window.h"

#include <Windows.h>

class Application
{
private:
    Window window = Window(L"SampleClassName", L"SampleWindowName");
private:
    BOOL startUpFlag = FALSE;
public:
    Application();
    ~Application();

    void run();
};
