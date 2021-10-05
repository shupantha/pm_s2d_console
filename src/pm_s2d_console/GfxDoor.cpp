#include "stdafx.h"
#include "GfxDoor.h"

#include "Agent.h"

CGfxDoor::CGfxDoor(int iID)
	: IGfxElement(iID)
{
	Initialize();

	m_iID = iID;
}

CGfxDoor::~CGfxDoor()
{
	S2D_FreeImage(m_door0);
	S2D_FreeImage(m_door1);
	S2D_FreeImage(m_door2);
}

void CGfxDoor::Initialize()
{
	// initialize members
	m_iFrameCounter = 0;

	m_iWidth = 0;
	m_iHeight = 0;

	m_door0 = nullptr;
	m_door1 = nullptr;
	m_door2 = nullptr;

	// start the timer
	m_Timer.Start();
}

void CGfxDoor::Load()
{
	Initialize();

	//////////////////////////////////////////////////////////////////////////
	// Load all the external bitmaps
	//////////////////////////////////////////////////////////////////////////
	switch (m_iID)
	{
	case ENTRANCE:
		m_door0 = S2D_CreateImage("./gfx/doors/en0.png");
		m_door1 = S2D_CreateImage("./gfx/doors/en1.png");
		m_door2 = S2D_CreateImage("./gfx/doors/en2.png");
		break;

	case EXIT:
		m_door0 = S2D_CreateImage("./gfx/doors/ex0.png");
		m_door1 = S2D_CreateImage("./gfx/doors/ex1.png");
		m_door2 = S2D_CreateImage("./gfx/doors/ex2.png");
		break;

	default:
		break;
	}
	//////////////////////////////////////////////////////////////////////////
}

void CGfxDoor::ResizeSprites(int iWidth, int iHeight)
{
	// set dimensions
	m_iWidth = iWidth;
	m_iHeight = iHeight;

	if (m_door0 == nullptr)
	{
		return;
	}

	// set width
	m_door0->width = iWidth;
	m_door1->width = iWidth;
	m_door2->width = iWidth;

	// set height
	m_door0->height = iHeight;
	m_door1->height = iHeight;
	m_door2->height = iHeight;
}

void CGfxDoor::Draw(int x, int y, int iDirection, int iState)
{
	if (iState == CAgent::DEAD)
	{
		return;
	}

	// 1/3 counter
	int iCounter = m_iFrameCounter / 3;

	switch (iCounter % 6)
	{
	case 0:
		m_door0->x = x;
		m_door0->y = y;
		S2D_DrawImage(m_door0);
		break;
	case 1:
		m_door1->x = x;
		m_door1->y = y;
		S2D_DrawImage(m_door1);
		break;
	case 2:
		m_door2->x = x;
		m_door2->y = y;
		S2D_DrawImage(m_door2);
		break;
	case 3:
		m_door2->x = x;
		m_door2->y = y;
		S2D_DrawImage(m_door2);
		break;
	case 4:
		m_door1->x = x;
		m_door1->y = y;
		S2D_DrawImage(m_door1);
		break;
	case 5:
		m_door0->x = x;
		m_door0->y = y;
		S2D_DrawImage(m_door0);
		break;
	default:
		break;
	}

	if (m_Timer.GetElapsedTime() > PERIOD_20_FPS)
	{
		m_iFrameCounter++;
		m_Timer.Start();
	}
}
