#include <Application.h>

Application::Application() : window( L"Direct3D test app" ) {}

int Application::Run()
{
	while ( true )
	{
		if ( const auto ecode = Window::ProcessMessages() ) return *ecode;
		RenderFrame();
	}
}

void Application::RenderFrame()
{	
	// TODO
}

