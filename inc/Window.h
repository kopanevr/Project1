#pragma once

#include "D3D.h"
#include "UIManager.h"

#include <Windows.h>

class Window final
{
private:
    static LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // ОКОННАЯ ПРОЦЕДУРА

    LRESULT HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#if __cppcpp > 201703L
    [[nodiscard]]
#endif
    BOOL RegisterWindowClass(); // РЕГИСТРАЦИЯ КЛАССА

#if __cppcpp > 201703L
    [[nodiscard]]
#endif
    BOOL CreateWindowInstance(); // СОЗДАНИЕ ОКНА
private:
    HINSTANCE hInstance; // HANDLE ЭКЗЕМПЛЯРА ПРИЛОЖЕНИЯ
    HWND hWnd; // HANDLE ОКНА

    const wchar_t* className = nullptr; // ИМЯ КЛАССА ОКНА
    const wchar_t* windowName = nullptr; // ИМЯ ОКНА

    static Window* pThis;

    BOOL startUpFlag = FALSE;

    ND3D::D3D d3D;
    UIManager uIManager;
public:
    Window(const wchar_t* className, const wchar_t* windowName);
    ~Window();

    void loop(); // ОСНОВНОЙ ЦИКЛ
};
