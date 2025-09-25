#include <Bindable/Topology.h>

Topology::Topology( DXDevice &gfx, D3D11_PRIMITIVE_TOPOLOGY type ) : _type( type ) {}

void Topology::Bind( DXDevice &gfx )
{
	GetContext( gfx )->IASetPrimitiveTopology( _type );
}