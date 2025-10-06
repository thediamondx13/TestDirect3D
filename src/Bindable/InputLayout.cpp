#include <Bindable/InputLayout.h>

InputLayout::InputLayout( const DXDevice &gfx, ID3DBlob *pVSBytecode,
	const std::vector<D3D11_INPUT_ELEMENT_DESC> &layout )
{
	GetDevice( gfx ).CreateInputLayout(
		layout.data(), static_cast<UINT>(layout.size()),
		pVSBytecode->GetBufferPointer(),
		pVSBytecode->GetBufferSize(),
		&_pInputLayout
	);
}

void InputLayout::Bind( const DXDevice &gfx )
{
	GetContext( gfx ).IASetInputLayout( _pInputLayout.Get() );
}