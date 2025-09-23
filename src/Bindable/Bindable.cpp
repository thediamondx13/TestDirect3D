#include <Bindable/Bindable.h>

ID3D11Device* Bindable::GetDevice( DXDevice& gfx )
{
	return gfx.pDevice.Get();
}

ID3D11DeviceContext* Bindable::GetContext( DXDevice& gfx )
{
	return gfx.pContext.Get();
}