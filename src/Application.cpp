#include <Application.h>

Application::Application() : _window( 1280, 720, L"Direct3D test app" ) {}

int Application::Run()
{
	constexpr int size = 1;

	_planets.resize( size );
	for ( int i = 0; i < size; i++ )
	{
		_planets[i] = std::make_unique<Planet>( _window.GetGfxDevice() );
		
		/*for ( auto &pPlanet : _planets )
		{
			if ( pPlanet == nullptr ) break;
			pPlanet->Update( 7.0f );
		}*/
	}
	
	_bh = std::make_unique<BlackHole>( _window.GetGfxDevice(), _window.GetGfxDevice().camera );

	while ( true )
	{
		if ( const auto ecode = Window::ProcessMessages() ) return *ecode;

		const float dt = _timer.delta();
		
		ProcessMouse();
		ProcessKeyboard( dt );
		RenderFrame( dt );
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
	_window.GetGfxDevice().FillBuffer( 0.3f, 0.1f, 0.1f );		

	for ( auto &pPlanet : _planets )
	{
		if ( pPlanet == nullptr ) break;
		pPlanet->Draw( _window.GetGfxDevice() );
		//pPlanet->Update( dt );
	}

	_bh->Draw( _window.GetGfxDevice() );

	_window.GetGfxDevice().SwapBuffers();
}