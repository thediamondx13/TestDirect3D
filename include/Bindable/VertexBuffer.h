#pragma once

#include "Bindable.h"

class VertexBuffer : public Bindable
{
public:
	template<typename V>
	VertexBuffer( const DXDevice &gfx, const std::vector<V> &vertices ) :
		_stride( static_cast<UINT>(sizeof( V )) )
	{
		const D3D11_BUFFER_DESC bd{
			static_cast<UINT>(sizeof( V ) * vertices.size()),
			D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER,
			0u, 0u, static_cast<UINT>(sizeof( V ))
		};

		const D3D11_SUBRESOURCE_DATA sd{
			vertices.data(), 0u, 0u
		};

		GetDevice( gfx ).CreateBuffer( &bd, &sd, &_pVertexBuffer );
	}

	void Bind( const DXDevice &gfx ) override;

protected:
	ComPtr<ID3D11Buffer> _pVertexBuffer;
	UINT _offset = 0u;
	UINT _stride;
};