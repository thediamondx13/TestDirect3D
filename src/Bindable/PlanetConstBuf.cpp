#include <Bindable/PlanetConstBuf.h>

#include <Drawable/Planet.h>

PlanetConstBuf::PlanetConstBuf( const DXDevice &gfx, const Planet &planet ) :
	_buf( PixelConstantBuffer<PlanetBuf>( gfx ) ), _planet(planet)
{}

void PlanetConstBuf::Bind( const DXDevice &gfx )
{
	PlanetBuf pb{};

	pb.color = _planet.GetColor();

	_buf.Update( gfx, pb );

	_buf.Bind( gfx );
}