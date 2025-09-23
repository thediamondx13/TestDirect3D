#pragma once

#include "Drawable.h"

class Sphere : public Drawable<Sphere>
{
public:
	Sphere( DXDevice &gfx );	

	inline DX::XMMATRIX GetTransform() const override;
	void Update( float dt ) override;

	~Sphere() = default;

protected:
	
	float radius;
	DX::XMFLOAT3 pos;
	DX::XMFLOAT3 velocity;
	DX::XMFLOAT3 acceleration;
	
	DX::XMFLOAT3 rotation;
	DX::XMFLOAT3 rotationVel;
};