#include <DXDevice.h>

DXDevice::DXDevice( HWND hWnd, LONG width, LONG height ) : camera( width, height )
{
	DXGI_SWAP_CHAIN_DESC pDesc = {};
	
	pDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	pDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	pDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	pDesc.BufferDesc.RefreshRate.Denominator = 0;
	pDesc.BufferDesc.RefreshRate.Numerator = 0;
	pDesc.BufferDesc.Height = height;
	pDesc.BufferDesc.Width = width;
		
	pDesc.SampleDesc.Quality = 0;
	pDesc.SampleDesc.Count = 1;
	
	pDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	pDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	pDesc.OutputWindow = hWnd;
	pDesc.Windowed = TRUE;
	pDesc.BufferCount = 1;	
	pDesc.Flags = 0;

	lastResult = D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
		0, nullptr, 0, D3D11_SDK_VERSION, &pDesc,
		&pChain, &pDevice, nullptr, &pContext
	);

	if ( FAILED( lastResult ) ) throw std::runtime_error( "Failed to initialize DirectX." );	

	SetDepthStencilBuffer( width, height );
	SetRenderTarget();

	SetViewport( width, height );

	FillBuffer( 0.0f, 0.0f, 0.0f );
}

void DXDevice::SetRenderTarget()
{
	// get back buffer COM
	ComPtr<ID3D11Resource> pBackBuffer = nullptr;
	lastResult = pChain->GetBuffer( 0u, __uuidof(ID3D11Resource), &pBackBuffer );
	if ( FAILED( lastResult ) ) throw std::runtime_error( "Failed to get back buffer." );

	// create render targer from back buffer
	lastResult = pDevice->CreateRenderTargetView( pBackBuffer.Get(), nullptr, &pTarget );
	if ( FAILED( lastResult ) ) throw std::runtime_error( "Failed to create render target view." );

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
	lastResult = pDevice->CreateDepthStencilState( &dsDesc, &pDSState );
	if ( FAILED( lastResult ) ) throw "Failed to create depth-stencil buffer.";

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
	lastResult = pDevice->CreateTexture2D( &textureDesc, nullptr, &pTexture );
	if ( FAILED( lastResult ) ) throw "Failed to create depth-stencil texture.";

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;	
	dsvDesc.Texture2D.MipSlice = 0u;

	// create depth-stencil view
	lastResult = pDevice->CreateDepthStencilView( pTexture.Get(), &dsvDesc, &pDSV );
	if ( FAILED( lastResult ) ) throw "Failed to create depth-stencil view.";
}

void DXDevice::FillBuffer( float r, float g, float b )
{
	const float color[] = { r, g, b, 1.0f };
	pContext->ClearRenderTargetView( pTarget.Get(), color );
	pContext->ClearDepthStencilView( pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u );
}

void DXDevice::DrawIndexed( UINT count )
{
	pContext->DrawIndexed( count, 0u, 0u );
}

void DXDevice::SwapBuffers()
{
	pChain->Present( 1u, 0u );
}