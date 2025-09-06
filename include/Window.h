#pragma once

// standard includes
#include <Windows.h>
#include <optional>
#include <memory>

// project includes
#include <Keyboard.h>
#include <Mouse.h>

// graphics includes
#include <DXDevice.h>

class Window
{
public:
	Window( const LPCWSTR caption );
	Window( const Window& ) = delete;
	Window& operator=( const Window& ) = delete;
	~Window() { DestroyWindow( hWnd ); }

	static std::optional<int> ProcessMessages();

	DXDevice& GetGfxDevice() { return *pGfx; }

	Keyboard keyboard;
	Mouse mouse;

private:
	class WindowClass
	{
	public:
		static HINSTANCE GetInstance() { return wndClass.hInstance; }
		static LPCWSTR GetName() { return wndClassName; }
		static DWORD GetStyle() { return wndStyle; }
	private:
		WindowClass();
		WindowClass( const WindowClass& ) = delete;
		WindowClass& operator=( const WindowClass& ) = delete;
		~WindowClass() { UnregisterClass( wndClassName, hInstance ); }
		
		static constexpr DWORD wndStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
		static constexpr LPCWSTR wndClassName = L"MainWindowClass";
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

	std::unique_ptr<DXDevice> pGfx;
};