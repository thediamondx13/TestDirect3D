#include <Bindable/VertexShader.h>

VertexShader::VertexShader( DXDevice& gfx, LPCWSTR path )
{
	// read file data
	D3DReadFileToBlob( path, &pVertexShaderBlob );

	// create vertex shader COM
	GetDevice(gfx)->CreateVertexShader(
		pVertexShaderBlob->GetBufferPointer(),
		pVertexShaderBlob->GetBufferSize(),
		nullptr, &pVertexShader
	);
}

void VertexShader::Bind( DXDevice& gfx )
{
	GetContext( gfx )->VSSetShader(
		pVertexShader.Get(), nullptr, 0u
	);
}

ID3DBlob* VertexShader::GetByteCode()
{
	return pVertexShaderBlob.Get();
}