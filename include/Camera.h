#pragma once

#include <DirectXMath.h>
#define DX DirectX

class Camera
{
public:
	Camera(int screenW, int screenH);
	Camera( const Camera & ) = delete;
	Camera &operator=( const Camera & ) = delete;

	DX::XMMATRIX GetCameraView() const;
	DX::XMMATRIX GetProjection() const;

	void ProcessMouseDelta( float dPitch, float dYaw );

	void MoveSideways( float dt );
	void MoveForward( float dt );

	~Camera() = default;
private:

	void UpdateLooks();

	// pitch (x), yaw (y), roll (z)
	DX::XMFLOAT3 _rotation{ 0.0f, 0.0f, 0.0f };
	DX::XMFLOAT3 _position{ 0.0f, 0.0f, 0.0f };
	DX::XMFLOAT3 _lookF{ 0.0f, 0.0f, 1.0f };
	DX::XMFLOAT3 _lookU{ 0.0f, 1.0f, 0.0f };
	DX::XMFLOAT3 _lookR{ 1.0f, 0.0f, 0.0f };	

	float _sensitivity = 1.0f;
	float _velocity = 20.0f;
	float _farZ = 200.0f;
	float _nearZ = 1.0f;
	float _aspectRatio;

	int _width, _height;
};