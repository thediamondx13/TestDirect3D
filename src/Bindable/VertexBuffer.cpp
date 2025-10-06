#include <Bindable/VertexBuffer.h>

void VertexBuffer::Bind( const DXDevice &gfx )
{
	GetContext( gfx ).IASetVertexBuffers(
		0u, 1u, _pVertexBuffer.GetAddressOf(),
		&_stride, &_offset
	);
}