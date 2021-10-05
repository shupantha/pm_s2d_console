#pragma once

#include "IGfxElement.h"

class CGfxRunner : public IGfxElement
{
public:
	CGfxRunner(int iID);
	~CGfxRunner();

public:
	// directions
	static const int RIGHT	= 1;
	static const int LEFT	= -1;

private:
	void Initialize();

public:
	void Load();

	void ResizeSprites(int iWidth, int iHeight);

	void Draw(int x, int y, int iDirection, int iState);

	void DrawInfo(int x, int y, void* pInfo);

	int GetID() { return m_iID; };

private:
	int m_iID;

private:
	void DrawDead(int x, int y, int iDirection);
	void DrawAlive(int x, int y, int iDirection);

private:
	int m_iFrameCounter0;
	int m_iFrameCounter1;

	int m_iWidth;
	int m_iHeight;

private:
	int m_iDirection;

private:
	CTimer m_Timer0;
	CTimer m_Timer1;

private:
	// images facing right
	S2D_Image* m_mazeRunner0;
	S2D_Image* m_mazeRunner1;
	S2D_Image* m_mazeRunner2;
	S2D_Image* m_mazeRunner3;
	S2D_Image* m_mazeRunner4;
	S2D_Image* m_mazeRunner5;

	S2D_Image* m_mazeRunnerDead0;
	S2D_Image* m_mazeRunnerDead1;
	S2D_Image* m_mazeRunnerDead2;

	// images facing left
	S2D_Image* m_mazeRunner0_;
	S2D_Image* m_mazeRunner1_;
	S2D_Image* m_mazeRunner2_;
	S2D_Image* m_mazeRunner3_;
	S2D_Image* m_mazeRunner4_;
	S2D_Image* m_mazeRunner5_;

	S2D_Image* m_mazeRunnerDead0_;
	S2D_Image* m_mazeRunnerDead1_;
	S2D_Image* m_mazeRunnerDead2_;

	// text
	S2D_Text* m_text;
};
