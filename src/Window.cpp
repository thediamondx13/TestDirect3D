#include <Window.h>

// ---------- WINDOW CLASS DEFINITION ----------

Window::WindowClass Window::WindowClass::s_wndClass;
constexpr LPCWSTR Window::WindowClass::s_wndClassName;

Window::WindowClass::WindowClass() : _hInstance( GetModuleHandle( nullptr ) )
{
	WNDCLASSEX wc{};

	// style definitions
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.hCursor = nullptr;
	wc.hIconSm = nullptr;
	wc.style = CS_OWNDC;
	wc.hIcon = nullptr;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	// end style definitions

	wc.lpfnWndProc = HandleStartupMsg;
	wc.lpszClassName = s_wndClassName;
	wc.hInstance = _hInstance;
	wc.cbSize = sizeof( wc );

	RegisterClassEx( &wc );
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass( s_wndClassName, _hInstance );
}

// ---------- WINDOW DEFINITION ----------

Window::Window( LONG width, LONG height, const LPCWSTR name ) : _style( WindowClass::GetStyle() ),
	_cCenter( width >> 1, height >> 1 ), _sCenter( width >> 1, height >> 1 ),
	_caption( name ), _width( width ), _height( height )
{
	// center the window rect
	_wRect.top = (GetSystemMetrics( SM_CYSCREEN ) - height) >> 1;
	_wRect.left = (GetSystemMetrics( SM_CXSCREEN ) - width) >> 1;
	_wRect.bottom = _wRect.top + height;
	_wRect.right = _wRect.left + width;
	
	// adjust window's width and height	
	AdjustWindowRect( &_wRect, _style, FALSE );	
	height = _wRect.bottom - _wRect.top;
	width = _wRect.right -= _wRect.left;

	// create and display the window
	_hWnd = CreateWindow( WindowClass::GetName(), _caption, _style,
		_wRect.left, _wRect.top, width, height, nullptr, nullptr,
		WindowClass::GetInstance(), this
	);
	ShowWindow( _hWnd, SW_SHOW );	

	// construct graphical device
	_pGfx = std::make_unique<DXDevice>( _hWnd, _width, _height );
}

std::optional<int> Window::ProcessMessages()
{
	MSG msg;
	while ( PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) )
	{	
		if (msg.message == WM_QUIT) return msg.wParam;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	};

	return std::nullopt;
}

LRESULT Window::HandleStartupMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if( msg == WM_CREATE )
	{
		// extract pointer to window instance from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

		// set WinAPI-managed user data to store pointer to window instance
		SetWindowLongPtr( hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd) );
		
		// change message handler to runtime handler after setting up
		SetWindowLongPtr( hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleRuntimeMsg) );

		// forward message to window instance handler
		return pWnd->HandleMsg( hWnd, msg, wParam, lParam );
	}

	// use defalut message handler for unhandled messages
	return DefWindowProc( hWnd, msg, wParam, lParam );
}

LRESULT Window::HandleRuntimeMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	// retrieve pointer to window instance	
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr( hWnd, GWLP_USERDATA ));

	// forward message to window instance handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_CLOSE:
		PostQuitMessage( 0 );
		return 0;

	// keyboard messages
	case WM_KEYDOWN:
		if( lParam & 0x40000000 ) break; // filter autorepeat
		if ( wParam == VK_ESCAPE ) mouse.Release();
		keyboard.OnKeyDown( static_cast<unsigned char>(wParam) );
		break;
	
	case WM_KEYUP:
		keyboard.OnKeyUp( static_cast<unsigned char>(wParam) );
		break;

	// set default cursor
	case WM_SETCURSOR:
		if( LOWORD(lParam) == HTCLIENT )
		{
			SetCursor( LoadCursor( nullptr, IDC_ARROW ) );
			return TRUE;
		}
		break;

	// window move
	case WM_MOVE:
		_sCenter = _cCenter;
		ClientToScreen( _hWnd, &_sCenter );		
		break;

	// mouse movement
	case WM_MOUSEMOVE:
	{
		if ( mouse.IsCaptured() )
		{
			POINTS pos = MAKEPOINTS( lParam );
			mouse.OnMouseMove( pos );

			// move mouse back to center
			mouse.SetPosition( _cCenter );
			SetCursorPos( _sCenter.x, _sCenter.y );
		}
		break;
	}
	
	// mouse button down
	case WM_LBUTTONDOWN: 
		if ( !mouse.IsCaptured() ) {
			// capture the mouse's movements
			mouse.Capture( _hWnd );

			/* set the position to the center
			 so the camera won't flick */
			mouse.SetPosition( _cCenter );
			SetCursorPos( _sCenter.x, _sCenter.y );
		}		
		[[fallthrough]]; //fall to process LKM with m.OnButtonDown 
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_XBUTTONDOWN:
		mouse.OnButtonDown( msg, wParam, lParam );
		break;
	
	// mouse button up
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	case WM_XBUTTONUP:
		mouse.OnButtonUp( msg, wParam, lParam );
		break;

	// mouse wheel
	case WM_MOUSEWHEEL:
		short delta = GET_WHEEL_DELTA_WPARAM( wParam );
		POINTS pos = MAKEPOINTS( lParam );
		mouse.OnScroll( delta, pos );
		break;
	}

	// inherit default message handling
	return DefWindowProc( hWnd, msg, wParam, lParam );
}