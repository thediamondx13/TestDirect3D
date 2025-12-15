#pragma once

// std includes
#include <memory>

// drawable objects
#include <Drawable/BlackHole.h>
#include <Drawable/Planet.h>
#include <Drawable/RTXQuad.h>

struct PlanetStruct
{
	/* .xyz contain position in space
	 and .w contains the radius */
	DX::XMFLOAT4 info;
	DX::XMFLOAT4 color;
};

struct PlanetBuffer
{
	DX::XMUINT4 count;
	PlanetStruct planets[100];
};

struct BlackHoleBuffer
{
	DX::XMUINT4 count;
	DX::XMFLOAT4 info[100];
	/* .xyz contain position in space
	 and .w contains the radius */
};

class Universe
{
public:
	Universe( const DXDevice &gfx );
	void Draw( const DXDevice &gfx );
	
	void Update( float dt );

	void EnableRTX() { _useRTX = true; }
	void DisableRTX() { _useRTX = false; }
	void ToggleRTX() { _useRTX = !_useRTX; }

	void CreatePlanet( const DXDevice& gfx );
	void DeleteLastPlanet();

protected:

	bool _useRTX;

	RTXQuad _quad;
	
	std::vector<std::unique_ptr<BlackHole>> _blackHoles;

	PixelConstantBuffer<BlackHoleBuffer> _blackHoleBuffer;


	std::vector<std::unique_ptr<Planet>> _planets;

	PixelConstantBuffer<PlanetBuffer> _planetBuffer;

	void BindBlackHoleBuffer( const DXDevice &gfx );
	void BindPlanetBuffer( const DXDevice &gfx );
};