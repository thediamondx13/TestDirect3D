#include <Application.h>

static std::wstring ToWideString(const char* narrowStr) {
	int len = MultiByteToWideChar(CP_UTF8, 0, narrowStr, -1, nullptr, 0);
	std::wstring wideStr(len, 0);
	MultiByteToWideChar(CP_UTF8, 0, narrowStr, -1, &wideStr[0], len);
	return wideStr;
}

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	try { return Application{}.Run(); }
	catch (const std::exception& e)
	{

		// TODO: save error logs to a file

		std::wstring wmsg = ToWideString(e.what());
		MessageBox(nullptr, wmsg.c_str(), L"Critical failure!", MB_OK | MB_ICONERROR);
	}

	return -1;
}