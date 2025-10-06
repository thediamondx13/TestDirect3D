#pragma once

#include "Drawable.h"

class RTXQuad : public Drawable<RTXQuad>
{
public:
	RTXQuad( const DXDevice &gfx );

	inline DX::XMMATRIX GetTransform() const override;

protected:

};