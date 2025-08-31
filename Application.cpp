#include <windows.h>

constexpr WCHAR wndClassName[] = L"MainWindowClass";

LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_CLOSE:
		PostQuitMessage( 0 );
		break;
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}

void RegisterNewClass(const HINSTANCE instance, const WCHAR* className)
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
	
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = className;
	wc.hInstance = instance;
	wc.cbSize = sizeof(wc);

	RegisterClassEx(&wc);
}

HWND CreateNewWindow(const HINSTANCE instance, const WCHAR* className) 
{
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);

	int nHeight = 600;
	int nWidth = 800;
	
	int y = (screenHeight - nHeight) / 2;
	int x = (screenWidth - nWidth) / 2;

	return CreateWindowEx(
		0, className,
		L"Direct3D application",
		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		x, y, nWidth, nHeight,
		nullptr, nullptr, instance, nullptr
	);
}

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	RegisterNewClass(hInstance, wndClassName);
	HWND hwnd = CreateNewWindow(hInstance, wndClassName);
	// TODO : add error handling

	ShowWindow(hwnd, SW_SHOW);

	MSG msg;
	BOOL gResult;

	while( (gResult = GetMessage( &msg, nullptr, 0, 0 )) > 0 )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	};

	return (gResult == 0) ? msg.wParam : -1;
}