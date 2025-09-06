#pragma once

// standard includes
#include <d3d11.h>
#include <stdexcept>
#include <wrl/client.h>
using namespace Microsoft::WRL;

class DXDevice
{
public:
	DXDevice( HWND hWnd );
	DXDevice( const DXDevice& ) = delete;
	DXDevice& operator=( const DXDevice& ) = delete;
	~DXDevice() = default;

	void ClearBuffer( float r, float g, float b );
	void DrawPrimitive( POINTS cursor );
	void SwapBuffers() { pChain->Present( 1u, 0u ); }
	
private:
	ComPtr<ID3D11Device> pDevice = nullptr;
	ComPtr<IDXGISwapChain> pChain = nullptr;
	ComPtr<ID3D11DeviceContext> pContext = nullptr;
	ComPtr<ID3D11RenderTargetView> pTarget = nullptr;
};