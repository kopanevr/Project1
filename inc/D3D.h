#include <Windows.h>

#include <d3d11.h>
#include <dxgi.h>

namespace ND3D {
struct D3DData
{
    IDXGISwapChain* pIDXGISwapChain = nullptr;
    ID3D11Device* pD3DDevice = nullptr;
    ID3D11DeviceContext* pD3DDeviceContext = nullptr;
    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
    ID3D11RenderTargetView* pIDXGIRenderTargetView = nullptr;
};

class D3D final
{
private:
    BOOL CreateRenderTargetView(); // СОЗДАНИЕ RENDER TARGET VIEW
    void CleanupRenderTargetView();

    IDXGIAdapter* GetAdapter(const UINT index) const; // ПОЛУЧЕНИЕ АДАПТЕРА
public:
    D3D() = default;
    ~D3D() = default;

    BOOL CreateDeviceD3D(HWND hWnd); // СОЗДАНИЕ УСТРОЙСТВА И ЦЕПОЧКИ БУФЕРОВ
    void CleanupDeviceD3D();
};}
