#include <Controls/Mouse.h>

std::optional<Mouse::Event> Mouse::ReadEvent()
{
	if ( _eventBuffer.empty() ) return std::nullopt;
	Mouse::Event e = _eventBuffer.front();
	_eventBuffer.pop();
	return e;
}

void Mouse::OnButtonDown( UINT msg, WPARAM wParam, LPARAM lParam )
{
	POINTS pos = MAKEPOINTS( lParam );
	Event::Button button = Event::NONE;

	switch ( msg )
	{
	case WM_LBUTTONDOWN:
		button = Event::LEFT;
		break;

	case WM_RBUTTONDOWN:
		button = Event::RIGHT;
		break;

	case WM_MBUTTONDOWN:
		button = Event::MIDDLE;
		break;

	case WM_XBUTTONDOWN:
		switch ( GET_XBUTTON_WPARAM( wParam ) )
		{
		case XBUTTON1:
			button = Event::FORWARD;
			break;

		case XBUTTON2:
			button = Event::BACK;
			break;
		}
	}

	_eventBuffer.push( Event( Event::K_DOWN, button, std::nullopt, pos ) );
	if ( _eventBuffer.size() > S_BUFFER_SIZE ) _eventBuffer.pop();
	_buttonStates.set( button );
}

void Mouse::OnButtonUp( UINT msg, WPARAM wParam, LPARAM lParam )
{
	POINTS pos = MAKEPOINTS( lParam );
	Event::Button button = Event::NONE;

	switch ( msg )
	{
	case WM_LBUTTONUP:
		button = Event::LEFT;
		break;

	case WM_RBUTTONUP:
		button = Event::RIGHT;
		break;

	case WM_MBUTTONUP:
		button = Event::MIDDLE;
		break;

	case WM_XBUTTONUP:
		switch ( GET_XBUTTON_WPARAM( wParam ) )
		{
		case XBUTTON1:
			button = Event::FORWARD;
			break;

		case XBUTTON2:
			button = Event::BACK;
			break;
		}
	}

	_eventBuffer.push( Event( Event::K_UP, button, std::nullopt, pos ) );
	if ( _eventBuffer.size() > S_BUFFER_SIZE ) _eventBuffer.pop();
	_buttonStates.set( button );
}

void Mouse::OnScroll( short delta, Pos pos )
{
	_wheelDeltaCarry += delta;

	while ( _wheelDeltaCarry >= WHEEL_DELTA )
	{
		_eventBuffer.push( Event( Event::WHL_UP, Event::NONE, std::nullopt, pos ) );
		if ( _eventBuffer.size() > S_BUFFER_SIZE ) _eventBuffer.pop();
		_wheelDeltaCarry -= WHEEL_DELTA;
	}

	while ( _wheelDeltaCarry <= -WHEEL_DELTA )
	{
		_eventBuffer.push( Event( Event::WHL_DOWN, Event::NONE, std::nullopt, pos ) );
		if ( _eventBuffer.size() > S_BUFFER_SIZE ) _eventBuffer.pop();
		_wheelDeltaCarry += WHEEL_DELTA;
	}
}

void Mouse::OnMouseMove( Pos pos )
{
	if ( _isCaptured ) {}

	_eventBuffer.push( Event( Event::NEW_POS, Event::NONE, _pos - pos, pos ) );
	if ( _eventBuffer.size() > S_BUFFER_SIZE ) _eventBuffer.pop();
	_pos = pos;
}

void Mouse::Capture( HWND hWnd )
{
	_eventBuffer.push( Event( Event::CAPTURE, Event::NONE, std::nullopt, _pos ) );
	if ( _eventBuffer.size() > S_BUFFER_SIZE ) _eventBuffer.pop();
	_isCaptured = true;
	SetCapture( hWnd );
}

void Mouse::Release()
{
	_eventBuffer.push( Event( Event::RELESE, Event::NONE, std::nullopt, _pos ) );
	if ( _eventBuffer.size() > S_BUFFER_SIZE ) _eventBuffer.pop();
	_isCaptured = false;
	ReleaseCapture();
}