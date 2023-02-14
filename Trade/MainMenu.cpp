#include "MainMenu.h"
#include "resource.h"

void MainMenu::Initialize(HWND mainWnd)
{
	RECT rc;
	GetWindowRect(mainWnd, &rc);

	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;



	HWND btn_trade = CreateWindow(
		WC_BUTTON,
		L"Close Application",
		BS_OWNERDRAW | WS_CHILD | WS_VISIBLE,
		int(0.02 * width), int(0.1 * height), int(0.45 * width), int(0.7 * height),
		mainWnd, (HMENU)IDB_MAIN_TRADE, 0, 0
	);

	HWND btn_base = CreateWindow(
		WC_BUTTON,
		L"Close Application",
		BS_OWNERDRAW | WS_CHILD | WS_VISIBLE,
		int(0.5 * width), int(0.1 * height), int(0.43 * width), int(0.7 * height),
		mainWnd, (HMENU)IDB_MAIN_BASE, 0, 0
	);
	UpdateWindow(mainWnd);
}

void MainMenu::DrawTradeBTN(LPDRAWITEMSTRUCT DrItStruct)
{
	HBRUSH backgroundColor =  CreateSolidBrush(RGB(0, 255, 0));
	SelectObject(DrItStruct->hDC, backgroundColor);
	Rectangle(DrItStruct->hDC, 
		DrItStruct->rcItem.left, DrItStruct->rcItem.top,
		DrItStruct->rcItem.right, DrItStruct->rcItem.bottom
	);

	DeleteObject(backgroundColor);


	HMODULE imgLoadDLL = LoadLibrary(L"ImageLoader.dll");
	
	if (imgLoadDLL == NULL)
		return;
	typedef unsigned char* (*IMAGE_LOAD)(const char* path);
	
	IMAGE_LOAD imgLoad = (IMAGE_LOAD)GetProcAddress(imgLoadDLL, "LoadPNG");
	unsigned char* result = imgLoad("trade machine.png");

	delete result;
	FreeLibrary(imgLoadDLL);
}
