#pragma once

#include "Bindable.h"

class InputLayout : public Bindable
{
public:
	InputLayout( const DXDevice &gfx, ID3DBlob *pVSBytecode,
		const std::vector<D3D11_INPUT_ELEMENT_DESC> &layout
	);

	void Bind( const DXDevice &gfx ) override;

protected:
	ComPtr<ID3D11InputLayout> _pInputLayout;
};