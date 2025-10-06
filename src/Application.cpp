#include <Application.h>

Application::Application() : _window( 800, 600, true, L"Direct3D test app" ),
	_universe( _window.GetGfxDevice() )
{
	//
}

int Application::Run()
{
	while ( true )
	{
		if ( const auto ecode = Window::ProcessMessages() ) return *ecode;

		const float dt = _timer.delta();
		
		ProcessMouse();
		ProcessKeyboard( dt );
		
		RenderFrame( dt );

		_universe.Update( dt );
	}
}

void Application::ProcessMouse()
{
	std::optional<Mouse::Event> e;
	while ( (e = _window.mouse.ReadEvent()).has_value() )
	{
		switch ( e->type )
		{
		case Mouse::Event::NEW_POS:
			const Mouse::Pos delta = e->delta.value();

			/* Vertical component must be inverted,
			 because of different coord systems */
			_window.GetGfxDevice().camera.ProcessMouseDelta(
				-2.0f * delta.y / _window.GetHeight(),
				+2.0f * delta.x / _window.GetWidth()
			);
			break;
		}
	}
}

void Application::ProcessKeyboard( const float dt )
{
	// move the camera around
	if ( _window.keyboard.IsKeyDown( 'W' ) )
		_window.GetGfxDevice().camera.MoveForward( +dt );
	else if ( _window.keyboard.IsKeyDown( 'S' ) )
		_window.GetGfxDevice().camera.MoveForward( -dt );

	if ( _window.keyboard.IsKeyDown( 'A' ) )
		_window.GetGfxDevice().camera.MoveSideways( -dt );
	else if ( _window.keyboard.IsKeyDown( 'D' ) )
		_window.GetGfxDevice().camera.MoveSideways( +dt );

	if ( _window.keyboard.IsKeyDown( VK_SPACE ) )
		_window.GetGfxDevice().camera.MoveVertical( +dt );
	else if ( _window.keyboard.IsKeyDown( VK_CONTROL ) )
		_window.GetGfxDevice().camera.MoveVertical( -dt );

	std::optional<Keyboard::Event> e;
	while ( (e = _window.keyboard.ReadKey()).has_value() )
	{
		if ( e->type == Keyboard::Event::K_DOWN )
		{
			switch ( e->key )
			{
			case 'W':
				_window.keyboard.ResetKey( 'S' );
				break;
			case 'A':
				_window.keyboard.ResetKey( 'D' );
				break;
			case 'S':
				_window.keyboard.ResetKey( 'W' );
				break;
			case 'D':
				_window.keyboard.ResetKey( 'A' );
				break;

			case VK_RETURN:
				_rayTracingEnabled = !_rayTracingEnabled;
			}
		}
		/*else
		{
			switch ( e->key )
			{
			
			}
		}*/
	}
}

void Application::RenderFrame( float dt )
{
	const DXDevice &gfx = _window.GetGfxDevice();
	gfx.FillBuffer( 0.3f, 0.1f, 0.1f );
	_universe.Draw( gfx );
	gfx.SwapBuffers();
}