#include <Bindable/TransformConstBuf.h>

TransformConstBuf::TransformConstBuf( const DXDevice &gfx, const DrawableBase &parent ) :
	_transformBuf( VertexConstantBuffer<DX::XMMATRIX>( gfx ) ), _parent( parent )
{}

void TransformConstBuf::Bind( const DXDevice &gfx )
{
	_transformBuf.Update( gfx,
		DX::XMMatrixTranspose(
			_parent.GetTransform() *
			gfx.camera.GetCameraView() *
			gfx.camera.GetProjection()
		)
	);

	_transformBuf.Bind( gfx );
}