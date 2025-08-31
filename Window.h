#include <Windows.h>

class Window
{
public:
	Window( const LPCWSTR caption );
	~Window();

	// explicitly delete copy constructor
	Window( const Window& ) = delete;
	
	// explicitly delete assignment operator
	Window& operator=( const Window& ) = delete;

private:
	class WindowClass
	{
	public:
		static HINSTANCE GetInstance();
		static LPCWSTR GetName();
		static DWORD GetStyle();
	private:
		WindowClass();
		~WindowClass();	
		
		// explicitly delete copy constructor
		WindowClass( const WindowClass& ) = delete;

	    // explicitly delete assignment operator
		WindowClass& operator=( const WindowClass& ) = delete;
		
		static constexpr DWORD wndStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
		static constexpr WCHAR wndClassName[] = L"MainWindowClass";
		static WindowClass wndClass;
		HINSTANCE hInstance;
	};

	static LRESULT CALLBACK HandleStartupMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	static LRESULT CALLBACK HandleRuntimeMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	LRESULT HandleMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	int width;
	int height;
	LPCWSTR caption;
	DWORD style;
	HWND hWnd;
};