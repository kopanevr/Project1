#pragma once

#include <Windows.h>
#include <d3d11.h>

class UIManager final
{
private:
    void body() const;
private:
    BOOL startUpFlag = FALSE;
public:
    UIManager() = default;
    ~UIManager() = default;

#if __cppcpp > 201703L
    [[nodiscard]]
#endif
    BOOL init(HWND hWnd, ID3D11Device* pD3DDevice, ID3D11DeviceContext* pID3D11DeviceContext);

    void shutDown();

    void run() const;

    LRESULT handleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
