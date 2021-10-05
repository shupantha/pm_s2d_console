// pm_s2d_console.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "PerpetualMazesS2D.h"
#include "Console.h"

#include <iostream>

int main(int argc, char* args[])
{
	CConsole console;
	console.Show(false);
	console.Close();

	// start the game at level 1
	StartGameWorld(1);

	return 0;
}
