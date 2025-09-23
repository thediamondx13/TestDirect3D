#pragma once

#include "Bindable.h"

class InputLayout : public Bindable
{
public:
	InputLayout( DXDevice& gfx, ID3DBlob* pVertexShaderBytecode,
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout
	);

	void Bind( DXDevice& gfx ) override;

protected:
	ComPtr<ID3D11InputLayout> pInputLayout;
};