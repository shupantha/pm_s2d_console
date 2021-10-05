#pragma once

#include "Util.h"
#include "Timer.h"

#include "..\simple2d\simple2d.h"

#define DELETE_PTR(p)	(p ? delete p : 0)

class IGfxElement
{
public:
	IGfxElement(int iID);
	~IGfxElement();

public:
	virtual void Load() = 0;

	virtual void ResizeSprites(int iWidth, int iHeight) = 0;

	virtual void Draw(int x, int y, int iDirection, int iState) = 0;

	virtual void DrawInfo(int x, int y, void* pInfo) = 0;

	virtual int GetID() = 0;
};
