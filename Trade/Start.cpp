#include "resource.h"
#include <Windows.h>
#include <CommCtrl.h>

#include "MainMenu.h"

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#pragma comment(lib, "comctl32.lib")

HWND mainWnd;

LRESULT CALLBACK Handler(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{
	case WM_COMMAND: {
		int mID = LOWORD(wParam);
		if (wParam == IDB_MAIN_TRADE)
			DestroyWindow(mainWnd);
	}break;
	case WM_DRAWITEM: {
		switch (wParam)
		{
		case IDB_MAIN_TRADE: {
			LPDRAWITEMSTRUCT drawItem = (LPDRAWITEMSTRUCT)lParam;
			MainMenu::DrawTradeBTN(drawItem);
		}break;
		default:
			break;
		}
	}break;
	case WM_DESTROY: {
		PostQuitMessage(0);
	}break;
	default:
		return DefWindowProc(wnd, msg, wParam, lParam);	
	}
}

int main(int s, char**d) {
	
	WNDCLASSEX wndClass;
	HMODULE hInstance = GetModuleHandle(NULL);

	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = Handler;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

	

	wndClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wndClass.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_ARROW));

	HBRUSH background = CreateSolidBrush(RGB(209, 213, 136));
	wndClass.hbrBackground = background;
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = L"TRADER";
	
	RegisterClassEx(&wndClass);

	mainWnd = CreateWindowEx(
		0,
		wndClass.lpszClassName,
		L" ¿——Œ¬€… ¿œœ¿–¿“",
	    WS_MINIMIZEBOX | WS_SYSMENU | WS_MAXIMIZE,
		0, 0, 700, 400,
		0, 0, hInstance, 0
	);
	if (mainWnd == NULL) 
		return 1;

	ShowWindow(mainWnd, SW_MAXIMIZE);
	SetActiveWindow(mainWnd);
	
	MSG msg;

	MainMenu::Initialize(mainWnd);
	

	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}