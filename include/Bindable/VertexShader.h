#pragma once

#include "Bindable.h"

class VertexShader : public Bindable
{
public:
	VertexShader( const DXDevice &gfx, LPCWSTR path );
	void Bind( const DXDevice &gfx ) override;
	ID3DBlob *GetByteCode();

protected:
	ComPtr<ID3D11VertexShader> _pVertexShader;
	ComPtr<ID3DBlob> _pVertexShaderBlob;
};