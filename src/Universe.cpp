#include <Universe.h>

Universe::Universe( const DXDevice &gfx ) : _useRTX( false ),
	_quad( gfx ), _blackHoleBuffer( gfx ), _planetBuffer( gfx )
{
	constexpr int size = 1;

	_blackHoles.resize( size );
	_blackHoles[0] = std::make_unique<BlackHole>( gfx );
	_blackHoles[0]->SetPosition( DX::XMVectorSet( 0.0f, 0.0f, 20.0f, 1.0f ) );
	_blackHoles[0]->SetVelocity( DX::XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f ) );
	// leave acceleration 0
	_blackHoles[0]->SetRadius( 1 );
	_blackHoles[0]->SetMass( 10 );
	
	_planets.resize( size + 1 );
	_planets[0] = std::make_unique<Planet>( gfx );
	_planets[0]->SetPosition( DX::XMVectorSet( 10.0f, 0.0f, 40.0f, 1.0f ) );
	_planets[0]->SetVelocity( DX::XMVectorSet( 0.0f, 0.04f, 0.0f, 0.0f ) );
	// leave acceleration 0
	_planets[0]->SetRadius( 3 );
	_planets[0]->SetMass( 1e12 );

	
	_planets[1] = std::make_unique<Planet>( gfx );
	_planets[1]->SetPosition( DX::XMVectorSet( -10.0f, 0.0f, 40.0f, 1.0f ) );
	_planets[1]->SetVelocity( DX::XMVectorSet( 0.0f, -0.04f, 0.0f, 0.0f ) );
	// leave acceleration 0
	_planets[1]->SetRadius( 3 );
	_planets[1]->SetMass( 1e12 );
	_planets[1]->SetColor( DX::XMFLOAT4( 1, 0.5, 0.5, 1 ) );
}

void Universe::Draw( const DXDevice &gfx )
{

	if ( _useRTX )
	{
		BindBlackHoleBuffer( gfx );
		BindPlanetBuffer( gfx );

		_quad.Draw( gfx );
	}
	else
	{
		for ( auto &pPlanet : _blackHoles )
		{
			if ( pPlanet == nullptr ) break;
			pPlanet->Draw( gfx );
		}

		for ( auto &pPlanet : _planets )
		{
			if ( pPlanet == nullptr ) break;
			pPlanet->Draw( gfx );
		}
	}
}


/* usage of dt breaks physics in case the application freezes for a long time */
void Universe::Update( float dt )
{
	//constexpr float G = 6.67430e-11f;
	constexpr float G = 6.67430e-12f;

	for ( auto &pBlackHole : _blackHoles )
	{
		if ( pBlackHole == nullptr ) break;
		pBlackHole->Update( dt );
	}

	for ( auto &pPlanet : _planets )
	{
		if ( pPlanet == nullptr ) break;

		DX::XMVECTOR pos = pPlanet->GetPosition();

		DX::XMVECTOR massOverDistSquared{};
		for ( auto &pOtherPlanet : _planets )
		{
			if ( pOtherPlanet == nullptr ) break;
			if ( pPlanet == pOtherPlanet ) continue;

			const DX::XMVECTOR vecToOther = DX::XMVectorSubtract( pOtherPlanet->GetPosition(), pos );
			const float lenSqr = DX::XMVectorGetX( DX::XMVector3Dot( vecToOther, vecToOther ) );

			massOverDistSquared = DX::XMVectorAdd( massOverDistSquared,
				DX::XMVectorScale( DX::XMVector3Normalize( vecToOther ),
					pOtherPlanet->GetMass() / lenSqr
				)
			);
		}

		pPlanet->SetAcceleration( DX::XMVectorScale( massOverDistSquared, G ) );
	}

	for ( auto &pPlanet : _planets )
	{
		if ( pPlanet == nullptr ) break;
		pPlanet->Update( dt );
	}
}

void Universe::BindBlackHoleBuffer( const DXDevice &gfx )
{
	BlackHoleBuffer buf{};

	buf.count = DX::XMUINT4( _blackHoles.size(), 0, 0, 0 );

	for ( uint32_t i = 0; i < buf.count.x; i++ )
	{
		if ( _blackHoles[i] == nullptr ) break;
		buf.info[i] = _blackHoles[i]->GetInfo();
	}

	_blackHoleBuffer.Update( gfx, buf );

	_blackHoleBuffer.Bind( gfx, 1u );
}

void Universe::BindPlanetBuffer( const DXDevice &gfx )
{
	PlanetBuffer buf{};

	buf.count = DX::XMUINT4( _planets.size(), 0, 0, 0 );

	for ( uint32_t i = 0; i < buf.count.x; i++ )
	{
		if ( _planets[i] == nullptr ) break;
		buf.info[i] = _planets[i]->GetInfo();
	}

	_planetBuffer.Update( gfx, buf );

	_planetBuffer.Bind( gfx, 2u );
}