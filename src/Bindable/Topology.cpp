#include <Bindable/Topology.h>

Topology::Topology( const DXDevice &gfx, D3D11_PRIMITIVE_TOPOLOGY type ) : _type( type ) {}

void Topology::Bind( const DXDevice &gfx )
{
	GetContext( gfx ).IASetPrimitiveTopology( _type );
}