#include "DXDevice.h"

DXDevice::DXDevice( HWND hWnd )
{
	DXGI_SWAP_CHAIN_DESC pDesc = {};
	pDesc.BufferDesc.Width = 0;
	pDesc.BufferDesc.Height = 0;
	pDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	pDesc.BufferDesc.RefreshRate.Numerator = 0;
	pDesc.BufferDesc.RefreshRate.Denominator = 0;
	pDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	pDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	pDesc.SampleDesc.Count = 1;
	pDesc.SampleDesc.Quality = 0;
	pDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	pDesc.BufferCount = 1;
	pDesc.OutputWindow = hWnd;
	pDesc.Windowed = TRUE;
	pDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	pDesc.Flags = 0;

	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr, 0,
		nullptr, 0,
		D3D11_SDK_VERSION,
		&pDesc,
		&pChain,
		&pDevice,
		nullptr,
		&pContext
	);
}

DXDevice::~DXDevice()
{
	if ( pContext ) pContext->Release();

	if ( pChain ) pChain->Release();

	if ( pDevice ) pDevice->Release();
}