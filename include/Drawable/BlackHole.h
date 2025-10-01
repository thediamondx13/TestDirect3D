#pragma once

#include "Drawable.h"

class BlackHole : public Drawable<BlackHole>
{
public:
	BlackHole( DXDevice &gfx, Camera &cam );

	inline DX::XMMATRIX GetTransform() const override;
	void Update( float dt ) override;

protected:

	float _radius;
	DX::XMFLOAT4 _pos;
	DX::XMFLOAT4 _velocity;
	DX::XMFLOAT4 _acceleration;

	DX::XMFLOAT4 _rotation;
	DX::XMFLOAT4 _rotationVel;
};