#include <Universe.h>

Universe::Universe( const DXDevice &gfx ) : _quad( gfx ), _useRTX( false )
{
	constexpr int size = 1;

	//_planets.resize( size );
	//for ( int i = 0; i < size; i++ )
	//{
	//	_planets[i] = std::make_unique<Planet>( gfx );

	//	/*for ( auto &pPlanet : _planets )
	//	{
	//		if ( pPlanet == nullptr ) break;
	//		pPlanet->Update( 7.0f );
	//	}*/
	//}
}

void Universe::Draw( const DXDevice &gfx )
{
	if ( _useRTX )
	{
		_quad.Draw( gfx );
	}
	else
	{
	
	}
}

void Universe::Update( float dt )
{

}