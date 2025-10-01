#include <Bindable/CameraConstBuf.h>

CameraConstBuf::CameraConstBuf( DXDevice &gfx, const Camera &cam ) : _cam( cam ),
	_pCamBuf( std::make_unique<PixelConstantBuffer<CamBuf>>( gfx ) )
{}

void CameraConstBuf::Bind( DXDevice &gfx )
{
	CamBuf cb{};
	
	cb.pos = _cam.GetPos();
	cb.res = _cam.GetResolution();
	DX::XMStoreFloat4x4( &cb.viewProjInv,
		DX::XMMatrixTranspose( DX::XMMatrixInverse( nullptr,
			_cam.GetCameraView() * _cam.GetProjection()
		))
	);
	
	_pCamBuf->Update( gfx, cb );

	_pCamBuf->Bind( gfx );
}