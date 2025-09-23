#include <Bindable/VertexBuffer.h>

void VertexBuffer::Bind( DXDevice& gfx )
{
	GetContext( gfx )->IASetVertexBuffers(
		0u, 1u, pVertexBuffer.GetAddressOf(),
		&stride, &offset
	);
}