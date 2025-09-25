#include <Bindable/TransformConstBuf.h>

TransformConstBuf::TransformConstBuf( DXDevice &gfx, const DrawableBase &parent ) : _parent( parent ),
	_pTransformBuf( std::make_unique<VertexConstantBuffer<DX::XMMATRIX>>( gfx ) )
{}

void TransformConstBuf::Bind( DXDevice &gfx )
{
	_pTransformBuf->Update( gfx,
		DX::XMMatrixTranspose(
			_parent.GetTransform() *
			gfx.camera.GetCameraView() *
			gfx.camera.GetProjection()
		)
	);

	_pTransformBuf->Bind( gfx );
}