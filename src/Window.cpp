#include <Window.h>

// ---------- WINDOW CLASS DEFINITION ----------

Window::WindowClass Window::WindowClass::wndClass;
constexpr LPCWSTR Window::WindowClass::wndClassName;

Window::WindowClass::WindowClass() noexcept : hInstance( GetModuleHandle( nullptr ) )
{
	WNDCLASSEX wc = { 0 };

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
	wc.lpszClassName = wndClassName;
	wc.hInstance = hInstance;
	wc.cbSize = sizeof( wc );

	RegisterClassEx( &wc );
}

// ---------- WINDOW DEFINITION ----------

constexpr int defWidth = 1280;
constexpr int defHeight = 720;

Window::Window( const LPCWSTR name ) noexcept : 
	width( defWidth ), height( defHeight ),
	style( WindowClass::GetStyle() )
{
	RECT wRect {};
	wRect.left = 100;
	wRect.top = 100;
	wRect.right = width + wRect.left;
	wRect.bottom = height + wRect.top;
	AdjustWindowRect( &wRect, style, FALSE );

	this->caption = name;

	hWnd = CreateWindow(WindowClass::GetName(), 
		caption, style, CW_USEDEFAULT, CW_USEDEFAULT, 
		wRect.right - wRect.left, wRect.bottom - wRect.top,
		nullptr, nullptr, WindowClass::GetInstance(), this);

	ShowWindow( hWnd, SW_SHOW );
}

LRESULT Window::HandleStartupMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept
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

LRESULT Window::HandleRuntimeMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept
{
	// retrieve pointer to window instance	
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr( hWnd, GWLP_USERDATA ));

	// forward message to window instance handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept
{
	switch( msg )
	{
	case WM_CLOSE:
		PostQuitMessage( 0 );
		return 0;

	case WM_KEYDOWN:
		if( lParam & 0x40000000 ) break; // filter autorepeat
		keyboard.OnKeyDown( static_cast<unsigned char>(wParam) );
		break;
	
	case WM_KEYUP:
		keyboard.OnKeyUp( static_cast<unsigned char>(wParam) );
		break;
	}

	// inherit default message handling
	return DefWindowProc( hWnd, msg, wParam, lParam );
}