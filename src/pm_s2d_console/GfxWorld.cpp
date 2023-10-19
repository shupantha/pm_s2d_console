#include "stdafx.h"
#include "GfxWorld.h"
#include "Settings.h"
#include "GfxDoor.h"
#include "GfxMonster.h"
#include "GfxPill.h"
#include "GfxRunner.h"
#include "Mazes.h"

#pragma warning(disable : 26812)

// settings
extern CSettings g_settings;

// debug info
extern bool g_bShowDebugInfo;

// current level
extern unsigned int g_uiLevel;

// bonuses: orange fruit - % permanent speed bonus
extern bool g_bSpeedBoost;

// bonuses: kiwi fruit - % extra power pill time
extern bool g_bTimeBoost;

// bonuses: mango fruit - kill 1 or 2 random monsters permanently
extern bool g_bPermaKill;

// bonuses: dragon fruit - show path to exit for a set amount of time
extern bool g_bShowPathToExit;

// cheat: click - show path
extern bool g_bShowPathToClick;

CGfxWorld::CGfxWorld()
{
	InitializeImages();
	Initialize();
}

CGfxWorld::~CGfxWorld()
{
	Initialize();
	DetachImages();
	InitializeImages();
}

void CGfxWorld::Initialize()
{
	// initialize members
	m_iWidth = 0;
	m_iHeight = 0;

	m_iCellSize = 0;
	m_iWallSize = 0;

	m_iFrameCounter = 0;

	m_iSpriteWidth = 0;
	m_iSpriteHeight = 0;

	m_dSpriteScaleX = 0.0;
	m_dSpriteScaleY = 0.0;

	m_iSpriteScaledWidth = 0;
	m_iSpriteScaledHeight = 0;

	// delete the gfx elements
	for (auto it = m_vpGfxElements.begin(), et = m_vpGfxElements.end(); it != et; ++it)
	{
		IGfxElement* pGfxElement = *it;
		if (!pGfxElement)
		{
			continue;
		}

		delete pGfxElement;
	}

	m_vpGfxElements.clear();

	m_pMaze = nullptr;
}

void CGfxWorld::DetachImages()
{
	S2D_FreeImage(m_spriteTemplate);
}

void CGfxWorld::InitializeImages()
{
	m_spriteTemplate = nullptr;
}

int CGfxWorld::GetElementCount()
{
	int iMazeWidth = m_iWidth / m_iCellSize;
	int iMazeHeight = m_iHeight / m_iCellSize;

	return (iMazeWidth * iMazeHeight);
}

int CGfxWorld::GetPowerPillCount()
{
	return (int)(GetElementCount() * POWER_PILL_FACTOR);
}

void CGfxWorld::Load(int iWidth, int iHeight, int iCellSize)
{
	Initialize();

	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_iCellSize = iCellSize;

	// create all elements
	for (int i = 0; i < GetElementCount(); ++i)
	{
		IGfxElement* pGfxElement = nullptr;

		AGENT_ID id = (AGENT_ID)i;

		switch (id)
		{
		case RUNNER:
			pGfxElement = new CGfxRunner(id);
			break;
		case GINKY:
			pGfxElement = new CGfxMonster(id);
			break;
		case BINKY:
			pGfxElement = new CGfxMonster(id);
			break;
		case YINKY:
			pGfxElement = new CGfxMonster(id);
			break;
		case RINKY:
			pGfxElement = new CGfxMonster(id);
			break;
		case ENTRANCE:
			pGfxElement = new CGfxDoor(id);
			break;
		case EXIT:
			pGfxElement = new CGfxDoor(id);
			break;
		case ORANGE:
			pGfxElement = new CGfxPill(id);
			break;
		case KIWI:
			pGfxElement = new CGfxPill(id);
			break;
		case MANGO:
			pGfxElement = new CGfxPill(id);
			break;
		case DRAGON:
			pGfxElement = new CGfxPill(id);
			break;
		default:
			break;
		}

		if (id >= UNIQUE_ID_COUNT && id < UNIQUE_ID_COUNT + GetPowerPillCount())
		{
			pGfxElement = new CGfxPill(POWER);
		}

		if (id >= UNIQUE_ID_COUNT + GetPowerPillCount())
		{
			pGfxElement = new CGfxPill(PILL);
		}

		if (!pGfxElement)
		{
			continue;
		}

		pGfxElement->Load();
		m_vpGfxElements.push_back(pGfxElement);
	}

	m_clrWall = g_settings.wallColor;
	m_clrPath = g_settings.pathColor;
	m_clrPathClick = g_settings.pathClickColor;

	//////////////////////////////////////////////////////////////////////////
	// Load all the external bitmaps
	//////////////////////////////////////////////////////////////////////////
	m_spriteTemplate = S2D_CreateImage("./gfx/common/st.bmp");
	//////////////////////////////////////////////////////////////////////////

	// get and set the default sprite size
	m_iSpriteWidth = m_spriteTemplate->width;
	m_iSpriteHeight = m_spriteTemplate->height;
	//////////////////////////////////////////////////////////////////////////
}

void CGfxWorld::ResizeSprites()
{
	// detach previous, and attach the resized maze
	// m_mazeImage->width = m_iWidth;
	// m_mazeImage->height = m_iHeight;

	// resize gfx elements
	for (int i = 0; i < (int)m_vpGfxElements.size(); ++i)
	{
		IGfxElement* pGfxElement = m_vpGfxElements.at(i);
		if (!pGfxElement)
		{
			continue;
		}
		
		pGfxElement->ResizeSprites(m_iSpriteScaledWidth, m_iSpriteScaledHeight);
	}
}

bool CGfxWorld::SetMaze(const unsigned char* pMaze, int iMazeSizeX, int iMazeSizeY, int iCellSize)
{
	if (!pMaze) return false;

	m_pMaze = pMaze;

	m_iWidth = iMazeSizeX * iCellSize;
	m_iHeight = iMazeSizeY * iCellSize;

	m_iCellSize = iCellSize;

	m_iWallSize = g_settings.wallSize;

	m_iSpriteWidth != 0 ? m_dSpriteScaleX = ((double)iCellSize - m_iWallSize) / m_iSpriteWidth : 0;
	m_iSpriteHeight != 0 ? m_dSpriteScaleY = ((double)iCellSize - m_iWallSize) / m_iSpriteHeight : 0;

	m_iSpriteScaledWidth = (int)(m_iSpriteWidth * m_dSpriteScaleX) - 1;
	m_iSpriteScaledHeight = (int)(m_iSpriteHeight * m_dSpriteScaleY) - 1;

	return true;
}

void CGfxWorld::Draw(vector<CAgent>& vAgents, vector<_Point>& vptPath, bool bgOnly, bool bNoExtras)
{
	// Draw maze / background
	DrawMaze();

	if (bgOnly)
	{
		return;
	}

	// draw agents
	Draw(vAgents, bNoExtras);

	// draw path
	Draw(vptPath);

	m_iFrameCounter++;
}

void CGfxWorld::Draw(vector<CAgent>& vAgents, bool bNoExtras)
{
	// draw every agent
	for (int i = 0; i < (int)vAgents.size(); ++i)
	{
		CAgent agent = vAgents.at(i);

		if (!agent.IsValid())
		{
			continue;
		}

		if (bNoExtras && agent.id() == PILL)
		{
			break;
		}

		Draw(agent, i, bNoExtras);
	}
}

void CGfxWorld::Draw(CAgent& agent, int iIndex, bool bNoExtras)
{
	int x = agent.position().x + m_iWallSize;
	int y = agent.position().y + m_iWallSize;

	IGfxElement* pGfxElement = m_vpGfxElements.at(iIndex);
	if (!pGfxElement)
	{
		return;
	}

	pGfxElement->Draw(x, y, agent.direction(), agent.state());

	if (bNoExtras)
	{
		return;
	}

	if (agent.id() == RUNNER)
	{
		if (agent.state() == CAgent::ALIVE)
		{
			int iCount = 0;
			if (g_bSpeedBoost) iCount++;
			if (g_bTimeBoost) iCount++;
			if (g_bPermaKill) iCount++;
			if (g_bShowPathToExit) iCount++;

			std::string str = "";
			switch (iCount)
			{
			case 1:
				str = ".";
				break;
			case 2:
				str = ":";
				break;
			case 3:
				str = ":.";
				break;
			case 4:
				str = "::";
				break;
			default:
				break;
			}
			pGfxElement->DrawInfo(x, y, (void*)&str);
		}

		if (agent.state() == CAgent::DEAD)
		{
			int t = (int)agent.state_time();
			t = (RUNNER_SPAWN_TIMEOUT - t) / 250;
			if ((t >= 0) && (t < (RUNNER_SPAWN_TIMEOUT / 250)))
			{
				std::string str = CUtil::toStringA(t);
				pGfxElement->DrawInfo(x, y, (void*)& str);
			}
		}
	}

	if (agent.id() == GINKY || agent.id() == BINKY || agent.id() == YINKY || agent.id() == RINKY)
	{
		if (agent.state() == CAgent::VULNERABLE)
		{
			int iStateTime = g_bTimeBoost ? (int)(MONSTER_VULNERABLE_TIMEOUT * TIME_BOOST) : MONSTER_VULNERABLE_TIMEOUT;

			int t = (int)agent.state_time();
			t = (iStateTime - t) / 1000;
			if ((t >= 0) && (t < (iStateTime / 1000)))
			{
				std::string str = CUtil::toStringA(t);
				pGfxElement->DrawInfo(x, y, (void*)& str);
			}
		}
	}

	if (g_bShowDebugInfo)
	{
		if ((agent.id() == PILL || agent.id() == POWER))
		{
			_Point pt = agent.GetCurrentLocation();
			std::string str = CUtil::format("(%d, %d)", pt.x / 2, pt.y / 2);

			pGfxElement->DrawInfo(x, y, (void*)&str);
		}
	}
}

void CGfxWorld::Draw(vector<_Point>& vptPath)
{
	static float r = m_clrPath.r;
	static float g = m_clrPath.g;
	static float b = m_clrPath.b;
	static float a = m_clrPath.a;

	r = (rand() % (m_clrPath.GetR() + 1)) / 255.0f;
	g = (rand() % (m_clrPath.GetG() + 1)) / 255.0f;
	b = (rand() % (m_clrPath.GetB() + 1)) / 255.0f;

	if (g_bShowPathToClick)
	{
		r = (rand() % (m_clrPathClick.GetR() + 1)) / 255.0f;
		g = (rand() % (m_clrPathClick.GetG() + 1)) / 255.0f;
		b = (rand() % (m_clrPathClick.GetB() + 1)) / 255.0f;
	}

	for (int i = 1; i < (int)vptPath.size(); ++i)
	{
		_Point pt1 = vptPath.at((size_t)i - 1);
		float x1 = pt1.x + m_iWallSize + m_iCellSize / 2.0f;
		float y1 = pt1.y + m_iWallSize + m_iCellSize / 2.0f;

		_Point pt2 = vptPath.at(i);
		float x2 = pt2.x + m_iWallSize + m_iCellSize / 2.0f;
		float y2 = pt2.y + m_iWallSize + m_iCellSize / 2.0f;

		S2D_DrawLine(x1, y1, x2, y2,
			1.0f,
			r, g, b, a,
			r, g, b, a,
			r, g, b, a,
			r, g, b, a);
	}
}

void CGfxWorld::DrawMaze()
{
	static float r = m_clrWall.r;
	static float g = m_clrWall.g;
	static float b = m_clrWall.b;
	static float a = m_clrWall.a;

	if (!m_pMaze)
	{
		return;
	}

	const unsigned char* pMaze = m_pMaze;

	int iWall = m_iWallSize;

	int iCellSize = m_iCellSize;

	int iMazeSizeX = m_iWidth / m_iCellSize;
	int iMazeSizeY = m_iHeight / m_iCellSize;

	for (int y = 0; y < iMazeSizeY; ++y)
	{
		int yy = y * iMazeSizeX;
		for (int x = 0; x < iMazeSizeX; ++x)
		{
			unsigned char cell = pMaze[x + yy];
			int nx = x * iCellSize,
				ny = y * iCellSize;

			if (!(cell & mazes::NORTH))
			{
				int x1 = nx + iWall;
				int y1 = ny + iWall;
				int x2 = nx + iCellSize + iWall;
				int y2 = ny + iWall;

				S2D_DrawLine((float)x1, (float)y1, (float)x2, (float)y2,
					(float)iWall,
					r, g, b, a,
					r, g, b, a,
					r, g, b, a,
					r, g, b, a);
			}
			if (!(cell & mazes::EAST))
			{
				int x1 = nx + iCellSize + iWall;
				int y1 = ny + iWall;
				int x2 = nx + iCellSize + iWall;
				int y2 = ny + iCellSize + iWall;

				S2D_DrawLine((float)x1, (float)y1, (float)x2, (float)y2,
					(float)iWall,
					r, g, b, a,
					r, g, b, a,
					r, g, b, a,
					r, g, b, a);
			}
			if (!(cell & mazes::SOUTH))
			{
				int x1 = nx + iWall;
				int y1 = ny + iCellSize + iWall;
				int x2 = nx + iCellSize + iWall;
				int y2 = ny + iCellSize + iWall;

				S2D_DrawLine((float)x1, (float)y1, (float)x2, (float)y2,
					(float)iWall,
					r, g, b, a,
					r, g, b, a,
					r, g, b, a,
					r, g, b, a);
			}
			if (!(cell & mazes::WEST))
			{
				int x1 = nx + iWall;
				int y1 = ny + iWall;
				int x2 = nx + iWall;
				int y2 = ny + iCellSize + iWall;

				S2D_DrawLine((float)x1, (float)y1, (float)x2, (float)y2,
					(float)iWall,
					r, g, b, a,
					r, g, b, a,
					r, g, b, a,
					r, g, b, a);
			}
		}
	}
}
