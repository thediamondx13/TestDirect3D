#pragma once

// standard includes
#include <Windows.h>
#include <optional>
#include <bitset>
#include <queue>

class Mouse
{
	friend class Window;

public:
	struct Pos
	{
		Pos( POINT p ) : x( p.x ), y( p.y ) {}
		Pos( POINTS p ) : x( p.x ), y( p.y ) {}
		Pos( int x, int y ) : x( x ), y( y ) {}

		Pos operator+( const Pos &rhs ) const
		{
			return Pos( x + rhs.x, y + rhs.y );
		}
		Pos operator-( const Pos &rhs ) const
		{
			return Pos( x - rhs.x, y - rhs.y );
		}

		int x, y;
	};

	class Event
	{
	public:
		enum Type { CAPTURE, NEW_POS, K_DOWN, K_UP, WHL_DOWN, WHL_UP, RELESE };
		enum Button { LEFT, RIGHT, MIDDLE, FORWARD, BACK, NONE };

		Event( Type type, Button button, std::optional<Pos> delta, Pos pos )
			: type( type ), button( button ), delta( delta ), pos( pos )
		{}

		std::optional<Pos> delta;
		Button button;
		Type type;
		Pos pos;
	};

	Mouse() : _pos( Pos( 0, 0 ) ) {};

	bool IsCaptured() const { return _isCaptured; }

	Pos GetPos() const { return _pos; }

	void SetPosition( Pos newPos ) { _pos = newPos; }
	std::optional<Event> ReadEvent();
	void FlushEventBuffer();

protected:
	void OnButtonDown( UINT msg, WPARAM wParam, LPARAM lParam );
	void OnButtonUp( UINT msg, WPARAM wParam, LPARAM lParam );
	void OnScroll( short delta, Pos pos );
	void OnMouseMove( Pos pos );
	void Capture( HWND hWnd );
	void Release();

	static constexpr unsigned int S_BUFFER_SIZE = 16u;
	static constexpr unsigned int S_N_BUTTONS = 5u;

	std::bitset<S_N_BUTTONS> _buttonStates;
	std::queue<Event> _eventBuffer;
	short _wheelDeltaCarry = 0;
	bool _isCaptured = false;
	Pos _pos;
};