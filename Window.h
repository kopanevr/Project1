#pragma once

#include <Windows.h>

class Window final
{
private:
    static LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // ОКОННАЯ ПРОЦЕДУРА

    LRESULT HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    BOOL RegisterWindowClass(); // РЕГИСТРАЦИЯ КЛАССА

    BOOL CreateWindowInstance(); // СОЗДАНИЕ ОКНА
private:
    HINSTANCE hInstance; // HANDLE ЭКЗЕМПЛЯРА ПРИЛОЖЕНИЯ
    HWND hWnd; // HANDLE ОКНА

    const char* className; // ИМЯ КЛАССА ОКНА
    const char* windowName; // ИМЯ ОКНА

    static Window* pThis;

    BOOL startUpFlag = FALSE;
public:
    Window(const char* className, const char* windowName);
    ~Window();

    void loop(); // ОСНОВНОЙ ЦИКЛ
};
