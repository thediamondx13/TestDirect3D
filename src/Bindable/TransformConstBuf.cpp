#include <Bindable/TransformConstBuf.h>

TransformConstBuf::TransformConstBuf( DXDevice& gfx, const DrawableBase& parent ) :
	parent( parent )
{
	if ( !pTransformBuf )
		pTransformBuf = std::make_unique<VertexConstantBuffer<DX::XMMATRIX>>( gfx );
}

void TransformConstBuf::Bind( DXDevice& gfx )
{
	pTransformBuf->Update( gfx, 
		DX::XMMatrixTranspose(
			parent.GetTransform() *
			gfx.camera.GetCameraView() *
			gfx.camera.GetProjection()
		)
	);

	pTransformBuf->Bind( gfx );
}

std::unique_ptr<VertexConstantBuffer<DX::XMMATRIX>> TransformConstBuf::pTransformBuf;