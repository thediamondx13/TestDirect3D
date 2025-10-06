#include <Drawable/Planet.h>

Planet::Planet( const DXDevice &gfx )
{
	_radius = 3.0f;
	_pos = DX::XMFLOAT4( 0.0f, 0.0f, 65.0f, 1.0f );
	_velocity = DX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f );
	_acceleration = DX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f );

	_rotation = DX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f );
	_rotationVel = DX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f );

	if ( StaticsNotInitialized() )
	{
		// sphere parameters
		constexpr int latDiv = 20;
		constexpr int longDiv = 40;
		const float latAng = DX::XM_PI / latDiv;
		const float longAng = DX::XM_2PI / longDiv;

		std::vector<DX::XMFLOAT3> vertices{};
		// base point that will be rotated
		const DX::XMVECTOR base = DX::XMVectorSet( _radius, 0.0f, 0.0f, 1.0f );

		// generate and store vertices of a sphere		
		for ( int i = 1; i < latDiv; i++ )
		{
			const DX::XMVECTOR latBase = DX::XMVector3TransformCoord(
				base, DX::XMMatrixRotationY( latAng * i )
			);
			for (int j = 0; j < longDiv; j++)
			{
				vertices.emplace_back();
				DX::XMStoreFloat3( &vertices.back(),
					DX::XMVector3TransformCoord( latBase, DX::XMMatrixRotationX( longAng * j ) )
				);
			}
		}

		// store sphere's poles
		const auto iNorthPole = vertices.size();
		vertices.emplace_back();
		DX::XMStoreFloat3( &vertices.back(), base );

		const auto iSouthPole = vertices.size();
		vertices.emplace_back();
		DX::XMStoreFloat3( &vertices.back(), DX::XMVectorNegate( base ) );
		
		// create vertex buffer
		AddBindStatic( std::make_unique<VertexBuffer>( gfx, vertices ) );

		
		std::vector<UINT> indices{};
		// generate and store indices of a sphere
		for ( UINT iLat = 0; iLat < latDiv - 2; iLat++ )
		{
			UINT iTmp = longDiv - 1;
			const UINT prevLat = iLat * longDiv;
			const UINT nextLat = prevLat + longDiv;

			for ( UINT iLong = 0; iLong < longDiv; iLong++ )
			{				
				indices.push_back( prevLat + iTmp );
				indices.push_back( nextLat + iTmp );
				indices.push_back( prevLat + iLong );

				indices.push_back( prevLat + iLong );
				indices.push_back( nextLat + iTmp );
				indices.push_back( nextLat + iLong );
				
				iTmp = iLong;
			}
		}
		
		UINT iTmp = longDiv - 1;
		for (UINT iLong = 0; iLong < longDiv; iLong++)
		{
			// north pole
			indices.push_back( iTmp );
			indices.push_back( iLong );
			indices.push_back( iNorthPole );

			// south pole
			indices.push_back( (latDiv - 2) * longDiv + iLong );
			indices.push_back( (latDiv - 2) * longDiv + iTmp );
			indices.push_back( iSouthPole );

			iTmp = iLong;
		}

		// create index buffer
		AddIndexBufferStatic( std::make_unique<IndexBuffer>( gfx, indices ) );

		// create pixel shader
		AddBindStatic( std::make_unique<PixelShader>( gfx, L"PlanetPS.cso" ) );

		// create vertex shader
		auto pVS = std::make_unique<VertexShader>( gfx, L"PlanetVS.cso" );
		auto *pVSBc = pVS->GetByteCode();
		AddBindStatic( std::move( pVS ) );

		// create input layout
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied{
			{"POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u}			
		};
		AddBindStatic( std::make_unique<InputLayout>( gfx, pVSBc, ied ) );

		AddBindStatic( std::make_unique<Topology>( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind( std::make_unique<TransformConstBuf>( gfx, *this ) );
}

void Planet::Update( float dt )
{
	/*DX::XMStoreFloat3( &_pos, DX::XMVectorAdd(
		DX::XMVectorScale( DX::XMLoadFloat3( &_velocity ), dt ),
		DX::XMLoadFloat3( &_pos ))
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

inline DX::XMMATRIX Planet::GetTransform() const
{
	return DX::XMMatrixRotationRollPitchYaw(_rotation.x, _rotation.y, _rotation.z) *
		DX::XMMatrixTranslation( _pos.x, _pos.y, _pos.z );
}