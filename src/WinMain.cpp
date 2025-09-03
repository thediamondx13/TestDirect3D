#include <Application.h>

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	try { return Application{}.Run(); }
	/*catch (custom exception e) 
	{
		
	}*/ 
	catch (const std::exception& e)
	{
		int len = MultiByteToWideChar(CP_UTF8, 0, e.what(), -1, nullptr, 0);
		std::wstring wmsg(len, L'\0');
		MultiByteToWideChar(CP_UTF8, 0, e.what(), -1, &wmsg[0], len);

		MessageBox(nullptr, wmsg.c_str(), L"Standard exception", MB_OK | MB_ICONERROR);
	}

	return -1;
}