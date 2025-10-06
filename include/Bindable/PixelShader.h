#pragma once

#include "Bindable.h"

class PixelShader : public Bindable
{
public:
	PixelShader( const DXDevice &gfx, const LPCWSTR path );
	void Bind( const DXDevice &gfx ) override;

protected:
	ComPtr<ID3D11PixelShader> _pPixelShader;
};