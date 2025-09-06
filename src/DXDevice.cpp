#include <DXDevice.h>

// standard includes
#include <d3dcompiler.h>
#include <vector>

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

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
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
	if ( FAILED(hr) ) throw std::runtime_error( "Failed to create D3D11 device." );

	ComPtr<ID3D11Resource> pBackBuffer = nullptr;
	hr = pChain->GetBuffer( 0u, __uuidof(ID3D11Resource), &pBackBuffer );
	if( FAILED( hr ) ) throw std::runtime_error( "Failed to get back buffer." );

	hr = pDevice->CreateRenderTargetView( pBackBuffer.Get(), nullptr, &pTarget );
	if( FAILED( hr ) ) throw std::runtime_error( "Failed to create render target view." );
	
	pContext->OMSetRenderTargets( 1u, pTarget.GetAddressOf(), nullptr );
}

void DXDevice::ClearBuffer( float r, float g, float b )
{
	const float color[] = { r, g, b, 1.0f };
	pContext->ClearRenderTargetView( pTarget.Get(), color );
}

void DXDevice::DrawPrimitive( POINTS cursor )
{
	//
	// TODO
	// refactor this whole method
	//

	HRESULT hr;

	float x0 = static_cast<float>(cursor.x) / 400.0f - 1.0f;
	float y0 = -static_cast<float>(cursor.y) / 300.0f + 1.0f;

	struct Vertex
	{
		float x, y;
		float x0, y0; // cursor position
	};

	const UINT vertLines = 25;
	const UINT horizLines = 25;
	const UINT pointsPerLine = 1000;
	
	std::vector<Vertex> vertices;
	vertices.reserve( pointsPerLine * (horizLines + vertLines) );
	
	float deltaX = 4.0f / (pointsPerLine - 1);
	float deltaY = 4.0f / (horizLines - 1);
	for( UINT i = 0; i < horizLines; i++ )
	{
		float y = -2.0f + (deltaY * i);
		for( UINT j = 0; j < pointsPerLine; j++ )
		{
			float x = -2.0f + (deltaX * j);
			vertices.push_back( { x, y, x0, y0 } );
		}
	}

	deltaY = deltaX;
	deltaX = 4.0f / (vertLines - 1);
	for( UINT i = 0; i < vertLines; i++ )
	{
		float x = -2.0f + (deltaX * i);
		for( UINT j = 0; j < pointsPerLine; j++ )
		{
			float y = -2.0f + (deltaY * j);
			vertices.push_back( { x, y, x0, y0 } );
		}
	}

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices.data();
	
	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = static_cast<UINT>(vertices.size() * sizeof( Vertex ));
	bd.StructureByteStride = sizeof( Vertex );
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	
	ComPtr<ID3D11Buffer> pVertexBuffer = nullptr;
	hr = pDevice->CreateBuffer( &bd, &sd, &pVertexBuffer );
	if( FAILED( hr ) ) throw std::runtime_error( "Failed to create vertex buffer." );

	const UINT offset = 0u;
	const UINT stride = sizeof( Vertex );
	pContext->IASetVertexBuffers( 0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
	

	// blob for shaders
	ComPtr<ID3DBlob> pBlob = nullptr;

	// load pixel shader
	hr = D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
	if( FAILED( hr ) ) throw std::runtime_error( "Failed to load pixel shader." );
	
	ComPtr<ID3D11PixelShader> pPixelShader = nullptr;
	hr = pDevice->CreatePixelShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader );
	if( FAILED( hr ) ) throw std::runtime_error( "Failed to create pixel shader." );

	pContext->PSSetShader( pPixelShader.Get(), nullptr, 0u );


	// load vertex shader
	hr = D3DReadFileToBlob( L"VertexShader.cso", &pBlob );
	if( FAILED( hr ) ) throw std::runtime_error( "Failed to load vertex shader." );

	ComPtr<ID3D11VertexShader> pVertexShader = nullptr;
	hr = pDevice->CreateVertexShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader );
	if( FAILED( hr ) ) throw std::runtime_error( "Failed to create vertex shader." );

	pContext->VSSetShader( pVertexShader.Get(), nullptr, 0u );



	ComPtr<ID3D11InputLayout> pInputLayout = nullptr;
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POS", 0, DXGI_FORMAT_R32G32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "POS", 1, DXGI_FORMAT_R32G32_FLOAT, 0u, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	hr = pDevice->CreateInputLayout(
		layout, static_cast<UINT>(std::size(layout)),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	);
	if( FAILED( hr ) ) throw std::runtime_error( "Failed to create input layout." );

	pContext->IASetInputLayout( pInputLayout.Get() );

	pContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP );

	D3D11_VIEWPORT vp = {};
	vp.Width = 800.0f;
	vp.Height = 600.0f;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pContext->RSSetViewports( 1u, &vp );

	UINT lineCount = horizLines + vertLines;
	for( UINT i = 0; i < lineCount; i++ )
		pContext->Draw( pointsPerLine, pointsPerLine * i );
}