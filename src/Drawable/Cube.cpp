#include <Drawable/Cube.h>

Cube::Cube( DXDevice& gfx )
{
	// positions
	r = 3;
	
	roll = 0;
	pitch = 0;
	yaw = 0;
	
	theta = 0;
	chi = 0;
	phi = 0;

	
	// velocities
	dr = 0;
		
	droll = -1.34f / 5;
	dpitch = 0.9f / 5;
	dyaw = 1.1f / 5;

	dphi = 0;
	dtheta = 0;
	dchi = 0.1f;

	struct Vertex
	{
		float x, y, z;
		UINT r, g, b;
	};

	if ( InitializeStatic() )
	{
		// add vertices bind
		const std::vector<Vertex> vertices{
			{-1, -1, -1,   0x3D, 0x00, 0xFF},
			{+1, -1, -1,   0xFF, 0xD7, 0x00},
			{-1, +1, -1,   0xBF, 0x21, 0x52},
			{+1, +1, -1,   0x9D, 0x00, 0xFF},
			{-1, -1, +1,   0x9D, 0x00, 0xFF},
			{+1, -1, +1,   0xBF, 0x21, 0x52},
			{-1, +1, +1,   0xFF, 0xD7, 0x00},
			{+1, +1, +1,   0x3D, 0x00, 0xFF}
		};
		AddBind( std::make_unique<VertexBuffer>( gfx, vertices ) );
		
		// add indices bind
		const std::vector<UINT> indices{
			0, 2, 1,  2, 3, 1,
			1, 3, 5,  3, 7, 5,
			2, 6, 3,  3, 6, 7,
			4, 5, 7,  4, 7, 6,
			0, 4, 2,  2, 4, 6,
			0, 1, 4,  1, 5, 4
		};
		AddIndexBuf( std::make_unique<IndexBuffer>( gfx, indices ) );

		// add pixel shader bind
		AddBind( std::make_unique<PixelShader>( gfx, L"CubePS.cso" ) );
		
		// add vertex shader bind
		auto pVS = std::make_unique<VertexShader>( gfx, L"CubeVS.cso" );
		auto pVSBc = pVS->GetByteCode();
		AddBind( std::move( pVS ) );		

		// add input layout bind
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied{
			{"POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32_UINT, 0u, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0u}
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

void Cube::Update( float dt )
{
	// dist from center
	r += (dr * dt);

	// rotation around world center
	theta += (dtheta * dt);
	phi += (dphi * dt);
	chi += (dchi * dt);

	// rotation around self center
	roll += (droll * dt);
	pitch += (dpitch * dt);
	yaw += (dyaw * dt);
}

inline DX::XMMATRIX Cube::GetTransform() const
{
	return DX::XMMatrixRotationRollPitchYaw( pitch, yaw, roll ) *
		DX::XMMatrixTranslation( r, 0.0f, 0.0f ) *
		DX::XMMatrixRotationRollPitchYaw( theta, phi, chi );
}