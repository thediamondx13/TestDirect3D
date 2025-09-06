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
	class Event
	{
	public:
		enum Type { ENTER, NEW_POS, K_DOWN, K_UP, WHL_DOWN, WHL_UP, LEAVE };
		enum Button { LEFT, RIGHT, MIDDLE, FORWARD, BACK, NONE };
	
		Event( Type type, Button button, POINTS pos ) 
			: type( type ), button( button ), pos( pos ) {}
		
		Button GetButton() const { return button; }
		POINTS GetPos() const { return pos; }
		Type GetType() const { return type; }
	private:
		Button button;
		POINTS pos;
		Type type;
	};

	Mouse() : pos( POINTS(0, 0) ) {};
	Mouse( const Mouse& ) = delete;
	Mouse& operator=( const Mouse& ) = delete;

	POINTS GetPos() const { return pos; }

	bool IsInWindow() const { return isInWindow; }

	std::optional<Event> ReadEvent();
	void FlushEventBuffer();

private:
	void OnMouseMove( POINTS pos );
	
	void OnButtonDown( UINT msg, WPARAM wParam, LPARAM lParam );
	void OnButtonUp( UINT msg, WPARAM wParam, LPARAM lParam );

	void OnScroll( short delta, POINTS pos );

	void OnMouseLeave();
	void OnMouseEnter();

	static constexpr unsigned int bufferSize = 16u;
	static constexpr unsigned int nButtons = 5u;

	std::bitset<nButtons> buttonStates;
	std::queue<Event> eventBuffer;
	short wheelDeltaCarry = 0;
	bool isInWindow = false;
	POINTS pos;
};