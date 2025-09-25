#include <Bindable/PixelShader.h>

PixelShader::PixelShader( DXDevice &gfx, const LPCWSTR path )
{
	ComPtr<ID3DBlob> pPixelShaderBlob;

	// read file data
	D3DReadFileToBlob( path, &pPixelShaderBlob );

	// create pixel shader COM
	GetDevice( gfx )->CreatePixelShader(
		pPixelShaderBlob->GetBufferPointer(),
		pPixelShaderBlob->GetBufferSize(),
		nullptr, &_pPixelShader
	);
}

void PixelShader::Bind( DXDevice &gfx )
{
	GetContext( gfx )->PSSetShader( _pPixelShader.Get(), nullptr, 0u );
}