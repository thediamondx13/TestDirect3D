#pragma once

// d3d11 libraries
#include <d3d11.h>
#include <d3dcompiler.h>

#include <DirectXMath.h>
namespace DX = DirectX;

// standard includes
#include <vector>
#include <stdexcept>
#include <wrl/client.h>
using namespace Microsoft::WRL;

#include <Camera.h>

class DXDevice
{
	friend class Bindable;

public:
	// constructor, operators, destructor
	DXDevice( HWND hWnd, LONG width, LONG height, bool windowed );

	// drawing routine
	void FillBuffer( float r, float g, float b ) const;
	void DrawIndexed( UINT count ) const;
	void SwapBuffers() const;

	Camera camera;

protected:
	void SetRenderTarget();
	void SetViewport( UINT width, UINT height );
	void SetDepthStencilBuffer( UINT width, UINT height );

	// DirectX device
	ComPtr<ID3D11Device> pDevice = nullptr;

	// DirectX context
	ComPtr<ID3D11DeviceContext> pContext = nullptr;

	// DirectX swapchain
	ComPtr<IDXGISwapChain> pChain = nullptr;

	// DirectX render target view
	ComPtr<ID3D11RenderTargetView> pTarget = nullptr;

	// DirectX depth-stencil view
	ComPtr<ID3D11DepthStencilView> pDSV = nullptr;
};