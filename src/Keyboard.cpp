#include <Keyboard.h>

std::optional<Keyboard::Event> Keyboard::ReadKey()
{
    if (keyBuffer.empty()) return std::nullopt;

    Keyboard::Event e = keyBuffer.front();
    keyBuffer.pop();
    return e;
}

void Keyboard::OnKeyDown( unsigned char key )
{
    keyStates.set( key );
    keyBuffer.push( Event( Event::K_DOWN, key ) );
    if ( keyBuffer.size() > bufferSize ) keyBuffer.pop();
}

void Keyboard::OnKeyUp( unsigned char key )
{
    keyStates.reset( key );
    keyBuffer.push( Event( Event::K_UP, key ) );
    if ( keyBuffer.size() > bufferSize ) keyBuffer.pop();
}
