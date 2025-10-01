#include <Drawable/Planet.h>

Planet::Planet( DXDevice &gfx )
{
	_radius = 3.0f;
	_pos = DX::XMFLOAT4( 0.0f, 0.0f, 65.0f, 1.0f );
	_velocity = DX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f );
	_acceleration = DX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f );

	_rotation = DX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f );
	_rotationVel = DX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f );

	if ( InitializeStatic() )
	{
		// sphere parameters
		constexpr int latDiv = 20;
		constexpr int longDiv = 40;
		const float latAng = DX::XM_PI / latDiv;
		const float longAng = DX::XM_2PI / longDiv;

		
		// generate and store vertices of a sphere
		std::vector<DX::XMFLOAT4> vertices{};
		const DX::XMVECTOR base = DX::XMVectorSet( _radius, 0.0f, 0.0f, 1.0f );
		for ( int i = 1; i < latDiv; i++ )
		{
			const DX::XMVECTOR latBase = DX::XMVector3TransformCoord(
				base, DX::XMMatrixRotationY( latAng * i )
			);
			for (int j = 0; j < longDiv; j++)
			{
				vertices.emplace_back();
				DX::XMStoreFloat4( &vertices.back(),
					DX::XMVector3TransformCoord( latBase, DX::XMMatrixRotationX( longAng * j ) )
				);
			}
		}

		const auto iNorthPole = vertices.size();
		vertices.emplace_back();
		DX::XMStoreFloat4( &vertices.back(), base );

		const auto iSouthPole = vertices.size();
		vertices.emplace_back();
		DX::XMStoreFloat4( &vertices.back(), 
			DX::XMVectorSetW( DX::XMVectorNegate( base ), 1.0f )
		);

		AddBind( std::make_unique<VertexBuffer>( gfx, vertices ) );


		// generate and store indices of a sphere
		std::vector<UINT> indices{};
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
		AddIndexBuf( std::make_unique<IndexBuffer>( gfx, indices ) );


		AddBind( std::make_unique<PixelShader>( gfx, L"PlanetPS.cso" ) );


		auto pVS = std::make_unique<VertexShader>( gfx, L"PlanetVS.cso" );
		const auto pVSBc = pVS->GetByteCode();
		AddBind( std::move( pVS ) );


		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied{
			{"POS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u}			
		};
		AddBind( std::make_unique<InputLayout>( gfx, pVSBc, ied ) );


		AddBind( std::make_unique<Topology>( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
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