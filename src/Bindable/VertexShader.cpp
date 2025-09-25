#include <Bindable/VertexShader.h>

VertexShader::VertexShader( DXDevice &gfx, LPCWSTR path )
{
	// read file data
	D3DReadFileToBlob( path, &_pVertexShaderBlob );

	// create vertex shader COM
	GetDevice( gfx )->CreateVertexShader(
		_pVertexShaderBlob->GetBufferPointer(),
		_pVertexShaderBlob->GetBufferSize(),
		nullptr, &_pVertexShader
	);
}

void VertexShader::Bind( DXDevice &gfx )
{
	GetContext( gfx )->VSSetShader(
		_pVertexShader.Get(), nullptr, 0u
	);
}

ID3DBlob *VertexShader::GetByteCode()
{
	return _pVertexShaderBlob.Get();
}