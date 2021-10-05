#pragma once

#include "IGfxElement.h"
#include "_Structures.h"

class CGfxPill : public IGfxElement
{
public:
	CGfxPill(int iID);
	~CGfxPill();

private:
	void Initialize();

public:
	void Load();

	void ResizeSprites(int iWidth, int iHeight);

	void Draw(int x, int y, int iDirection, int iState);

	void DrawInfo(int x, int y, void* pInfo) {};

	int GetID() { return m_iID; };

private:
	bool DrawPill(int x, int y);

private:
	int m_iID;

private:
	int m_iFrameCounter;

	int m_iWidth;
	int m_iHeight;

	_Color m_clrPill;

private:
	CTimer m_Timer;

private:
	// pill images
	S2D_Image* m_pill0;
	S2D_Image* m_pill1;
	S2D_Image* m_pill2;
};
