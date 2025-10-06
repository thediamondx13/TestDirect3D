#include <Bindable/CameraConstBuf.h>

CameraConstBuf::CameraConstBuf( const DXDevice &gfx ) :
	_camBuf( PixelConstantBuffer<CamBuf>( gfx ) )
{}

void CameraConstBuf::Bind( const DXDevice &gfx )
{
	CamBuf cb{};
	
	cb.pos = gfx.camera.GetPos();
	cb.res = gfx.camera.GetResolution();
	DX::XMStoreFloat4x4( &cb.viewProjInv,
		DX::XMMatrixTranspose( DX::XMMatrixInverse( nullptr,
			gfx.camera.GetCameraView() * gfx.camera.GetProjection()
		))
	);
	
	_camBuf.Update( gfx, cb );

	_camBuf.Bind( gfx );
}