#pragma once

#include "IGfxElement.h"

class CGfxMonster : public IGfxElement
{
public:
	CGfxMonster(int iID);
	~CGfxMonster();

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
	void DrawEdible(int x, int y, int iDirection);
	void DrawAlive(int x, int y, int iDirection);

private:
	int m_iFrameCounter0;
	int m_iFrameCounter1;
	int m_iFrameCounter2;

	int m_iWidth;
	int m_iHeight;

private:
	int m_iDirection;

private:
	CTimer m_Timer0;
	CTimer m_Timer1;
	CTimer m_Timer2;

private:
	// facing right
	// monster images
	S2D_Image* m_monster0;
	S2D_Image* m_monster1;
	S2D_Image* m_monster2;
	S2D_Image* m_monster3;
	S2D_Image* m_monster4;
	S2D_Image* m_monster5;

	// edible monsters
	S2D_Image* m_monsterEdible0;
	S2D_Image* m_monsterEdible1;
	S2D_Image* m_monsterEdible2;
	S2D_Image* m_monsterEdible3;
	S2D_Image* m_monsterEdible4;
	S2D_Image* m_monsterEdible5;

	// dead monsters
	S2D_Image* m_monsterDead0;
	S2D_Image* m_monsterDead1;
	S2D_Image* m_monsterDead2;
	S2D_Image* m_monsterDead3;
	S2D_Image* m_monsterDead4;
	S2D_Image* m_monsterDead5;

	// facing left
	// monster images
	S2D_Image* m_monster0_;
	S2D_Image* m_monster1_;
	S2D_Image* m_monster2_;
	S2D_Image* m_monster3_;
	S2D_Image* m_monster4_;
	S2D_Image* m_monster5_;

	// edible monsters
	S2D_Image* m_monsterEdible0_;
	S2D_Image* m_monsterEdible1_;
	S2D_Image* m_monsterEdible2_;
	S2D_Image* m_monsterEdible3_;
	S2D_Image* m_monsterEdible4_;
	S2D_Image* m_monsterEdible5_;

	// dead monsters
	S2D_Image* m_monsterDead0_;
	S2D_Image* m_monsterDead1_;
	S2D_Image* m_monsterDead2_;
	S2D_Image* m_monsterDead3_;
	S2D_Image* m_monsterDead4_;
	S2D_Image* m_monsterDead5_;

	// text
	S2D_Text* m_text;
};
