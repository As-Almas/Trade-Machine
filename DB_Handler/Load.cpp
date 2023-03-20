#include "pch.h"
#include <Windows.h>


HANDLE hFile;

bool LoadDBData(char* path, char key) {
	
	hFile = CreateFileA(
		path,
		GENERIC_READ | GENERIC_WRITE,
		0,
		0,
		OPEN_ALWAYS | CREATE_ALWAYS,
		0, 0
	);

	if (hFile == INVALID_HANDLE_VALUE) {
		return false;
	}

	return true;
}


void CloseDB() {
	CloseHandle(hFile);
}