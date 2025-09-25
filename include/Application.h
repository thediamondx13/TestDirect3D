#pragma once

// standard includes
#include <chrono>

// project includes
#include <Window.h>

#include <Drawable/Cube.h>
#include <Drawable/Sphere.h>

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

    Window _window;
    Timer _timer;

    std::vector<std::unique_ptr<DrawableBase>> _cubes;

    std::unique_ptr<Sphere> _sphere;
};