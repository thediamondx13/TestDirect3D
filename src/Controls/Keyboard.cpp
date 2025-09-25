#include <Controls/Keyboard.h>

std::optional<Keyboard::Event> Keyboard::ReadKey()
{
    if ( _keyBuffer.empty() ) return std::nullopt;

    Keyboard::Event e = _keyBuffer.front();
    _keyBuffer.pop();
    return e;
}

void Keyboard::OnKeyDown( unsigned char key )
{
    _keyStates.set( key );
    _keyBuffer.push( Event( Event::K_DOWN, key ) );
    if ( _keyBuffer.size() > S_BUFFER_SIZE ) _keyBuffer.pop();
}

void Keyboard::OnKeyUp( unsigned char key )
{
    _keyStates.reset( key );
    _keyBuffer.push( Event( Event::K_UP, key ) );
    if ( _keyBuffer.size() > S_BUFFER_SIZE ) _keyBuffer.pop();
}