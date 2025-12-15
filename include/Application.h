#pragma once

// standard includes
#include <chrono>

// project includes
#include <Window.h>
#include <Universe.h>

using namespace std::chrono;

class Application
{
public:
    Application();
    int Run();

protected:
    class Timer
    {
    public:
        Timer() : last( std::chrono::steady_clock::now() ) {}

        float delta()
        {
            float delta = duration<float>( steady_clock::now() - last ).count();
            last = steady_clock::now();
            return delta;
        }

        float time() const
        {
            return duration<float>( steady_clock::now() - last ).count();
        }

        void reset()
        {
            last = steady_clock::now();
        }

    protected:
        steady_clock::time_point last;
    };

    void ProcessMouse();
    void ProcessKeyboard( const float dt );

    void RenderFrame( float dt );

    Timer _timer;
    Window _window;

    // all physic props
    Universe _universe;
    bool _paused = true;
};