#pragma once

#include "Bindable.h"

class VertexShader : public Bindable
{
public:
	VertexShader( DXDevice &gfx, LPCWSTR path );
	void Bind( DXDevice &gfx ) override;
	ID3DBlob *GetByteCode();

protected:
	ComPtr<ID3D11VertexShader> _pVertexShader;
	ComPtr<ID3DBlob> _pVertexShaderBlob;
};