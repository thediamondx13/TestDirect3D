#pragma once

#include "Drawable.h"

class Planet : public Drawable<Planet>
{
public:
	Planet( DXDevice &gfx );
	void Update( float dt ) override;
	inline DX::XMMATRIX GetTransform() const override;

protected:
	float _radius;
	DX::XMFLOAT4 _pos;
	DX::XMFLOAT4 _velocity;
	DX::XMFLOAT4 _acceleration;

	DX::XMFLOAT4 _rotation;
	DX::XMFLOAT4 _rotationVel;
};