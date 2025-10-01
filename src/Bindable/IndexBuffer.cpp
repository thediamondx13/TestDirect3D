#include <Bindable/IndexBuffer.h>

IndexBuffer::IndexBuffer( DXDevice &gfx, const std::vector<UINT> &indices ) :
	_count( static_cast<UINT>(indices.size()) )
{
	const D3D11_BUFFER_DESC ibd{
		static_cast<UINT>(_count * sizeof( UINT )),
		D3D11_USAGE_DEFAULT, D3D11_BIND_INDEX_BUFFER,
		0u, 0u, static_cast<UINT>(sizeof( UINT ))
	};

	const D3D11_SUBRESOURCE_DATA isd{
		indices.data(), 0u, 0u
	};

	GetDevice( gfx )->CreateBuffer( &ibd, &isd, &_pIndexBuffer );
}

void IndexBuffer::Bind( DXDevice &gfx )
{
	GetContext( gfx )->IASetIndexBuffer( _pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u );
}

UINT IndexBuffer::GetCount() const
{
	return _count;
}