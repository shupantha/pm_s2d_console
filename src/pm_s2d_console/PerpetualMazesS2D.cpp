#include "stdafx.h"
#include "PerpetualMazesS2D.h"

#include "Util.h"

#include "GameWorld.h"

#include "GfxRunner.h"
#include "GfxMonster.h"
#include "GfxDoor.h"
#include "GfxPill.h"

#include "Settings.h"

#include <stdio.h>
#include <string.h>

#define SAVE_FILE_NAME				_T("__save.txt")
#define SCREEN_SHOT_NAME			_T("_screenshot")
#define SCREEN_SHOT_EXT				_T(".png")
#define ITERATION_COUNT				50

// settings
CSettings g_settings;

// game variables
CGameWorld g_gw;
int g_iGameDifficulty = 0;

// int g_screenX = GetSystemMetrics(SM_CXFULLSCREEN);
// int g_screenY = GetSystemMetrics(SM_CYFULLSCREEN);

int g_screenX = GetSystemMetrics(SM_CXSCREEN);
int g_screenY = GetSystemMetrics(SM_CYSCREEN) - 1;

// click point
int g_x;
int g_y;

// simple2d
S2D_Window* g_pGameWindow = nullptr;

S2D_Image* g_helpScreen = S2D_CreateImage("./gfx/common/bg.bmp");

// info text
S2D_Text* g_pText = nullptr;

std::string g_strText = "Perpetual Mazes";
std::string g_strKey = "";

bool g_bShowHelp = false;
bool g_bShowInfo = true;
bool g_bShowDebugInfo = false;
bool g_bFirstTime = true;

// time
CTimer g_timerRun;
unsigned int g_uiIterations = 0;
double g_dFPS = 0.0;

void StartGameWorld(unsigned int uiLevel)
{
	if (g_pGameWindow == nullptr && g_bFirstTime)
	{
		g_pGameWindow = S2D_CreateWindow(
			"Perpetual Mazes",			// title of the window
			g_screenX,					// height
			g_screenY,					// width
			UpdateGameWorld,			// callback function pointers (these can be NULL)
			RenderGameWorld,			// callback function pointers (these can be NULL)
			S2D_BORDERLESS | S2D_FULLSCREEN);	// flags: S2D_RESIZABLE, S2D_BORDERLESS, S2D_FULLSCREEN, S2D_HIGHDPI

		if (g_pGameWindow == nullptr)
		{
			return;
		}

		g_pGameWindow->on_key = on_key;
		g_pGameWindow->on_mouse = on_mouse;
	}

	if (g_pGameWindow == nullptr)
	{
		return;
	}

	g_gw.LoadLevel(uiLevel, g_pGameWindow);

	// initialize the game world
	if (!g_gw.IsReady())
	{
		return;
	}

	if (g_bFirstTime)
	{
		g_gw.SetHard();
		g_iGameDifficulty = (int)g_gw.GetDifficulty();
	}
	else
	{
		g_gw.SetDifficulty((CGameWorld::DIFFICULTY)g_iGameDifficulty);
	}
	
	g_gw.SetDemo(false);

	g_gw.InitializeGameWorld();

	// info text load
	if (g_pText == nullptr)
	{
		std::string strFont = "./msc/" + g_settings.textFont;
		g_pText = S2D_CreateText(strFont.c_str(), "", 10);
	}

	g_pText->x = 2;
	g_pText->y = g_pGameWindow->height - BOTTOM_BAR_SIZE;
	g_pText->color.r = g_settings.textColor.r;
	g_pText->color.g = g_settings.textColor.g;
	g_pText->color.b = g_settings.textColor.b;
	g_pText->color.a = g_settings.textColor.a;

	if (g_bFirstTime)
	{
		g_bFirstTime = false;

		S2D_Show(g_pGameWindow);
	}
}

void UpdateGameWorld()
{
	// timers for calculating FPS
	if (g_uiIterations == 0)
	{
		g_timerRun.Start();
	}
	g_uiIterations++;

	// check for end game
	if (!g_gw.IsRunning())
	{
		return;
	}

	// check for completion
	if (g_gw.IsLevelComplete())
	{
		return;
	}

	// check game over
	if (g_gw.IsGameOver())
	{
		return;
	}

	// on help screen do noting else
	if (g_bShowHelp)
	{
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// pause the game
	if (g_gw.IsPaused())
	{
		return;
	}

	// game
	{
		g_gw.MoveAgents();
		g_gw.HandleCollisions();
		g_gw.UpdateStates();

		// set the new targets for the monsters
		g_gw.SetMonsterTarget(GINKY);
		g_gw.SetMonsterTarget(BINKY);
		g_gw.SetMonsterTarget(YINKY);
		g_gw.SetMonsterTarget(RINKY);
	}
	//////////////////////////////////////////////////////////////////////////

	// show info: level, lives, score, difficulty, fps, key press, spawn timer, etc.
	if (g_bShowInfo)
	{
		// update the text info
		// level
		g_strText = "level = ";
		g_strText += CUtil::toStringA((int)g_gw.GetLevel());
		g_strText += ", ";

		// lives
		g_strText += "lives = ";
		g_strText += CUtil::toStringA((int)g_gw.GetLives());
		g_strText += ", ";

		// score
		g_strText += "score = ";
		g_strText += CUtil::toStringA((int)g_gw.GetScore());
		g_strText += ", ";

		// difficulty
		g_strText += g_gw.GetDifficulty((CGameWorld::DIFFICULTY)g_iGameDifficulty);
		g_strText += ", ";

		// fps
		g_strText += "fps = ";
		g_strText += CUtil::toStringA(g_dFPS, 1);

		// key press
		if (!g_strKey.empty())
		{
			g_strText += ", ";
			g_strText += "key = ";
			g_strText += g_strKey;
		}

		// spawn
		if (g_gw.GetSpawnTime() > 0)
		{
			g_strText += ", ";
			g_strText += "spawn = ";

			int n = (RUNNER_SPAWN_TIMEOUT - g_gw.GetSpawnTime()) / 250;

			g_strText += CUtil::toStringA(n);
		}

		// debug information
		if(g_bShowDebugInfo)
		{
			g_strText += "    ";
			g_strText += "    ";
			g_strText += "    ";
			g_strText += "    ";
			g_strText += "    ";
			g_strText += "    ";
			g_strText += "    ";
			g_strText += "    ";
			g_strText += g_gw.GetAgent(RUNNER).GetLog();
		}

		S2D_SetText(g_pText, g_strText.c_str());
	}
}

void RenderGameWorld()
{
	// on help screen do noting else
	if (g_bShowHelp)
	{
		// show help
		CUtil::pause(PAUSE_TIMEOUT);
		ShowHelp();
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// pause the game
	if (g_gw.IsPaused())
	{
		// draw only the maze
		CUtil::pause(PAUSE_TIMEOUT);
		g_gw.Draw(true, true);
		return;
	}
	else
	// draw
	{
		g_gw.Draw(false, g_gw.GetLevel() > NO_EXTRAS);
	}
	//////////////////////////////////////////////////////////////////////////

	if (g_bShowInfo)
	{
		// draw info text
		S2D_DrawText(g_pText);
	}

	// Calculate run time in seconds for 1 update/render cycle
	if (g_uiIterations >= ITERATION_COUNT)
	{
		g_uiIterations = 0;

		g_dFPS = ITERATION_COUNT / g_timerRun.GetElapsedTime();
	}
}

void on_key(S2D_Event e)
{
	g_strKey = e.key;

	// check "e.key" for the key being interacted with
	switch (e.type)
	{
	case S2D_KEY_DOWN:
		// Key was pressed
		if (CUtil::compareNoCase(std::string(e.key), "Up") == 0)
		{
			g_gw.Move(CAgent::NORTH);
		}

		if (CUtil::compareNoCase(std::string(e.key), "Down") == 0)
		{
			g_gw.Move(CAgent::SOUTH);
		}

		if (CUtil::compareNoCase(std::string(e.key), "Left") == 0)
		{
			g_gw.Move(CAgent::WEST);
		}

		if (CUtil::compareNoCase(std::string(e.key), "Right") == 0)
		{
			g_gw.Move(CAgent::EAST);
		}

		if (CUtil::compareNoCase(std::string(e.key), "D") == 0)
		{
			g_gw.ToggleDemoMode();
		}

		if (CUtil::compareNoCase(std::string(e.key), "S") == 0)
		{
			g_gw.ToggleSound();
		}

		if (CUtil::compareNoCase(std::string(e.key), "P") == 0)
		{
			g_gw.TogglePause();
		}

		if (CUtil::compareNoCase(std::string(e.key), "H") == 0)
		{
			g_bShowHelp = !g_bShowHelp;
		}

		if (CUtil::compareNoCase(std::string(e.key), "I") == 0)
		{
			g_bShowInfo = !g_bShowInfo;
		}

		if (CUtil::compareNoCase(std::string(e.key), "C") == 0)
		{
			g_gw.ToggleCheat();
		}

		if (CUtil::compareNoCase(std::string(e.key), "F1") == 0)
		{
			SaveGame();
		}

		if (CUtil::compareNoCase(std::string(e.key), "F2") == 0)
		{
			LoadGame();
		}

		if (CUtil::compareNoCase(std::string(e.key), "F3") == 0)
		{
			ScreenShot();
		}

		if (CUtil::compareNoCase(std::string(e.key), "F7") == 0)
		{
			OpenSaveGameFolder();
		}

		if (CUtil::compareNoCase(std::string(e.key), "F8") == 0)
		{
			OpenSaveGameFile();
		}

		if (CUtil::compareNoCase(std::string(e.key), "F10") == 0)
		{
			g_bShowDebugInfo = !g_bShowDebugInfo;
		}

		if (CUtil::compareNoCase(std::string(e.key), "1") == 0)
		{
			g_gw.SetEasy();
			g_iGameDifficulty = (int)g_gw.GetDifficulty();
		}

		if (CUtil::compareNoCase(std::string(e.key), "2") == 0)
		{
			g_gw.SetMedium();
			g_iGameDifficulty = (int)g_gw.GetDifficulty();
		}

		if (CUtil::compareNoCase(std::string(e.key), "3") == 0)
		{
			g_gw.SetHard();
			g_iGameDifficulty = (int)g_gw.GetDifficulty();
		}

		if (CUtil::compareNoCase(std::string(e.key), "TAB") == 0)
		{
			// start next level
			StartNextLevel();
		}

		break;

	case S2D_KEY_HELD:
		// Key is being held down
		if (CUtil::compareNoCase(std::string(e.key), "Up") == 0)
		{
			g_gw.Move(CAgent::NORTH);
		}

		if (CUtil::compareNoCase(std::string(e.key), "Down") == 0)
		{
			g_gw.Move(CAgent::SOUTH);
		}

		if (CUtil::compareNoCase(std::string(e.key), "Left") == 0)
		{
			g_gw.Move(CAgent::WEST);
		}

		if (CUtil::compareNoCase(std::string(e.key), "Right") == 0)
		{
			g_gw.Move(CAgent::EAST);
		}
		break;

	case S2D_KEY_UP:
		// Key was released
		if (CUtil::compareNoCase(std::string(e.key), "Escape") == 0)
		{
			// stop previously running game
			S2D_Close(g_pGameWindow);
			S2D_FreeWindow(g_pGameWindow);

			exit(0);
		}
		break;
	}
}

// "e.button" can be one of:
//   S2D_MOUSE_LEFT
//   S2D_MOUSE_MIDDLE
//   S2D_MOUSE_RIGHT
//   S2D_MOUSE_X1
//   S2D_MOUSE_X2

void on_mouse(S2D_Event e)
{
	switch (e.type)
	{
	case S2D_MOUSE_DOWN:
		// Mouse button was pressed
		// Use "e.button" to see what button was clicked
		// Check "e.dblclick" to see if was a double click
		if (e.button == S2D_MOUSE_LEFT)
		{
			Cheat();
		}
		break;

	case S2D_MOUSE_UP:
		// Mouse button was released
		// Use "e.button" to see what button was clicked
		// Check "e.dblclick" to see if was a double click
		break;

	case S2D_MOUSE_SCROLL:
		// Mouse was scrolled
		// Check "e.direction" for direction being scrolled, normal or inverted:
		//   S2D_MOUSE_SCROLL_NORMAL
		//   S2D_MOUSE_SCROLL_INVERTED
		// Check "e.delta_x" and "e.delta_y" for the difference in x and y position
		break;

	case S2D_MOUSE_MOVE:
		// Mouse was moved
		// Check "e.delta_x" and "e.delta_y" for the difference in x and y position
		break;
	}
}
////////

void ShowHelp()
{
	if (!g_bShowHelp)
	{
		return;
	}

	if (g_helpScreen == nullptr)
	{
		return;
	}

	if (g_pGameWindow == nullptr)
	{
		return;
	}

	int screenW = g_pGameWindow->width;
	int screenH = g_pGameWindow->height;

	int helpW = g_helpScreen->width;
	int helpH = g_helpScreen->height;

	int x = screenW / 2 - helpW / 2;
	int y = screenH / 2 - helpH / 2;

	g_helpScreen->x = x;
	g_helpScreen->y = y;

	S2D_DrawImage(g_helpScreen);
}

void SaveGame()
{
	// save the current level on file
	_tstring strFilePath = g_gw.GetPMDirectory();
	strFilePath += SAVE_FILE_NAME;

	int iLevel = (int)g_gw.GetLevel();
	int iLives = (int)g_gw.GetLives();
	int iTotalLives = (int)g_gw.GetTotalLives();
	int iScore = (int)g_gw.GetScore();

	if (iLevel <= 0 || iLives <= 0 || iTotalLives <= 0 || iScore <= 0)
	{
		return;
	}

	_tstring strFileData = CUtil::format(_T("%d \r\n%d \r\n%d \r\n%d \r\n"), iLevel, iLives, iTotalLives, iScore);

	CUtil::writeFile(strFilePath, strFileData);
}

void LoadGame()
{
	// load the saved level from file
	_tstring strFilePath = g_gw.GetPMDirectory();
	strFilePath += SAVE_FILE_NAME;

	_tstring strFileData = _T("");
	CUtil::readFile(strFilePath, strFileData);

	int iLevel = _ttoi(CUtil::getToken(strFileData, _T("\r\n"), 0).c_str());
	int iLives = _ttoi(CUtil::getToken(strFileData, _T("\r\n"), 1).c_str());
	int iTotalLives = _ttoi(CUtil::getToken(strFileData, _T("\r\n"), 2).c_str());
	int iScore = _ttoi(CUtil::getToken(strFileData, _T("\r\n"), 3).c_str());

	if (iLevel <= 0 || iLives <= 0 || iTotalLives <= 0 || iScore <= 0)
	{
		return;
	}

	// restart game
	StartGameWorld(iLevel);

	// set the loaded data
	g_gw.SetSavedData(iScore, iLives, iTotalLives, iLevel);

	// auto play off
	g_gw.SetDemo(false);
}

void ScreenShot()
{
	static int i = 0;
	i++;

	if (g_pGameWindow == nullptr)
	{
		return;
	}

	// save the screen shot to file
	_tstring strFilePath = g_gw.GetPMDirectory();
	strFilePath += SCREEN_SHOT_NAME;
	strFilePath += _T("-");
	strFilePath += CUtil::toString(i, 2);
	strFilePath += SCREEN_SHOT_EXT;

	S2D_Screenshot(g_pGameWindow, CUtil::getStringA(strFilePath).c_str());
}

void OpenSaveGameFolder()
{
	// get the save game folder path
	_tstring strFilePath = g_gw.GetPMDirectory();

	ShellExecute(0, 0, strFilePath.c_str(), 0, 0, SW_SHOW);
}

void OpenSaveGameFile()
{
	// get the save game file path
	_tstring strFilePath = g_gw.GetPMDirectory();
	strFilePath += SAVE_FILE_NAME;

	ShellExecute(0, 0, strFilePath.c_str(), 0, 0, SW_SHOW);
}

void Cheat()
{
	if (g_pGameWindow == nullptr)
	{
		return;
	}

	g_x = g_pGameWindow->mouse.x;
	g_y = g_pGameWindow->mouse.y;

	if (!g_gw.IsRunning())
	{
		return;
	}

	_Point ptGrid = CAgent::ToGrid(_Point(g_x, g_y));

	g_x = ptGrid.x / 2;
	g_y = ptGrid.y / 2;

	if (g_gw.IsCheatActive())
	{
		g_gw.Move(AGENT_ID::RUNNER, ptGrid);
	}
	else
	{
		if (ptGrid.IsNear(g_gw.GetAgent(RUNNER).GetCurrentLocation(), 3.0))
		{
			g_gw.Move(AGENT_ID::RUNNER, ptGrid);
		}
	}
}

void StartNextLevel()
{
	// check for completion
	if (g_gw.IsLevelComplete())
	{
		// restart game
		StartGameWorld(g_gw.GetLevel());

		return;
	}

	// check game over
	if (g_gw.IsGameOver())
	{
		// restart game
		StartGameWorld(1);

		return;
	}
}
