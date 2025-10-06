#pragma once

// std includes
#include <memory>

// drawable objects
#include <Drawable/Planet.h>
#include <Drawable/RTXQuad.h>

class Universe
{
public:
	Universe( const DXDevice &gfx );
	void Draw( const DXDevice &gfx );
	
	void Update( float dt );

	void EnableRTX() { _useRTX = true; }
	void DisableRTX() { _useRTX = false; }
	void ToggleRTX() { _useRTX = !_useRTX; }

protected:

	bool _useRTX;

	RTXQuad _quad;
	//std::vector<std::unique_ptr<Planet>> _planets;
};