#pragma once

// standard includes
#include <Windows.h>
#include <optional>

// graphics includes
#include <Controls/Keyboard.h>
#include <Controls/Mouse.h>
#include <DXDevice.h>

class Window
{
public:
	Window( LONG width, LONG height, bool windowed, const LPCWSTR caption );
	~Window() { DestroyWindow( _hWnd ); }

	static std::optional<int> ProcessMessages();

	DXDevice &GetGfxDevice() { return *_pGfx.get(); }
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
		~WindowClass();

		static constexpr DWORD s_wndStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
		static constexpr LPCWSTR s_wndClassName = L"MainWindowClass";
		static WindowClass s_wndClass;
		HINSTANCE _hInstance;
	};

	static LRESULT CALLBACK HandleStartupMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	static LRESULT CALLBACK HandleRuntimeMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	LRESULT HandleMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	// graphics object pointer
	std::unique_ptr<DXDevice> _pGfx;

	// client and screen centers
	POINT _cCenter, _sCenter;

	// other window stuff
	LONG _width, _height;
	LPCWSTR _caption;
	bool _windowed;
	DWORD _style;
	RECT _wRect;
	HWND _hWnd;
};