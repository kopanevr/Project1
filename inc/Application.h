#pragma once

#include "Window.h"

#include <Windows.h>

class Application
{
private:
    Window window = Window("SampleClassName", "SampleWindowName");
private:
    BOOL startUpFlag = FALSE;
public:
    Application();
    ~Application();

    void run();
};
