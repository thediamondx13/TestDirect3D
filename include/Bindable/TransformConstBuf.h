 #pragma once

#include "ConstantBuffers.h"
#include <Drawable/Drawable.h>

class TransformConstBuf : public Bindable
{
public:
	TransformConstBuf( DXDevice &gfx, const DrawableBase &parent );
	void Bind( DXDevice &gfx ) override;

protected:
	std::unique_ptr<VertexConstantBuffer<DX::XMMATRIX>> _pTransformBuf;
	const DrawableBase &_parent;
};