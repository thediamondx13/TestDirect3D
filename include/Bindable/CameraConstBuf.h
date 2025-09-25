#pragma once

#include "ConstantBuffers.h"
#include <Camera.h>
#include <memory>

struct CamBuf
{
	DX::XMMATRIX proj;
	DX::XMMATRIX view;
	DX::XMVECTOR look;
	DX::XMVECTOR pos;
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