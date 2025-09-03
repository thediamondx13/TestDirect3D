#pragma once

// standard includes
#include <chrono>

// project includes
#include <Window.h>

using namespace std::chrono;

class Application
{
public:
	Application();
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	int Run();

private:
    class Timer 
    {
    public:
        Timer() : last(std::chrono::steady_clock::now()) {}
        double time() const { return duration<double>(steady_clock::now() - last).count(); }
        void reset() { last = steady_clock::now(); }

    private:
        steady_clock::time_point last;
    };

	void RenderFrame();

	Window window;
	Timer timer;
};