#include <Mouse.h>

std::optional<Mouse::Event> Mouse::ReadEvent()
{
	if( eventBuffer.empty() ) return std::nullopt;
	Mouse::Event e = eventBuffer.front();
	eventBuffer.pop();
	return e;
}

void Mouse::OnMouseEnter() noexcept
{
	isInWindow = true;
	eventBuffer.push( Event( Event::ENTER, Event::NONE, pos ) );
	if( eventBuffer.size() > bufferSize ) eventBuffer.pop();
}

void Mouse::OnMouseMove( POINTS pos ) noexcept
{
	this->pos = pos;
	eventBuffer.push( Event( Event::NEW_POS, Event::NONE, pos ) );
	if ( eventBuffer.size() > bufferSize ) eventBuffer.pop();
}

void Mouse::OnButtonDown( UINT msg, WPARAM wParam, LPARAM lParam ) noexcept
{
	POINTS pos = MAKEPOINTS( lParam );
	Event::Button button = Event::NONE;

	switch (msg)
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
		switch( GET_XBUTTON_WPARAM( wParam ) )
		{
		case XBUTTON1:
			button = Event::FORWARD;
			break;

		case XBUTTON2:
			button = Event::BACK;
			break;
		}
	}

	buttonStates.set( button );
	eventBuffer.push( Event( Event::K_DOWN, button, pos ) );
	if( eventBuffer.size() > bufferSize ) eventBuffer.pop();
}
void Mouse::OnButtonUp( UINT msg, WPARAM wParam, LPARAM lParam ) noexcept
{
	POINTS pos = MAKEPOINTS( lParam );
	Event::Button button = Event::NONE;

	switch (msg)
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
		switch( GET_XBUTTON_WPARAM( wParam ) ) 
		{
		case XBUTTON1: 
			button = Event::FORWARD;
			break;

		case XBUTTON2: 
			button = Event::BACK;
			break;
		}
	}

	buttonStates.set( button );
	eventBuffer.push( Event( Event::K_UP, button, pos ) );
	if( eventBuffer.size() > bufferSize ) eventBuffer.pop();
}

void Mouse::OnScroll( short delta, POINTS pos ) noexcept
{
	wheelDeltaCarry += delta;

	while( wheelDeltaCarry >= WHEEL_DELTA )
	{
		eventBuffer.push( Event( Event::WHL_UP, Event::NONE, pos ) );
		if( eventBuffer.size() > bufferSize ) eventBuffer.pop();
		wheelDeltaCarry -= WHEEL_DELTA;
	}

	while( wheelDeltaCarry <= -WHEEL_DELTA )
	{
		eventBuffer.push( Event( Event::WHL_DOWN, Event::NONE, pos ) );
		if( eventBuffer.size() > bufferSize ) eventBuffer.pop();
		wheelDeltaCarry += WHEEL_DELTA;
	}
}

void Mouse::OnMouseLeave() noexcept
{
	isInWindow = false;
	eventBuffer.push( Event( Event::LEAVE, Event::NONE, pos ) );
	if( eventBuffer.size() > bufferSize ) eventBuffer.pop();
}