#pragma once

//#include <DirectXMath.h>
//#define DX DirectX

#include <DirectXMath.h>
#define DX DirectX

class Camera
{
public:
	Camera( int screenW, int screenH );

	DX::XMMATRIX GetCameraView() const;
	DX::XMMATRIX GetProjection() const;

	DX::XMVECTOR GetLook() const;
	DX::XMVECTOR GetPos() const;

	void ProcessMouseDelta( float dPitch, float dYaw );
	void MoveSideways( float dt );
	void MoveForward( float dt );

private:
	void UpdateLooks();

	// pitch (x), yaw (y), roll (z)
	DX::XMFLOAT3 _rotation{ 0.0f, 0.0f, 0.0f };

	// camera forward up and right unit vectors
	DX::XMFLOAT3 _lookF{ 0.0f, 0.0f, 1.0f };
	DX::XMFLOAT3 _lookU{ 0.0f, 1.0f, 0.0f };
	DX::XMFLOAT3 _lookR{ 1.0f, 0.0f, 0.0f };

	// camera world position ( inverted )
	DX::XMFLOAT3 _position{ 0.0f, 0.0f, 0.0f };

	// camera control stuff
	float _sensitivity = 1.0f;
	float _velocity = 20.0f;

	// camera vision stuff
	float _farZ = 200.0f;
	float _nearZ = 1.0f;
	float _aspectRatio;
};