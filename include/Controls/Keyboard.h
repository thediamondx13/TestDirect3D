#pragma once

// standard includes
#include <optional>
#include <bitset>
#include <queue>

class Keyboard
{
	friend class Window;

public:
	class Event
	{
	public:
		enum Type { K_DOWN, K_UP };

		Event( Type type, unsigned char key ) : type( type ), key( key ) {}
		unsigned char key;
		Type type;
	};

	Keyboard() = default;

	std::optional<Event> ReadKey();
	void FlushEventBuffer() { _keyBuffer = std::queue<Event>(); }
	bool IsKeyDown( unsigned char key ) const { return _keyStates.test( key ); }
	void ResetKey( unsigned char key ) { _keyStates.reset( key ); }
	void SetKey( unsigned char key ) { _keyStates.set( key ); }

private:
	void ClearStates() { _keyStates.reset(); }
	void OnKeyDown( unsigned char key );
	void OnKeyUp( unsigned char key );

	static constexpr unsigned int S_BUFFER_SIZE = 16u;
	static constexpr unsigned int S_N_KEYS = 256u;
	//bool useAutorepeat = false;

	std::bitset<S_N_KEYS> _keyStates;
	std::queue<Event> _keyBuffer;
};