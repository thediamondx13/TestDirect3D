#include <Camera.h>

Camera::Camera( int screenW, int screenH ) :
	_resolution( (float) screenW, (float) screenH, (float) screenW / screenH, 0.0f )
{
	// _resolution.z stores aspect ratio
}

DX::XMFLOAT4 Camera::GetPos() const
{
	return _position;
}

DX::XMFLOAT4 Camera::GetResolution() const
{
	return _resolution;
}

DX::XMMATRIX Camera::GetProjection() const
{
	return DX::XMMatrixPerspectiveFovLH( _fov, _resolution.z, _nearZ, _farZ );
}

DX::XMMATRIX Camera::GetCameraView() const
{
	return DX::XMMatrixLookToLH(
		DX::XMLoadFloat4( &_position ),
		DX::XMLoadFloat4( &_lookF ),
		DX::XMLoadFloat4( &_lookU )
	);
}

void Camera::ProcessMouseDelta( float dPitch, float dYaw )
{
	/* do not use _rotation directly as the .x
	 component is inverted, use _look(F/U/R) */

	_rotation.x += _sensitivity * dPitch;
	if ( _rotation.x > +1.57f ) _rotation.x = +1.57f;
	if ( _rotation.x < -1.57f ) _rotation.x = -1.57f;

	_rotation.y -= _sensitivity * dYaw;
	if ( _rotation.y > 6.28f ) _rotation.y -= 6.28f;
	if ( _rotation.y < 0.00f ) _rotation.y += 6.28f;

	UpdateLooks();
}

void Camera::MoveVertical( float dt )
{
	const DX::XMVECTOR delta = DX::XMVectorScale(
		DX::XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f ), _velocity * dt
	);

	// scale this vector and add to position
	DX::XMStoreFloat4( &_position,
		DX::XMVectorAdd( DX::XMLoadFloat4( &_position ), delta )
	);
}

void Camera::MoveSideways( float dt )
{
	// right look vector projected onto XZ plane
	const DX::XMVECTOR rightXZ = DX::XMVector3Normalize(
		DX::XMVectorSetY( DX::XMLoadFloat4( &_lookR ), 0.0f )
	);

	// scale this vector and add to position
	DX::XMStoreFloat4( &_position,
		DX::XMVectorAdd( DX::XMLoadFloat4( &_position ),
			DX::XMVectorScale( rightXZ, _velocity * dt )
		)
	);
}

void Camera::MoveForward( float dt )
{
	// forward look vector projected onto XZ plane
	const DX::XMVECTOR forwardXZ = DX::XMVector3Normalize(
		DX::XMVectorSetY( DX::XMLoadFloat4( &_lookF ), 0.0f )
	);

	// scale this vector and add to position
	DX::XMStoreFloat4( &_position,
		DX::XMVectorAdd( DX::XMLoadFloat4( &_position ),
			DX::XMVectorScale( forwardXZ, _velocity * dt )
		)
	);
}

void Camera::UpdateLooks()
{
	const DX::XMMATRIX rotate = DX::XMMatrixRotationRollPitchYaw(
		_rotation.x, _rotation.y, _rotation.z
	);

	// update forward vector
	DX::XMStoreFloat4( &_lookF, DX::XMVector3Normalize(
		DX::XMVector3TransformNormal( DX::XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f ), rotate )
	) );

	// update up vector
	DX::XMStoreFloat4( &_lookU, DX::XMVector3Normalize(
		DX::XMVector3TransformNormal( DX::XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f ), rotate )
	) );

	// update right vector
	DX::XMStoreFloat4( &_lookR, DX::XMVector3Normalize(
		DX::XMVector3TransformNormal( DX::XMVectorSet( 1.0f, 0.0f, 0.0f, 0.0f ), rotate )
	) );
}