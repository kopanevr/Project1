#include "Window.h"

#include <iostream>
#include <cassert>

using namespace ND3D;

extern D3DData d3DData;

LRESULT CALLBACK Window::WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) // ОКОННАЯ ПРОЦЕДУРА
{
    Window* pWnd = Window::pThis;

    CONST LRESULT result = pWnd->HandleMessage(hWnd, uMsg, wParam, lParam);

    if (result != (LRESULT)0)
    {
        return result;
    }
    
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0); // ПОМЕЩЕНИЕ СООБЩЕНИЯ WM_QUIT В ОЧЕРЕДЬ СООБЩЕНИЙ ПОТОКА

        return (LRESULT)0;
    }
    
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT Window::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return uIManager.handleMessage(hWnd, uMsg, wParam, lParam);;
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

    WNDCLASSW wc = {};

    wc.lpfnWndProc = WinProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = className;

    if (RegisterClassW(&wc) == 0)
    {
        std::cerr << "Ошибка регистрации класса окна. Код ошибки: " << GetLastError() << std::endl;

        return FALSE;
    }

    return TRUE;
}

BOOL Window::CreateWindowInstance() // СОЗДАНИЕ ОКНА
{
    hWnd = CreateWindowW(
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

Window::Window(const wchar_t* className, const wchar_t* windowName)
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
    startUpFlag = FALSE;

    if (hWnd != NULL) { DestroyWindow(hWnd); }

    if (hInstance != NULL) { UnregisterClassW(className, hInstance); }
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

    if (d3DData.pD3DDevice == nullptr || d3DData.pD3DDeviceContext == nullptr)
    {
        return;
    }

    ShowWindow(hWnd, SW_SHOWNORMAL);
    UpdateWindow(hWnd); // ПЕРЕРИСОВКА ОКНА

    if (uIManager.init(hWnd, d3DData.pD3DDevice, d3DData.pD3DDeviceContext) != TRUE)
    {
        uIManager.shutDown();

        return;
    }

    MSG msg;

    BOOL state = FALSE;

    while (!state)
    {
        if (PeekMessageW(&msg, NULL, 0U, 0U, PM_REMOVE) == TRUE)
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

    d3D.CleanupDeviceD3D();
}
