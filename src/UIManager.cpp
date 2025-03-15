#include "UIManager.h"

#include "D3D.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

using namespace ND3D;

extern D3DData d3DData;

void UIManager::body() const
{
    ImGui::SetNextWindowPos(ImVec2(600.0f, 50.0f));

    ImGui::Begin("_", nullptr,
        //

        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_AlwaysAutoResize
    ); // НАЧАЛО ОКНА

    //

    if (ImGui::CollapsingHeader("General") == true)
    {
        //
    }

    ImGui::Text("Select a vehicle to display"); // ТЕКСТ

    //

    ImGui::End(); // КОНЕЦ ОКНА
}

//

BOOL UIManager::init(HWND hWnd, ID3D11Device* pD3DDevice, ID3D11DeviceContext* pID3D11DeviceContext)
{
    IMGUI_CHECKVERSION(); // ПРОВЕРКА СОВМЕСТИМОСТИ КОДОВОЙ БАЗЫ И БИБЛИОТЕКИ IMGUI

    if (ImGui::CreateContext() == nullptr) // СОЗДАНИЕ КОНТЕКСТА IMGUI
    {
        return FALSE;
    }

    ImGuiIO& io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    if (ImGui_ImplDX11_Init(pD3DDevice, pID3D11DeviceContext) != true) // ИНИЦИАЛИЗАЦИЯ БЭКЕНДА IMGUI ДЛЯ DIRECTX 11
    {
        return FALSE;
    }

    if (ImGui_ImplWin32_Init(hWnd) != true) // ИНИЦИАЛИЗАЦИЯ БЭКЕНДА IMGUI ДЛЯ WINDOWS API
    {
        return FALSE;
    }

    startUpFlag = TRUE;

    return TRUE;
}

void UIManager::shutDown()
{
    startUpFlag = FALSE;

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext(); // УНИЧТОЖЕНИЕ КОНТЕКСТА IMGUI
}

void UIManager::run() const
{
    if (startUpFlag != TRUE)
    {
        return;
    }

    ImGui_ImplDX11_NewFrame(); // ПОДГОТОВКА БЭКЕНДА DIRECTX 11 К РЕНЕДИРНГУ НОВОГО КАДРА
    ImGui_ImplWin32_NewFrame(); // ПОДГОТОВКА БЭКЕНДА WINDOWS API К РЕНДЕРИНГУ НОВОГО КАДРА
    ImGui::NewFrame(); // ПОДГОТОВКА IMGUI К РЕНДЕРИНГУ НОВОГО КАДРА

    //

    UIManager::body(); // ОСНОВНОЕ ТЕЛО

    //

    ImGui::Render(); // СОЗДАНИЕ СПИСКА КОМАНД РЕНДЕРИНГА НОВОГО КАДРА
    
    constexpr ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    CONST FLOAT clearColorWithAlpha[4] = {clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w};

    d3DData.pD3DDeviceContext->OMSetRenderTargets(1U, &d3DData.pIDXGIRenderTargetView, nullptr); // УСТАНОВКА RENDER TARGET VIEW
    d3DData.pD3DDeviceContext->ClearRenderTargetView(d3DData.pIDXGIRenderTargetView, clearColorWithAlpha); // ОЧИСТКА RENDER TARGET VIEW

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData()); // РЕНДЕРИНГ НОВОГО КАДРА IMGUI С ПОМОЩЬЮ DIRECTX 11

    d3DData.pIDXGISwapChain->Present(1U, 0U); // ОТОБРАЖЕНИЕ БУФЕРА НА ЭКРАНЕ
}

LRESULT UIManager::handleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    //

    return ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
}
