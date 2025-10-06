#include <Drawable/RTXQuad.h>

RTXQuad::RTXQuad( const DXDevice &gfx )
{
	// draw a full-screen quad
	const std::vector<UINT> indices{ 0, 1, 2, 3 };
	AddIndexBufferStatic( std::make_unique<IndexBuffer>( gfx, indices ) );

	const std::vector<DX::XMFLOAT2> vertices{
		DX::XMFLOAT2( -1.0f, -1.0f ), DX::XMFLOAT2( -1.0f, +1.0f ),
		DX::XMFLOAT2( +1.0f, -1.0f ), DX::XMFLOAT2( +1.0f, +1.0f )
	};
	AddBindStatic( std::make_unique<VertexBuffer>( gfx, vertices ) );

	// add pixel shader bind
	AddBindStatic( std::make_unique<PixelShader>( gfx, L"RTXQuadPS.cso" ) );

	// add vertex shader bind
	auto pVS = std::make_unique<VertexShader>( gfx, L"RTXQuadVS.cso" );
	auto *pVSBytes = pVS->GetByteCode();
	AddBindStatic( std::move( pVS ) );

	// add input layout bind
	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied{
		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u}
	};
	AddBindStatic( std::make_unique<InputLayout>( gfx, pVSBytes, ied ) );

	// add primitive topology bind
	AddBindStatic( std::make_unique<Topology>( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP ) );

	// add camera pixel shader constant buffer
	AddBindStatic( std::make_unique<CameraConstBuf>( gfx ) );
}

inline DX::XMMATRIX RTXQuad::GetTransform() const
{
	return DX::XMMatrixIdentity();
}