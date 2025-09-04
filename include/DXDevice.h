#pragma once

#include <d3d11.h>

class DXDevice
{
public:
	DXDevice( HWND hWnd );
	DXDevice( const DXDevice& ) = delete;
	DXDevice& operator=( const DXDevice& ) = delete;
	~DXDevice();

	void SwapBuffers() { pChain->Present( 1u, 0u ); }

private:
	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pChain = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
};