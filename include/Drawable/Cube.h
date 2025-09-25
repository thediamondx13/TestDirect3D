#pragma once

#include "Drawable.h"

class Cube : public Drawable<Cube>
{
public:
	Cube( DXDevice &gfx );

	inline DX::XMMATRIX GetTransform() const override;
	void Update( float dt ) override;

protected:
	// distance to center
	float r;

	// rotation around world center
	float theta, phi, chi;

	// rotation around self center
	float roll, pitch, yaw;

	// speed
	float droll, dpitch, dyaw;
	float dtheta, dphi, dchi;
	float dr;
};