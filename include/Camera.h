#pragma once

#include <DirectXMath.h>
namespace DX = DirectX;

class Camera
{
public:
	Camera( int screenW, int screenH );

	DX::XMFLOAT4 GetPos() const;
	DX::XMFLOAT4 GetResolution() const;
	DX::XMMATRIX GetProjection() const;
	DX::XMMATRIX GetCameraView() const;

	void ProcessMouseDelta( float dPitch, float dYaw );
	void MoveVertical( float dt );
	void MoveSideways( float dt );
	void MoveForward( float dt );

private:
	void UpdateLooks();

	// camera world position
	DX::XMFLOAT4 _position{ 0.0f, 0.0f, 0.0f, 1.0f };

	// pitch (x), yaw (y), roll (z)
	DX::XMFLOAT4 _rotation{ 0.0f, 0.0f, 0.0f, 0.0f };

	// camera forward up and right unit vectors
	DX::XMFLOAT4 _lookF{ 0.0f, 0.0f, 1.0f, 0.0f };
	DX::XMFLOAT4 _lookU{ 0.0f, 1.0f, 0.0f, 0.0f };
	DX::XMFLOAT4 _lookR{ 1.0f, 0.0f, 0.0f, 0.0f };	

	// camera control stuff
	float _sensitivity = 1.0f;
	float _velocity = 20.0f;

	// camera vision stuff
	DX::XMFLOAT4 _resolution;
	float _farZ = 200.0f;
	float _nearZ = 1.0f;
	float _fov = 1.57f;
};