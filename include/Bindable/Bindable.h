#pragma once

#include <DXDevice.h>

class Bindable
{
public:
	Bindable() = default;
	
	virtual void Bind( const DXDevice &gfx ) = 0;

protected:
	static ID3D11Device &GetDevice( const DXDevice &gfx );
	static ID3D11DeviceContext &GetContext( const DXDevice &gfx );
};