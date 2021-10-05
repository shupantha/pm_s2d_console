#include "stdafx.h"
#include "Console.h"

#include <tchar.h>

#include <cstdlib>

using namespace std;

void CConsole::SetTitle(const TCHAR* szTitle)
{
	SetConsoleTitle(szTitle);
}

TCHAR* CConsole::GetTitle()
{
	// get the title of our console and return it
	static TCHAR szTitle[256] = _T("");
	GetConsoleTitle(szTitle, 256);

	return szTitle;
}

HWND CConsole::GetHWND()
{
	// try to find our console window and return its HWND
	return FindWindow(_T("ConsoleWindowClass"), GetTitle());
}

void CConsole::Show(bool bShow)
{
	HWND hWnd = GetHWND();

	if(hWnd != NULL)
	{
		ShowWindow(hWnd, bShow ? SW_SHOW : SW_HIDE);
	}
}

void CConsole::DisableClose()
{
	HWND hWnd = GetHWND();
	
	// disable the [x] button if we found our console
	if(hWnd != NULL)
	{
		HMENU hMenu = GetSystemMenu(hWnd, 0);

		if(hMenu != NULL)
		{
			DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
			DrawMenuBar(hWnd);
		}
	}
}

void CConsole::Clear()
{
	system("CLS");
}

void CConsole::Close()
{
	FreeConsole();
}
