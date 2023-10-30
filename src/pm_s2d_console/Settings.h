#pragma once
#include "_Structures.h"
#include "Util.h"

class CSettings
{
public:
	CSettings();
	~CSettings();

private:
	void Load();

public:
	void Load(_tstring strSettingsFileName);
	void LoadNext();

public:
	int wallSize;
	_Color wallColor;

	_Color pathColor;
	_Color pathClickColor;

	_Color pillColor;

	std::string textFont;
	_Color textColor;
};
