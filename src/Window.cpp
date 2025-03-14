#include "Window.h"

#include <iostream>
#include <cassert>

LRESULT CALLBACK Window::WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) // ОКОННАЯ ПРОЦЕДУРА
{
    Window* pWnd = Window::pThis;

    if (pWnd != nullptr)
    {
        return pWnd->HandleMessage(hWnd, uMsg, wParam, lParam);
    }
    else
    {
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}

LRESULT Window::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    CONST LRESULT result = uIManager.handleMessage(hWnd, uMsg, wParam, lParam);

    if (result != (LRESULT)0) { return result; }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

BOOL Window::RegisterWindowClass() // РЕГИСТРАЦИЯ КЛАССА
{
    hInstance = (HINSTANCE)GetModuleHandle(NULL);

    assert(hInstance != NULL);

    if (hInstance == NULL)
    {
        std::cerr << "Ошибка получения HANDLE экземпляра приложения. Код ошибки: " << GetLastError() << std::endl;

        return FALSE;
    }

    WNDCLASSA wc = {};

    wc.lpfnWndProc = WinProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = className;

    if (RegisterClassA(&wc) == 0)
    {
        std::cerr << "Ошибка регистрации класса окна. Код ошибки: " << GetLastError() << std::endl;

        return FALSE;
    }

    return TRUE;
}

BOOL Window::CreateWindowInstance() // СОЗДАНИЕ ОКНА
{
    hWnd = CreateWindowA(
        className,
        windowName,
        WS_OVERLAPPEDWINDOW, // СТИЛЬ

        CW_USEDEFAULT, // ПОЛОЖЕНИЕ ПО X
        CW_USEDEFAULT, // ПОЛОЖЕНИЕ ПО Y
        CW_USEDEFAULT, // ШИРИНА
        CW_USEDEFAULT, // ВЫСОТА

        NULL, // ДЕСКРИПТОР РОДИТЕЛЬСКОГО ОКНА
        NULL, // ДЕСКРИПТОР МЕНЮ
        hInstance,
        NULL
    );

    assert(hWnd != NULL);

    if (hWnd == NULL)
    {
        std::cerr << "Ошибка создания окна. Код ошибки: " << GetLastError() << std::endl;

        return FALSE;
    }

    return TRUE;
}

//

Window* Window::pThis = nullptr;

//

Window::Window(const char* className, const char* windowName)
    :   hInstance(NULL),
        hWnd(NULL),
        className(className),
        windowName(windowName)
{
    Window::pThis = this;

    if (Window::RegisterWindowClass() == FALSE) // РЕГИСТРАЦИЯ КЛАССА
    {
        return;
    }

    if (Window::CreateWindowInstance() == FALSE) // СОЗДАНИЕ ОКНА
    {
        return;
    }

    startUpFlag = TRUE;
}

Window::~Window()
{
    if (hWnd != NULL) { DestroyWindow(hWnd); }

    if (hInstance != NULL) { UnregisterClassA(className, hInstance); }
}

void Window::loop() // ОСНОВНОЙ ЦИКЛ
{
    assert(startUpFlag != FALSE);

    if (startUpFlag == FALSE)
    {
        return;
    }

    if (d3D.CreateDeviceD3D(hWnd) == FALSE) // СОЗДАНИЕ УСТРОЙСТВА И ЦЕПОЧКИ БУФЕРОВ
    {
        return;
    }

    assert(ND3D::d3DData.pD3DDevice != nullptr);
    assert(ND3D::d3DData.pD3DDeviceContext != nullptr);

    if (ND3D::d3DData.pD3DDevice == nullptr || ND3D::d3DData.pD3DDeviceContext == nullptr)
    {
        return;
    }

    if (uIManager.init(hWnd, ND3D::d3DData.pD3DDevice, ND3D::d3DData.pD3DDeviceContext) != TRUE)
    {
        uIManager.shutDown();

        return;
    }

    MSG msg;

    BOOL state = FALSE;

    while (TRUE)
    {
        if (PeekMessageA(&msg, hWnd, 0U, 0U, PM_REMOVE) == TRUE)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
            {
                state = TRUE;
            }
        }

        if (state == TRUE)
        {
            break;
        }

        uIManager.run();
    }

    uIManager.shutDown();
}
