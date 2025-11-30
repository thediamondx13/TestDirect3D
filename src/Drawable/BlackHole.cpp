#include <Drawable/BlackHole.h>

BlackHole::BlackHole( const DXDevice &gfx ) : Planet( gfx )
{
	SetColor( DX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f ) );
}