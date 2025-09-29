#pragma once

//#include <DirectXMath.h>
//#define DX DirectX

#include <DirectXMath.h>
#define DX DirectX

class Camera
{
public:
	Camera( int screenW, int screenH );

	DX::XMVECTOR GetRes() const;
	DX::XMVECTOR GetPos() const;
	DX::XMMATRIX GetProjection() const;
	DX::XMMATRIX GetCameraView() const;

	void ProcessMouseDelta( float dPitch, float dYaw );
	void MoveSideways( float dt );
	void MoveForward( float dt );

private:
	void UpdateLooks();

	// camera world position
	DX::XMFLOAT3 _position{ 0, 0, 0 };

	// pitch (x), yaw (y), roll (z)
	DX::XMFLOAT3 _rotation{ 0, 0, 0 };

	// camera forward up and right unit vectors
	DX::XMFLOAT3 _lookF{ 0, 0, 1 };
	DX::XMFLOAT3 _lookU{ 0, 1, 0 };
	DX::XMFLOAT3 _lookR{ 1, 0, 0 };	

	// camera control stuff
	float _sensitivity = 1.0f;
	float _velocity = 20.0f;

	// camera vision stuff
	DX::XMFLOAT3 _resolution;
	float _farZ = 200.0f;
	float _nearZ = 1.0f;
};