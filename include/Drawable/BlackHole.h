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
	DX::XMFLOAT3 _pos;
	DX::XMFLOAT3 _velocity;
	DX::XMFLOAT3 _acceleration;

	DX::XMFLOAT3 _rotation;
	DX::XMFLOAT3 _rotationVel;
};