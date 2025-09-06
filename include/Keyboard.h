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
		bool IsDown() const { return type == Type::K_DOWN; }

	private:
		Type type;
		unsigned char key;
	};

	Keyboard() = default;
	Keyboard( const Keyboard& ) = delete;
	Keyboard& operator=( const Keyboard& ) = delete;

	std::optional<Event> ReadKey();
	void FlushEventBuffer() { keyBuffer = std::queue<Event>(); }
	bool IsKeyDown( unsigned char key ) const { return keyStates.test( key ); }

private:
	void ClearStates() { keyStates.reset(); }
	void OnKeyDown( unsigned char key );
	void OnKeyUp( unsigned char key );

	static constexpr unsigned int bufferSize = 16u;
	static constexpr unsigned int nKeys = 256u;
	//bool useAutorepeat = false;

	std::bitset<nKeys> keyStates;
	std::queue<Event> keyBuffer;
};

