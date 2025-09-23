#include <Drawable/Sphere.h>

Sphere::Sphere( DXDevice &gfx )
{
	radius = 2.0f;
	pos = DX::XMFLOAT3( 0.0f, 0.0f, 7.0f );
	velocity = DX::XMFLOAT3( 0.0f, 0.0f, 0.0f );
	acceleration = DX::XMFLOAT3( 1.0f, 0.0f, 0.0f );

	rotation = DX::XMFLOAT3( 0.0f, 0.0f, 0.0f );
	rotationVel = DX::XMFLOAT3( 0.0f, 0.0f, 0.0f );

	if ( InitializeStatic() )
	{
		// generate and store vertices
		std::vector<DX::XMFLOAT3> vertices{};		
		constexpr int longDiv = 40;
		constexpr int latDiv = 20;
				
		DX::XMVECTOR base = DX::XMVectorSet( radius, 0.0f, 0.0f, 0.0f );
		const float longAng = DX::XM_2PI / longDiv;
		const float latAng = DX::XM_PI / latDiv;

		for ( int i = 1; i < latDiv; i++ )
		{
			DX::XMVECTOR latBase = DX::XMVector3Transform(
				base, DX::XMMatrixRotationY( latAng * i )
			);
			for (int j = 0; j < longDiv; j++)
			{
				vertices.emplace_back();				
				DX::XMStoreFloat3( &vertices.back(),
					DX::XMVector3Transform( latBase, DX::XMMatrixRotationX( longAng * j ) )
				);
			}
		}

		const auto iNorthPole = vertices.size();
		vertices.emplace_back();
		DX::XMStoreFloat3( &vertices.back(), base );

		const auto iSouthPole = vertices.size();
		vertices.emplace_back();
		DX::XMStoreFloat3( &vertices.back(), DX::XMVectorNegate(base) );		

		// bind generated vertices
		AddBind( std::make_unique<VertexBuffer>( gfx, vertices ) );
		
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
		
		UINT  iTmp = longDiv - 1;
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

		// add indices bind
		AddIndexBuf( std::make_unique<IndexBuffer>( gfx, indices ) );

		// add pixel shader bind
		AddBind( std::make_unique<PixelShader>( gfx, L"SpherePS.cso" ) );

		// add vertex shader bind
		auto pVS = std::make_unique<VertexShader>( gfx, L"SphereVS.cso" );
		auto pVSBc = pVS->GetByteCode();
		AddBind( std::move( pVS ) );

		// add input layout bind
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied{
			{"POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u}			
		};
		AddBind( std::make_unique<InputLayout>( gfx, pVSBc, ied ) );

		// add primitive topology bind
		AddBind( std::make_unique<Topology>( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
	}
	else
	{
		// get binds from static vector
		// if it's initialized
		SetIndexFromStatic();
	}

	// add transformation matrix bind
	AddBind( std::make_unique<TransformConstBuf>( gfx, *this ) );
}

void Sphere::Update( float dt )
{
	DX::XMStoreFloat3( &pos, DX::XMVectorAdd(
		DX::XMVectorScale( DX::XMLoadFloat3( &velocity ), dt ),
		DX::XMLoadFloat3( &pos ))
	);

	DX::XMStoreFloat3( &velocity, DX::XMVectorAdd(
		DX::XMVectorScale( DX::XMLoadFloat3( &acceleration ), dt ),
		DX::XMLoadFloat3( &velocity ))
	);

	if ( pos.x > 1 ) acceleration.x = -1.0f;
	if ( pos.x < -1 ) acceleration.x = 1.0f;

	if ( abs( velocity.x ) > 3.0f ) velocity.x *= 0.9f;

	/*DX::XMStoreFloat3( &rotation, DX::XMVectorAdd(
		DX::XMVectorScale( DX::XMLoadFloat3( &rotationVel ), dt ),
		DX::XMLoadFloat3( &rotation ))
	);*/
	
}

inline DX::XMMATRIX Sphere::GetTransform() const
{
	return DX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) *
		DX::XMMatrixTranslation( pos.x, pos.y, pos.z );
}
