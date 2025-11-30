#include <Drawable/Planet.h>

Planet::Planet( const DXDevice &gfx )
{
	constexpr int latDiv = 20;
	constexpr int longDiv = 40;

	// create vertex buffer
	AddBind( std::make_unique<VertexBuffer>( gfx, GenerateVertices( latDiv, longDiv ) ) );

	if ( StaticsNotInitialized() )
	{
		// create index buffer
		AddIndexBufferStatic( std::make_unique<IndexBuffer>( gfx, GenerateIndices( latDiv, longDiv ) ) );

		// create pixel shader
		AddBindStatic( std::make_unique<PixelShader>( gfx, L"PlanetPS.cso" ) );

		// create vertex shader
		auto pVS = std::make_unique<VertexShader>( gfx, L"PlanetVS.cso" );
		auto *pVSBc = pVS->GetByteCode();
		AddBindStatic( std::move( pVS ) );

		// create input layout
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u},
			{"COLOR", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0u, 16u, D3D11_INPUT_PER_VERTEX_DATA, 0u}
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

void Planet::SetAcceleration( const DX::XMVECTOR &a )
{
	DX::XMStoreFloat4( &_acceleration, a );
}

void Planet::SetVelocity( const DX::XMVECTOR &s )
{
	DX::XMStoreFloat4( &_velocity, s );
}

DX::XMVECTOR Planet::GetVelocity() const
{
	return DX::XMLoadFloat4( &_velocity );
}

void Planet::SetPosition( const DX::XMVECTOR &p )
{
	DX::XMStoreFloat4( &_pos, p );
}

DX::XMVECTOR Planet::GetPosition() const
{
	return DX::XMLoadFloat4( &_pos );
}

void Planet::SetRadius( const float newRadius )
{
	_radius = newRadius;
}

float Planet::GetRadius() const
{
	return _radius;
}

DX::XMFLOAT4 Planet::GetInfo() const
{
	return DX::XMFLOAT4( _pos.x, _pos.y, _pos.z, _radius );
}

void Planet::SetColor( const DX::XMFLOAT4 color )
{
	_color = color;
}

void Planet::Update( float dt )
{
	const DX::XMVECTOR newVelocity = DX::XMVectorAdd( DX::XMLoadFloat4( &_velocity ),
		DX::XMVectorScale( DX::XMLoadFloat4( &_acceleration ), dt )
	);

	DX::XMStoreFloat4( &_pos, DX::XMVectorAdd( DX::XMLoadFloat4( &_pos ), newVelocity ) );

	DX::XMStoreFloat4( &_velocity, newVelocity );

	/*DX::XMStoreFloat3( &rotation, DX::XMVectorAdd(
		DX::XMVectorScale( DX::XMLoadFloat3( &rotationVel ), dt ),
		DX::XMLoadFloat3( &rotation ))
	);*/
}

std::vector<Planet::Vertex> Planet::GenerateVertices( const int latDiv, const int longDiv ) const
{
	const float latAng = DX::XM_PI / latDiv;
	const float longAng = DX::XM_2PI / longDiv;

	std::vector<Planet::Vertex> vertices{};
	// base point that will be rotated
	const DX::XMVECTOR base = DX::XMVectorSet( 1.0f, 0.0f, 0.0f, 1.0f );

	// generate and store vertices of a sphere		
	for ( int i = 1; i < latDiv; i++ )
	{
		const DX::XMVECTOR latBase = DX::XMVector3TransformCoord(
			base, DX::XMMatrixRotationY( latAng * i )
		);
		for ( int j = 0; j < longDiv; j++ )
		{
			vertices.emplace_back();

			DX::XMStoreFloat4( &vertices.back().position,
				DX::XMVector3TransformCoord( latBase, DX::XMMatrixRotationX( longAng * j ) )
			);

			vertices.back().color = _color;
		}
	}

	// store sphere's poles
	vertices.emplace_back();
	DX::XMStoreFloat4( &vertices.back().position, base );
	vertices.back().color = _color;

	vertices.emplace_back();
	DX::XMStoreFloat4( &vertices.back().position, DX::XMVectorNegate( base ) );
	vertices.back().color = _color;

	return vertices;
}

std::vector<UINT> Planet::GenerateIndices( const UINT latDiv, const UINT longDiv ) const
{
	const UINT iNorthPole = (latDiv - 1) * longDiv;
	const UINT iSouthPole = iNorthPole + 1;

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
	for ( UINT iLong = 0; iLong < longDiv; iLong++ )
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

	return indices;
}

void Planet::SetMass( const float newMass )
{
	_mass = newMass;
}

float Planet::GetMass() const
{
	return _mass;
}

inline DX::XMMATRIX Planet::GetTransform() const
{
	return DX::XMMatrixScaling( _radius, _radius, _radius ) *
		DX::XMMatrixRotationRollPitchYaw(_rotation.x, _rotation.y, _rotation.z) *
		DX::XMMatrixTranslation( _pos.x, _pos.y, _pos.z );
}