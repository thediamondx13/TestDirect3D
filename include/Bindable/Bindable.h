#pragma once

#include <DXDevice.h>

class Bindable
{
public:
	virtual void Bind( DXDevice& gfx ) = 0;
	virtual ~Bindable() = default;

protected:
	static ID3D11DeviceContext* GetContext( DXDevice& gfx );
	static ID3D11Device* GetDevice( DXDevice& gfx );
};