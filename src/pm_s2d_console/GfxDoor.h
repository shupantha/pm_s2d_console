#pragma once

#include "IGfxElement.h"

class CGfxDoor : public IGfxElement
{
public:
	CGfxDoor(int iID);
	~CGfxDoor();

private:
	void Initialize();

public:
	void Load();

	void ResizeSprites(int iWidth, int iHeight);

	void Draw(int x, int y, int iDirection, int iState);

	void DrawInfo(int x, int y, void* pInfo) {};

	int GetID() { return m_iID; };

private:
	int m_iID;

private:
	int m_iFrameCounter;

	int m_iWidth;
	int m_iHeight;

private:
	CTimer m_Timer;

private:
	// pill images
	S2D_Image* m_door0;
	S2D_Image* m_door1;
	S2D_Image* m_door2;
};
