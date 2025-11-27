#pragma once

#include "Bindable.h"

template<typename C>
class ConstantBuffer : public Bindable
{
public:
	ConstantBuffer( const DXDevice &gfx, const C &consts )
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

		GetDevice( gfx ).CreateBuffer( &cbd, &csd, &_pConstBuf );
	}

	ConstantBuffer( const DXDevice &gfx )
	{
		const D3D11_BUFFER_DESC cbd{
			sizeof( C ),
			D3D11_USAGE_DYNAMIC,
			D3D11_BIND_CONSTANT_BUFFER,
			D3D11_CPU_ACCESS_WRITE, 0u, 0u
		};

		GetDevice( gfx ).CreateBuffer( &cbd, nullptr, &_pConstBuf );
	}

	void Update( const DXDevice &gfx, const C &consts )
	{
		D3D11_MAPPED_SUBRESOURCE msr;

		// map the buffer
		GetContext( gfx ).Map( _pConstBuf.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr );

		// update the data
		memcpy( msr.pData, &consts, sizeof( consts ) );

		// unmap the buffer
		GetContext( gfx ).Unmap( _pConstBuf.Get(), 0u );
	}
protected:
	ComPtr<ID3D11Buffer> _pConstBuf;
};

template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
public:
	using ConstantBuffer<C>::ConstantBuffer;
	using ConstantBuffer<C>::_pConstBuf;
	using Bindable::GetContext;

	void Bind( const DXDevice &gfx )
	{
		GetContext( gfx ).VSSetConstantBuffers(
			0u, 1u, _pConstBuf.GetAddressOf()
		);
	};
};

template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
public:
	using ConstantBuffer<C>::ConstantBuffer;
	using ConstantBuffer<C>::_pConstBuf;
	using Bindable::GetContext;

	void Bind( const DXDevice &gfx )
	{
		GetContext( gfx ).PSSetConstantBuffers(
			0u, 1u, _pConstBuf.GetAddressOf()
		);
	};

	void Bind( const DXDevice &gfx, const UINT slot )
	{
		GetContext( gfx ).PSSetConstantBuffers(
			slot, 1u, _pConstBuf.GetAddressOf()
		);
	};
};