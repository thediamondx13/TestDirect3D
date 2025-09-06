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
	window.GetGfxDevice().ClearBuffer( 0.3f, 0.1f, 0.1f );
	window.GetGfxDevice().DrawPrimitive(window.mouse.GetPos());
	window.GetGfxDevice().SwapBuffers();
}

