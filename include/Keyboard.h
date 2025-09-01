#pragma once

#include <queue>
#include <bitset>
#include <optional>

class Keyboard
{
	friend class Window;

public:
	class Event
	{
	public:
		enum Type { K_DOWN, K_UP };

		Event( Type type, unsigned char key ) noexcept : type( type ), key( key ) {}
		bool IsDown() const noexcept { return type == Type::K_DOWN; }

	private:
		Type type;
		unsigned char key;
	};

	Keyboard() = default;
	Keyboard( const Keyboard& ) = delete;
	Keyboard& operator=( const Keyboard& ) = delete;

	std::optional<Event> ReadKey() noexcept;
	void FlushEventBuffer() noexcept { keyBuffer = std::queue<Event>(); }
	bool IsKeyDown( unsigned char key ) const noexcept { return keyStates.test( key ); }

private:
	void ClearStates() noexcept { keyStates.reset(); }
	void OnKeyDown( unsigned char key ) noexcept;
	void OnKeyUp( unsigned char key ) noexcept;

	static constexpr unsigned int bufferSize = 16u;
	static constexpr unsigned int nKeys = 256u;
	//bool useAutorepeat = false;

	std::bitset<nKeys> keyStates;
	std::queue<Event> keyBuffer;
};

