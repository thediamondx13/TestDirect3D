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
	
		Event( Type type, Button button, POINTS pos ) noexcept 
			: type( type ), button( button ), pos( pos ) {}
		
		Button GetButton() const noexcept { return button; }
		POINTS GetPos() const noexcept { return pos; }
		Type GetType() const noexcept { return type; }
	private:
		Button button;
		POINTS pos;
		Type type;
	};

	Mouse() : pos( POINTS(0, 0) ) {};
	Mouse( const Mouse& ) = delete;
	Mouse& operator=( const Mouse& ) = delete;

	bool IsInWindow() const noexcept { return isInWindow; }

	std::optional<Event> ReadEvent();
	void FlushEventBuffer();

private:
	void OnMouseMove( POINTS pos ) noexcept;
	
	void OnButtonDown( UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;
	void OnButtonUp( UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;

	void OnScroll( short delta, POINTS pos ) noexcept;

	void OnMouseLeave() noexcept;
	void OnMouseEnter() noexcept;

	static constexpr unsigned int bufferSize = 16u;
	static constexpr unsigned int nButtons = 5u;

	std::bitset<nButtons> buttonStates;
	std::queue<Event> eventBuffer;
	short wheelDeltaCarry = 0;
	bool isInWindow = false;
	POINTS pos;
};