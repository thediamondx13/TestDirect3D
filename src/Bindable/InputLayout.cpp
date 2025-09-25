#include <Bindable/InputLayout.h>

InputLayout::InputLayout( DXDevice &gfx, ID3DBlob *pVertexShaderBytecode,
	const std::vector<D3D11_INPUT_ELEMENT_DESC> &layout )
{
	GetDevice( gfx )->CreateInputLayout(
		layout.data(), static_cast<UINT>(layout.size()),
		pVertexShaderBytecode->GetBufferPointer(),
		pVertexShaderBytecode->GetBufferSize(),
		&_pInputLayout
	);
}

void InputLayout::Bind( DXDevice &gfx )
{
	GetContext( gfx )->IASetInputLayout( _pInputLayout.Get() );
}