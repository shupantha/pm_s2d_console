#include "stdafx.h"
#include "GfxPill.h"

#include "Settings.h"
#include "Agent.h"

// settings
extern CSettings g_settings;

CGfxPill::CGfxPill(int iID)
	: IGfxElement(iID)
{
	Initialize();

	m_iID = iID;
}

CGfxPill::~CGfxPill()
{
	S2D_FreeImage(m_pill0);
	S2D_FreeImage(m_pill1);
	S2D_FreeImage(m_pill2);
}

void CGfxPill::Initialize()
{
	// initialize members
	m_iFrameCounter = 0;

	m_iWidth = 0;
	m_iHeight = 0;

	m_pill0 = nullptr;
	m_pill1 = nullptr;
	m_pill2 = nullptr;

	m_text = nullptr;

	// start the timer
	m_Timer.Start();
}

void CGfxPill::Load()
{
	Initialize();

	//////////////////////////////////////////////////////////////////////////
	// Load all the external bitmaps
	//////////////////////////////////////////////////////////////////////////
	switch (m_iID)
	{
	case PILL:
		// m_pill0 = S2D_CreateImage("./gfx/pills/n0.png");
		// m_pill1 = S2D_CreateImage("./gfx/pills/n1.png");
		// m_pill2 = S2D_CreateImage("./gfx/pills/n2.png");
		break;

	case POWER:
		m_pill0 = S2D_CreateImage("./gfx/pills/p0.png");
		m_pill1 = S2D_CreateImage("./gfx/pills/p1.png");
		m_pill2 = S2D_CreateImage("./gfx/pills/p2.png");
		break;

	case ORANGE:
		m_pill0 = S2D_CreateImage("./gfx/pills/o0.png");
		m_pill1 = S2D_CreateImage("./gfx/pills/o1.png");
		m_pill2 = S2D_CreateImage("./gfx/pills/o2.png");
		break;

	case KIWI:
		m_pill0 = S2D_CreateImage("./gfx/pills/k0.png");
		m_pill1 = S2D_CreateImage("./gfx/pills/k1.png");
		m_pill2 = S2D_CreateImage("./gfx/pills/k2.png");
		break;

	case MANGO:
		m_pill0 = S2D_CreateImage("./gfx/pills/m0.png");
		m_pill1 = S2D_CreateImage("./gfx/pills/m1.png");
		m_pill2 = S2D_CreateImage("./gfx/pills/m2.png");
		break;

	case DRAGON:
		m_pill0 = S2D_CreateImage("./gfx/pills/d0.png");
		m_pill1 = S2D_CreateImage("./gfx/pills/d1.png");
		m_pill2 = S2D_CreateImage("./gfx/pills/d2.png");
		break;

	default:
		break;
	}
	//////////////////////////////////////////////////////////////////////////

	// load text
	std::string strFont = "./msc/" + g_settings.textFont;
	m_text = S2D_CreateText(strFont.c_str(), "", 8);

	// text color
	m_text->color.r = g_settings.textColor.r;
	m_text->color.g = g_settings.textColor.g;
	m_text->color.b = g_settings.textColor.b;
	m_text->color.a = g_settings.textColor.a;

	m_clrPill = g_settings.pillColor;
}

void CGfxPill::ResizeSprites(int iWidth, int iHeight)
{
	// set dimensions
	m_iWidth = iWidth;
	m_iHeight = iHeight;

	if (m_pill0 == nullptr)
	{
		return;
	}

	// set width
	m_pill0->width = iWidth;
	m_pill1->width = iWidth;
	m_pill2->width = iWidth;

	// set height
	m_pill0->height = iHeight;
	m_pill1->height = iHeight;
	m_pill2->height = iHeight;
}

void CGfxPill::Draw(int x, int y, int iDirection, int iState)
{
	if (iState == CAgent::DEAD)
	{
		return;
	}

	if (DrawPill(x, y))
	{
		return;
	}

	// 1/3 counter
	int iCounter = m_iFrameCounter / 3;

	switch (iCounter % 6)
	{
	case 0:
		m_pill0->x = x;
		m_pill0->y = y;
		S2D_DrawImage(m_pill0);
		break;
	case 1:
		m_pill1->x = x;
		m_pill1->y = y;
		S2D_DrawImage(m_pill1);
		break;
	case 2:
		m_pill2->x = x;
		m_pill2->y = y;
		S2D_DrawImage(m_pill2);
		break;
	case 3:
		m_pill2->x = x;
		m_pill2->y = y;
		S2D_DrawImage(m_pill2);
		break;
	case 4:
		m_pill1->x = x;
		m_pill1->y = y;
		S2D_DrawImage(m_pill1);
		break;
	case 5:
		m_pill0->x = x;
		m_pill0->y = y;
		S2D_DrawImage(m_pill0);
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

bool CGfxPill::DrawPill(int x, int y)
{
	static float r = m_clrPill.r;
	static float g = m_clrPill.g;
	static float b = m_clrPill.b;
	static float a = m_clrPill.a;

	if (m_iID != PILL)
	{
		return false;
	}

	float X = (x + m_iWidth / 2.0f);
	float Y = (y + m_iHeight / 2.0f);
	float Width = 1.0f;
	float Height = 1.0f;

	// 1/3 counter
	int iCounter = m_iFrameCounter / 3;

	switch (iCounter % 6)
	{
	case 0:
		X -= 1;
		break;
	case 1:
		X += 0;
		break;
	case 2:
		X += 1;
		break;
	case 3:
		X += 1;
		break;
	case 4:
		X += 0;
		break;
	case 5:
		X -= 1;
		break;
	default:
		break;
	}

	S2D_DrawLine(X, Y, X + Width, Y,
		1.0f,
		r, g, b, a,
		r, g, b, a,
		r, g, b, a,
		r, g, b, a);

	if (m_Timer.GetElapsedTime() > PERIOD_20_FPS)
	{
		m_iFrameCounter++;
		m_Timer.Start();
	}

	return true;
}

void CGfxPill::DrawInfo(int x, int y, void* pInfo)
{
	if (pInfo == nullptr)
	{
		return;
	}

	// initialize arguments
	std::string* pstr = (std::string*)(pInfo);
	S2D_SetText(m_text, pstr->c_str());

	m_text->x = x + 1;
	m_text->y = y + 1;

	// draw string
	S2D_DrawText(m_text);
}
