#pragma once

#include "Bindable.h"

class VertexBuffer : public Bindable
{
public:
	template<typename V>
	VertexBuffer( DXDevice& gfx, const std::vector<V>& vertices ) :
		stride( static_cast<UINT>(sizeof( V )) )
	{
		const D3D11_BUFFER_DESC bd{
			static_cast<UINT>(sizeof( V ) * vertices.size()),
			D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER,
			0u, 0u, static_cast<UINT>(sizeof( V ))
		};

		const D3D11_SUBRESOURCE_DATA sd{
			vertices.data(), 0u, 0u
		};

		GetDevice( gfx )->CreateBuffer( &bd, &sd, &pVertexBuffer );
	}

	void Bind( DXDevice& gfx ) override;

protected:
	UINT stride;
	UINT offset = 0u;
	ComPtr<ID3D11Buffer> pVertexBuffer;
};