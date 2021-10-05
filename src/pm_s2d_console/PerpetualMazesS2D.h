#pragma once

#include "..\simple2d\simple2d.h"

void StartGameWorld(unsigned int uiLevel);
void UpdateGameWorld();
void RenderGameWorld();

void on_key(S2D_Event e);
void on_mouse(S2D_Event e);

void ShowHelp();

void SaveGame();
void LoadGame();

void ScreenShot();

void Cheat();
void StartNextLevel();
