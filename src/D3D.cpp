#include "D3D.h"

#include <iostream>
#include <cassert>

using namespace ND3D;

BOOL D3D::CreateDeviceD3D(HWND hWnd) // СОЗДАНИЕ УСТРОЙСТВА И ЦЕПОЧКИ БУФЕРОВ
{
    DXGI_SWAP_CHAIN_DESC dsc = {0};

    //

    dsc.BufferCount = 2; // КОЛИЧЕСТВО БУФЕРОВ

    dsc.BufferDesc.Width = 0; // ШИРИНА БУФЕРА. ЗНАЧЕНИЕ 0 УКАЗЫВАЕТ, ЧТО ШИРИНА БУДЕТ АВТОМАТИЧЕСКИ ОПРЕДЕЛЕНА РАЗМЕРОМ ОКНА.
    dsc.BufferDesc.Height = 0; // ВЫСОТА БУФЕРА ЗНАЧЕНИЕ 0 УКАЗЫВАЕТ, ЧТО ВЫСОТА БУДЕТ АВТОМАТИЧЕСКИ ОПРЕДЕЛЕНА РАЗМЕРОМ ОКНА.
    dsc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // ФОРМАТ ПИКСЕЛЕЙ БУФЕРА. 8 БИТ. НОРМАЛИЗОВАННЫЕ В ДИАПАЗОН [0; 1].
    dsc.BufferDesc.RefreshRate.Numerator = 60;
    dsc.BufferDesc.RefreshRate.Denominator = 1;

    dsc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // ЦЕЛЬ БУФЕРА

    dsc.OutputWindow = hWnd;

    dsc.SampleDesc.Count = 1; // КОЛИЧЕСТВО ВЫБОРОК ДЛЯ СГЛАЖИВАНИЯ
    dsc.SampleDesc.Quality = 0; // УРОВЕНЬ КАЧЕСТВА СГЛАЖИВАНИЯ

    dsc.Windowed = TRUE; // РЕЖИМ ПРИЛОЖЕНИЯ

    dsc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // РЕЖИМ ПЕРЕКЛЮЧЕНИЯ БУФЕРОВ

    dsc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // ФЛАГИ

    //

    D3D_FEATURE_LEVEL featureLevel[] = {
        D3D_FEATURE_LEVEL_10_0
    };

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        NULL, // АДАПТЕР
        D3D_DRIVER_TYPE_HARDWARE, // ТИП ДРАЙВЕРА
        NULL,
        0, // ФЛАГИ
        featureLevel,
        ARRAYSIZE(featureLevel),
        D3D11_SDK_VERSION,
        &dsc,
        &D3DData.pIDXGISwapChain,
        &D3DData.pD3DDevice,
        &D3DData.featureLevel,
        &D3DData.pD3DDeviceContext
    );

    assert(FAILED(hr) != true);

    if (FAILED(hr) == true)
    {
        std::cerr << "Ошибка создания устройства и цепочки буферов." << std::endl;

        return FALSE;
    }

    if (D3D::CreateRenderTargetView() != TRUE) // СОЗДАНИЕ RENDER TARGET
    {
        return FALSE;
    }

    return TRUE;
}

void D3D::CleanupDeviceD3D()
{
    D3D::CleanupRenderTargetView();

    if (D3DData.pIDXGISwapChain != nullptr) { D3DData.pIDXGISwapChain->Release(); D3DData.pIDXGISwapChain = nullptr; }
    if (D3DData.pD3DDevice != nullptr) { D3DData.pD3DDevice->Release(); D3DData.pD3DDevice = nullptr; }
    if (D3DData.pD3DDeviceContext != nullptr) { D3DData.pD3DDeviceContext->Release(); D3DData.pD3DDeviceContext = nullptr; }
}

BOOL D3D::CreateRenderTargetView() // СОЗДАНИЕ RENDER TARGET VIEW
{
    ID3D11Texture2D* pBackBuffer = nullptr;

    HRESULT hr = D3DData.pIDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);

    assert(FAILED(hr) != true);

    if (FAILED(hr) == true)
    {
        std::cerr << "Ошибка получения доступа к буферу." << std::endl;

        return FALSE;
    }

    D3DData.pD3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &D3DData.pIDXGIRenderTargetView);

    pBackBuffer->Release(); // УМЕНЬШИТЬ СЧЕТЧИК ССЫЛОК

    return TRUE;
}

void D3D::CleanupRenderTargetView()
{
    if (D3DData.pIDXGIRenderTargetView != nullptr) { D3DData.pIDXGIRenderTargetView->Release(); D3DData.pIDXGIRenderTargetView = nullptr; }
}
