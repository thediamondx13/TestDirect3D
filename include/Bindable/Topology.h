#pragma once

#include "Bindable.h"

class Topology : public Bindable
{
public:
	Topology( const DXDevice &gfx, const D3D11_PRIMITIVE_TOPOLOGY type );
	void Bind( const DXDevice &gfx ) override;

protected:
	D3D11_PRIMITIVE_TOPOLOGY _type;
};