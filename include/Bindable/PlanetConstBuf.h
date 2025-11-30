#pragma once

#include "ConstantBuffers.h"

class Planet;

struct PlanetBuf
{
	DX::XMFLOAT4 color;
};

class PlanetConstBuf : public Bindable
{
public:
	PlanetConstBuf( const DXDevice &gfx, const Planet &planet );
	void Bind( const DXDevice &gfx ) override;

protected:
	PixelConstantBuffer<PlanetBuf> _buf;
	const Planet &_planet;
};