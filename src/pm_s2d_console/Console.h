#pragma once
#include <windows.h>

class CConsole
{
public:
	// set and get title of console
	void SetTitle(const TCHAR* szTitle);
	TCHAR* GetTitle();

	// get HWND and/or HANDLE of console
	HWND GetHWND();

	// show/hide the console
	void Show(bool bShow = true);

	// disable the [x] button of the console
	void DisableClose();

	// clear the screen
	void Clear();

	// close the console
	void Close();
};
