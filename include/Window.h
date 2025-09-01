#pragma once

#include <Windows.h>
#include <Keyboard.h>

class Window
{
public:
	Window( const LPCWSTR caption ) noexcept;
	~Window() { DestroyWindow( hWnd ); }

	// explicitly delete copy constructor
	Window( const Window& ) = delete;
	
	// explicitly delete assignment operator
	Window& operator=( const Window& ) = delete;

	Keyboard keyboard;

private:
	class WindowClass
	{
	public:
		static HINSTANCE GetInstance() noexcept { return wndClass.hInstance; }
		static LPCWSTR GetName() noexcept { return wndClassName; }
		static DWORD GetStyle() noexcept { return wndStyle; }
	private:
		WindowClass() noexcept;
		~WindowClass() { UnregisterClass( wndClassName, hInstance ); }
		
		// explicitly delete copy constructor
		WindowClass( const WindowClass& ) = delete;

	    // explicitly delete assignment operator
		WindowClass& operator=( const WindowClass& ) = delete;
		
		static constexpr DWORD wndStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
		static constexpr LPCWSTR wndClassName = L"MainWindowClass";
		static WindowClass wndClass;
		HINSTANCE hInstance;
	};

	static LRESULT CALLBACK HandleStartupMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;
	static LRESULT CALLBACK HandleRuntimeMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;
	LRESULT HandleMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;

	int width;
	int height;
	LPCWSTR caption;
	DWORD style;
	HWND hWnd;
};