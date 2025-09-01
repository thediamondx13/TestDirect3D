#include <Keyboard.h>

std::optional<Keyboard::Event> Keyboard::ReadKey() noexcept
{
    if (keyBuffer.empty()) return std::nullopt;

    Keyboard::Event e = keyBuffer.front();
    keyBuffer.pop();
    return e;
}

void Keyboard::OnKeyDown( unsigned char key ) noexcept
{
    keyStates[key] = true;
    keyBuffer.push( Event( Event::K_DOWN, key ) );
    if ( keyBuffer.size() > bufferSize ) keyBuffer.pop();
}

void Keyboard::OnKeyUp( unsigned char key ) noexcept
{
    keyStates[key] = false;
    keyBuffer.push( Event( Event::K_UP, key ) );
    if ( keyBuffer.size() > bufferSize ) keyBuffer.pop();
}
