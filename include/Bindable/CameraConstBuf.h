#pragma once

#include "ConstantBuffers.h"

struct CamBuf
{
	DX::XMFLOAT4X4 viewProjInv;
	DX::XMFLOAT4 pos;
	DX::XMFLOAT4 res;
};

class CameraConstBuf : public Bindable
{
public:
	CameraConstBuf( const DXDevice &gfx );
	void Bind( const DXDevice &gfx ) override;

protected:
	PixelConstantBuffer<CamBuf> _camBuf;
};