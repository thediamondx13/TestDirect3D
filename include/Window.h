#pragma once

// standard includes
#include <Windows.h>
#include <optional>
#include <memory>

// graphics includes
#include <Controls/Keyboard.h>
#include <Controls/Mouse.h>
#include <DXDevice.h>

class Window
{
public:
	Window( LONG width, LONG height, const LPCWSTR caption );
	Window& operator=( const Window& ) = delete;
	Window( const Window& ) = delete;

	~Window() { DestroyWindow( _hWnd ); }

	static std::optional<int> ProcessMessages();

	DXDevice& GetGfxDevice() { return *_pGfx; }
	LONG GetHeight() const { return _height; }
	LONG GetWidth() const { return _width; }

	Keyboard keyboard;
	Mouse mouse;

private:
	class WindowClass
	{
	public:
		static HINSTANCE GetInstance() { return s_wndClass._hInstance; }
		static LPCWSTR GetName() { return s_wndClassName; }
		static DWORD GetStyle() { return s_wndStyle; }
	private:
		WindowClass();
		WindowClass( const WindowClass& ) = delete;
		WindowClass& operator=( const WindowClass& ) = delete;
		~WindowClass() { UnregisterClass( s_wndClassName, _hInstance ); }
		
		static constexpr DWORD s_wndStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
		static constexpr LPCWSTR s_wndClassName = L"MainWindowClass";
		static WindowClass s_wndClass;
		HINSTANCE _hInstance;
	};

	static LRESULT CALLBACK HandleStartupMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	static LRESULT CALLBACK HandleRuntimeMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	LRESULT HandleMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	std::unique_ptr<DXDevice> _pGfx;
	
	// client and screen centers
	POINT _cCenter, _sCenter;
	LONG _width, _height;
	LPCWSTR _caption;
	DWORD _style;
	RECT _wRect;
	HWND _hWnd;	
};