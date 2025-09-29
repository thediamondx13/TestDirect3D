#pragma once

#include "ConstantBuffers.h"
#include <Camera.h>
#include <memory>

struct CamBuf
{
	DX::XMMATRIX view;
	DX::XMVECTOR pos;
	DX::XMVECTOR res;
};

class CameraConstBuf : public Bindable
{
public:
	CameraConstBuf( DXDevice &gfx, const Camera &cam );
	void Bind( DXDevice &gfx ) override;

protected:
	std::unique_ptr<PixelConstantBuffer<CamBuf>> _pCamBuf;
	const Camera &_cam;
};