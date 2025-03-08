#pragma once

#include "D3D.h" 
#include "UIManager.h"

#include <Windows.h>

class Window final
{
private:
    static LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // ОКОННАЯ ПРОЦЕДУРА

    LRESULT HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    BOOL RegisterWindowClass(); // РЕГИСТРАЦИЯ КЛАССА

    BOOL CreateWindowInstance(); // СОЗДАНИЕ ОКНА
private:
    HINSTANCE hInstance; // HANDLE ЭКЗЕМПЛЯРА ПРИЛОЖЕНИЯ
    HWND hWnd; // HANDLE ОКНА

    const char* className = nullptr; // ИМЯ КЛАССА ОКНА
    const char* windowName = nullptr; // ИМЯ ОКНА

    static Window* pThis;

    BOOL startUpFlag = FALSE;

    ND3D::D3D d3D;
    UIManager uIManager;
public:
    Window(const char* className, const char* windowName);
    ~Window();

    void loop(); // ОСНОВНОЙ ЦИКЛ
};
