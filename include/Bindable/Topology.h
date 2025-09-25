#pragma once

#include "Bindable.h"

class Topology : public Bindable
{
public:
	Topology( DXDevice &gfx, D3D11_PRIMITIVE_TOPOLOGY type );
	void Bind( DXDevice &gfx ) override;

protected:
	D3D11_PRIMITIVE_TOPOLOGY _type;
};