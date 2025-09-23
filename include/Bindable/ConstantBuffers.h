#pragma once

#include "Bindable.h"

template<typename C>
class ConstantBuffer : public Bindable
{
public:
	ConstantBuffer( DXDevice& gfx, const C& consts )
	{
		const D3D11_BUFFER_DESC cbd{
			sizeof( consts ),
			D3D11_USAGE_DYNAMIC,
			D3D11_BIND_CONSTANT_BUFFER,
			D3D11_CPU_ACCESS_WRITE, 0u, 0u
		};

		const D3D11_SUBRESOURCE_DATA csd{
			&consts, 0u, 0u
		};

		GetDevice( gfx )->CreateBuffer( &cbd, &csd, &pConstBuf );
	}

	ConstantBuffer( DXDevice& gfx )
	{
		const D3D11_BUFFER_DESC cbd{
			sizeof( C ),
			D3D11_USAGE_DYNAMIC,
			D3D11_BIND_CONSTANT_BUFFER,
			D3D11_CPU_ACCESS_WRITE, 0u, 0u
		};

		GetDevice( gfx )->CreateBuffer( &cbd, nullptr, &pConstBuf );
	}

	void Update( DXDevice& gfx, const C& consts )
	{
		D3D11_MAPPED_SUBRESOURCE msr;

		// map the buffer
		GetContext( gfx )->Map( pConstBuf.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr );

		// update the data
		memcpy( msr.pData, &consts, sizeof( consts ) );

		// unmap the buffer
		GetContext( gfx )->Unmap( pConstBuf.Get(), 0u );
	}
protected:
	ComPtr<ID3D11Buffer> pConstBuf;
};

template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C>
{	
public:
	using ConstantBuffer<C>::ConstantBuffer;
	using ConstantBuffer<C>::pConstBuf;
	using Bindable::GetContext;

	void Bind( DXDevice& gfx )
	{
		GetContext( gfx )->VSSetConstantBuffers(
			0u, 1u, pConstBuf.GetAddressOf()
		);
	};
};

template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C>
{	
public:
	using ConstantBuffer<C>::ConstantBuffer;
	using ConstantBuffer<C>::pConstBuf;
	using Bindable::GetContext;

	void Bind( DXDevice& gfx )
	{
		GetContext( gfx )->PSSetConstantBuffers(
			0u, 1u, pConstBuf.GetAddressOf()
		);
	};
};