#include <Camera.h>

Camera::Camera( int screenW, int screenH ) :
	_resolution( DX::XMFLOAT3( screenW, screenH, static_cast<float>(screenW) / screenH ) )
{
	// _resolution.z stores aspect ratio
}

DX::XMVECTOR Camera::GetRes() const
{
	return DX::XMLoadFloat3( &_resolution );
}

DX::XMVECTOR Camera::GetPos() const
{
	return DX::XMLoadFloat3( &_position );
}

DX::XMMATRIX Camera::GetProjection() const
{
	return DX::XMMatrixPerspectiveLH( _resolution.z, 1.0f, _nearZ, _farZ );
}

DX::XMMATRIX Camera::GetCameraView() const
{
	const DX::XMVECTOR pos = DX::XMLoadFloat3( &_position );

	return DX::XMMatrixLookToLH( pos,
		DX::XMLoadFloat3( &_lookF ),
		DX::XMLoadFloat3( &_lookU )
	);
}

void Camera::MoveSideways( float dt )
{
	const DX::XMVECTOR delta = DX::XMVectorScale( DX::XMLoadFloat3( &_lookR ), _velocity * dt );
	DX::XMStoreFloat3( &_position, DX::XMVectorAdd( DX::XMLoadFloat3( &_position ), delta ) );
}

void Camera::MoveForward( float dt )
{
	const DX::XMVECTOR delta = DX::XMVectorScale( DX::XMLoadFloat3( &_lookF ), _velocity * dt );
	DX::XMStoreFloat3( &_position, DX::XMVectorAdd( DX::XMLoadFloat3( &_position ), delta ) );
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

void Camera::UpdateLooks()
{
	const DX::XMMATRIX rotate = DX::XMMatrixRotationRollPitchYaw(
		_rotation.x, _rotation.y, _rotation.z
	);

	// update forward vector
	DX::XMStoreFloat3( &_lookF, DX::XMVector3Normalize(
		DX::XMVector3Transform( DX::XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f ), rotate )
	) );

	// update up vector
	DX::XMStoreFloat3( &_lookU, DX::XMVector3Normalize(
		DX::XMVector3Transform( DX::XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f ), rotate )
	) );

	// update right vector
	DX::XMStoreFloat3( &_lookR, DX::XMVector3Normalize(
		DX::XMVector3Transform( DX::XMVectorSet( 1.0f, 0.0f, 0.0f, 0.0f ), rotate )
	) );
}