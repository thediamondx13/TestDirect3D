#pragma once

#include "Drawable.h"

class Planet : public Drawable<Planet>
{
public:
	Planet( DXDevice &gfx );

	inline DX::XMMATRIX GetTransform() const override;
	inline DX::XMVECTOR GetInfo() const;
	void Update( float dt ) override;

protected:

	float _radius;
	DX::XMFLOAT3 _pos;
	DX::XMFLOAT3 _velocity;
	DX::XMFLOAT3 _acceleration;

	DX::XMFLOAT3 _rotation;
	DX::XMFLOAT3 _rotationVel;
};