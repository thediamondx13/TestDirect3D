#include <DXDevice.h>

DXDevice::DXDevice( HWND hWnd, LONG width, LONG height, bool windowed ) : camera( width, height )
{
	DXGI_SWAP_CHAIN_DESC scDesc = {};
	
	scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scDesc.BufferDesc.RefreshRate.Denominator = 0;
	scDesc.BufferDesc.RefreshRate.Numerator = 0;
	scDesc.BufferDesc.Height = height;
	scDesc.BufferDesc.Width = width;

	scDesc.SampleDesc.Quality = 0;
	scDesc.SampleDesc.Count = 1;

	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scDesc.OutputWindow = hWnd;
	scDesc.Windowed = windowed;
	scDesc.BufferCount = 1;
	scDesc.Flags = 0;

	D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
		0, nullptr, 0, D3D11_SDK_VERSION, &scDesc,
		&pChain, &pDevice, nullptr, &pContext
	);

	SetDepthStencilBuffer( width, height );
	SetRenderTarget();

	SetViewport( width, height );

	FillBuffer( 0.0f, 0.0f, 0.0f );
}

void DXDevice::SetRenderTarget()
{
	// get back buffer COM
	ComPtr<ID3D11Resource> pBackBuffer = nullptr;
	pChain->GetBuffer( 0u, __uuidof(ID3D11Resource), &pBackBuffer );

	// create render targer from back buffer
	pDevice->CreateRenderTargetView( pBackBuffer.Get(), nullptr, &pTarget );

	// bind render targer
	pContext->OMSetRenderTargets( 1u, pTarget.GetAddressOf(), pDSV.Get() );
}

void DXDevice::SetViewport( UINT width, UINT height )
{
	D3D11_VIEWPORT vp{};

	vp.Height = static_cast<float>(height);
	vp.Width = static_cast<float>(width);

	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;

	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	pContext->RSSetViewports( 1u, &vp );
}

void DXDevice::SetDepthStencilBuffer( UINT width, UINT height )
{
	D3D11_DEPTH_STENCIL_DESC dsDesc{};
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	dsDesc.DepthEnable = TRUE;

	// create depth-stencil buffer
	ComPtr<ID3D11DepthStencilState> pDSState;
	pDevice->CreateDepthStencilState( &dsDesc, &pDSState );

	// bind depth-stencil buffer
	pContext->OMSetDepthStencilState( pDSState.Get(), 1u );

	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.Format = DXGI_FORMAT_D32_FLOAT;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.SampleDesc.Quality = 0u;
	textureDesc.SampleDesc.Count = 1u;
	textureDesc.Height = height;
	textureDesc.ArraySize = 1u;
	textureDesc.MipLevels = 1u;
	textureDesc.Width = width;

	// create the texture
	ComPtr<ID3D11Texture2D> pTexture;
	pDevice->CreateTexture2D( &textureDesc, nullptr, &pTexture );

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.Texture2D.MipSlice = 0u;

	// create depth-stencil view
	pDevice->CreateDepthStencilView( pTexture.Get(), &dsvDesc, &pDSV );
}

void DXDevice::FillBuffer( float r, float g, float b ) const
{
	const float color[] = { r, g, b, 1.0f };
	pContext->ClearRenderTargetView( pTarget.Get(), color );
	pContext->ClearDepthStencilView( pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u );
}

void DXDevice::DrawIndexed( UINT count ) const
{
	pContext->DrawIndexed( count, 0u, 0u );
}

void DXDevice::SwapBuffers() const
{
	pChain->Present( 1u, 0u );
}