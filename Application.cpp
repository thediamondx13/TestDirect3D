#include <Windows.h>
#include "Window.h"

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	try {
		Window window( L"Direct3D test app" );
		BOOL gResult;
		MSG msg;

		while( (gResult = GetMessage( &msg, nullptr, 0, 0 )) > 0 )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		};

		return (gResult == 0) ? msg.wParam : -1;
	}
	catch( ... )
	{
		return -1;
	}
	return -1;
}