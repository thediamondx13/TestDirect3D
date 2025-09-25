#pragma once

#include "Bindable.h"

class PixelShader : public Bindable
{
public:
	PixelShader( DXDevice &gfx, const LPCWSTR path );
	void Bind( DXDevice &gfx ) override;

protected:
	ComPtr<ID3D11PixelShader> _pPixelShader;
};