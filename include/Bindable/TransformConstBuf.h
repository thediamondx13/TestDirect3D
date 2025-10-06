 #pragma once

#include "ConstantBuffers.h"
#include <Drawable/Drawable.h>

class TransformConstBuf : public Bindable
{
public:
	TransformConstBuf( const DXDevice &gfx, const DrawableBase &parent );
	void Bind( const DXDevice &gfx ) override;

protected:
	VertexConstantBuffer<DX::XMMATRIX> _transformBuf;
	const DrawableBase &_parent;
};