#include <Bindable/CameraConstBuf.h>

CameraConstBuf::CameraConstBuf( DXDevice &gfx, const Camera &cam ) : _cam( cam ),
	_pCamBuf( std::make_unique<PixelConstantBuffer<CamBuf>>( gfx ) )
{}

void CameraConstBuf::Bind( DXDevice &gfx )
{
	const CamBuf cb{
		_cam.GetProjection(),
		_cam.GetCameraView(),
		_cam.GetLook(),
		_cam.GetPos()
	};

	_pCamBuf->Update( gfx, cb );

	_pCamBuf->Bind( gfx );
}