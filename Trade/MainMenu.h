#pragma once
#include <Windows.h>
#include <CommCtrl.h>

namespace MainMenu {

	void Initialize(HWND mainWnd);
	void DrawTradeBTN(LPDRAWITEMSTRUCT DrItStruct);
}