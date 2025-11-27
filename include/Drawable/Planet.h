#pragma once

#include "Drawable.h"

class Planet : public Drawable<Planet>
{
public:
	Planet( const DXDevice &gfx );

	inline DX::XMMATRIX GetTransform() const override;
	
	void SetAcceleration( const DX::XMVECTOR &a );
	
	// velocity set/get
	void SetVelocity( const DX::XMVECTOR &s );
	DX::XMVECTOR GetVelocity() const;

	// position set/get
	void SetPosition( const DX::XMVECTOR &p );
	DX::XMVECTOR GetPosition() const;

	// radius set/get
	void SetRadius( const float newRadius );
	float GetRadius() const;

	// returns position in .xyz and radius in .w
	DX::XMFLOAT4 GetInfo() const;

	// mass set/get
	void SetMass( const float newMass );
	float GetMass() const;

	void Update( float dt );

protected:

	float _mass = 0.0f;
	float _radius = 0.0f;

	// position
	DX::XMFLOAT4 _pos = { 0.0f, 0.0f, 0.0f, 1.0f };
	DX::XMFLOAT4 _velocity = { 0.0f, 0.0f, 0.0f, 0.0f };
	DX::XMFLOAT4 _acceleration = { 0.0f, 0.0f, 0.0f, 0.0f };

	// rotation
	DX::XMFLOAT4 _rotation = { 0.0f, 0.0f, 0.0f, 0.0f };
	DX::XMFLOAT4 _rotationVel = { 0.0f, 0.0f, 0.0f, 0.0f };
};