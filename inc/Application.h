#pragma once

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
