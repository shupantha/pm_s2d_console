#pragma once

#include "Util.h"
#include "IGfxElement.h"
#include "_Structures.h"
#include "Agent.h"

class CGfxWorld
{
public:
	CGfxWorld();
	~CGfxWorld();

private:
	vector<IGfxElement*> m_vpGfxElements;

private:
	void Initialize();

	void DetachImages();
	void InitializeImages();

public:
	int GetWallSize() { return m_iWallSize; };

	int GetElementCount();

	int GetPowerPillCount();

public:
	void Load(int iWidth, int iHeight, int iCellSize);

	bool SetMaze(const unsigned char* pMaze, int iMazeSizeX, int iMazeSizeY, int iCellSize);

	void ResizeSprites();

	void Draw(vector<CAgent>& vAgents, vector<_Point>& vptPath, bool bgOnly, bool bNoExtras);

private:
	void Draw(vector<CAgent>& vAgents, bool bNoExtras);
	void Draw(CAgent& agent, int iIndex, bool bNoExtras);
	void Draw(vector<_Point>& vptPath);
	void DrawMaze();

private:
	int m_iFrameCounter;

private:
	int m_iWidth;
	int m_iHeight;

	int m_iCellSize;

	int m_iWallSize;

private:
	_Color m_clrWall;
	_Color m_clrPath;
	_Color m_clrPathClick;

private:
	int m_iSpriteWidth;
	int m_iSpriteHeight;

	double m_dSpriteScaleX;
	double m_dSpriteScaleY;

	int m_iSpriteScaledWidth;
	int m_iSpriteScaledHeight;

private:
	S2D_Image* m_spriteTemplate;

private:
	const unsigned char* m_pMaze;
};
