#include <Bindable/Bindable.h>

ID3D11Device &Bindable::GetDevice( const DXDevice &gfx )
{
	return *gfx.pDevice.Get();
}

ID3D11DeviceContext &Bindable::GetContext( const DXDevice &gfx )
{
	return *gfx.pContext.Get();
}