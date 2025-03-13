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

    dsc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // РЕЖИМ ПЕРЕКЛЮЧЕНИЯ БУФЕРОВ

    dsc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // ФЛАГИ

    //

    D3D_FEATURE_LEVEL featureLevel[] = {
        D3D_FEATURE_LEVEL_10_1
    };

    UINT createDeviceFlags = 0U;

#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        NULL, // АДАПТЕР
        D3D_DRIVER_TYPE_HARDWARE, // ТИП ДРАЙВЕРА
        NULL,
        createDeviceFlags, // ФЛАГИ
        featureLevel,
        ARRAYSIZE(featureLevel),
        D3D11_SDK_VERSION,
        &dsc,
        &d3DData.pIDXGISwapChain,
        &d3DData.pD3DDevice,
        &d3DData.featureLevel,
        &d3DData.pD3DDeviceContext
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

    if (d3DData.pIDXGISwapChain != nullptr) { d3DData.pIDXGISwapChain->Release(); d3DData.pIDXGISwapChain = nullptr; }
    if (d3DData.pD3DDeviceContext != nullptr) { d3DData.pD3DDeviceContext->Release(); d3DData.pD3DDeviceContext = nullptr; }
    if (d3DData.pD3DDevice != nullptr) { d3DData.pD3DDevice->Release(); d3DData.pD3DDevice = nullptr; }
}

BOOL D3D::CreateRenderTargetView() // СОЗДАНИЕ RENDER TARGET VIEW
{
    ID3D11Texture2D* pBackBuffer = nullptr;

    HRESULT hr = d3DData.pIDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);

    assert(FAILED(hr) != true);

    if (FAILED(hr) == true)
    {
        std::cerr << "Ошибка получения доступа к буферу." << std::endl;

        return FALSE;
    }

    d3DData.pD3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &d3DData.pIDXGIRenderTargetView);

    pBackBuffer->Release(); // УМЕНЬШИТЬ СЧЕТЧИК ССЫЛОК

    return TRUE;
}

void D3D::CleanupRenderTargetView()
{
    if (d3DData.pIDXGIRenderTargetView != nullptr) { d3DData.pIDXGIRenderTargetView->Release(); d3DData.pIDXGIRenderTargetView = nullptr; }
}
