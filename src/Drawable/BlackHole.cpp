#include <Drawable/BlackHole.h>

BlackHole::BlackHole( DXDevice &gfx, Camera &cam )
{
	_radius = 30.0f;
	_pos = DX::XMFLOAT4( 0.0f, 0.0f, 40.0f, 1.0f );
	_velocity = DX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f );
	_acceleration = DX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f );

	_rotation = DX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f );
	_rotationVel = DX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f );

	if ( InitializeStatic() )
	{
		// draw a full-screen quad
		const std::vector<UINT> indices{ 0, 1, 2, 3 };
		const std::vector<DX::XMFLOAT4> vertices{
			DX::XMFLOAT4( -1.0f, -1.0f, 0.99f, 1.0f ),
			DX::XMFLOAT4( -1.0f, +1.0f, 0.99f, 1.0f ),
			DX::XMFLOAT4( +1.0f, -1.0f, 0.99f, 1.0f ),
			DX::XMFLOAT4( +1.0f, +1.0f, 0.99f, 1.0f )
		};
		AddBind( std::make_unique<VertexBuffer>( gfx, vertices ) );
		AddIndexBuf( std::make_unique<IndexBuffer>( gfx, indices ) );

		// add pixel shader bind
		AddBind( std::make_unique<PixelShader>( gfx, L"BlackHolePS.cso" ) );

		// add vertex shader bind
		auto pVS = std::make_unique<VertexShader>( gfx, L"BlackHoleVS.cso" );
		auto pVSBc = pVS->GetByteCode();
		AddBind( std::move( pVS ) );

		// add input layout bind
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied{
			{"POS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u}
		};
		AddBind( std::make_unique<InputLayout>( gfx, pVSBc, ied ) );

		// add primitive topology bind
		AddBind( std::make_unique<Topology>( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP ) );

		// add camera pixel shader constant buffer
		AddBind( std::make_unique<CameraConstBuf>( gfx, cam ) );
	}
	else
	{
		SetIndexFromStatic();
	}

	struct BlackHole
	{
		DX::XMFLOAT4 pos;
		float rad;
	};
}

void BlackHole::Update( float dt )
{
	/*DX::XMStoreFloat4( &_pos, DX::XMVectorAdd(
		DX::XMVectorScale( DX::XMLoadFloat3( &_velocity ), dt ),
		DX::XMLoadFloat3( &_pos ) )
	);*/

	/*DX::XMStoreFloat3( &velocity, DX::XMVectorAdd(
		DX::XMVectorScale( DX::XMLoadFloat3( &acceleration ), dt ),
		DX::XMLoadFloat3( &velocity ))
	);*/

	/*if ( pos.x > 1 ) acceleration.x = -1.0f;
	if ( pos.x < -1 ) acceleration.x = 1.0f;*/

	//if ( abs( velocity.x ) > 3.0f ) velocity.x *= 0.9f;

	/*DX::XMStoreFloat3( &rotation, DX::XMVectorAdd(
		DX::XMVectorScale( DX::XMLoadFloat3( &rotationVel ), dt ),
		DX::XMLoadFloat3( &rotation ))
	);*/
}

inline DX::XMMATRIX BlackHole::GetTransform() const
{
	return DX::XMMatrixRotationRollPitchYaw( _rotation.x, _rotation.y, _rotation.z ) *
		DX::XMMatrixTranslation( _pos.x, _pos.y, _pos.z );
}