#include "stdafx.h"
#include "GfxRunner.h"

#include "Settings.h"
#include "Agent.h"

// settings
extern CSettings g_settings;

// current level
extern unsigned int g_uiLevel;

CGfxRunner::CGfxRunner(int iID)
	: IGfxElement(iID)
{
	Initialize();

	m_iID = iID;
}

CGfxRunner::~CGfxRunner()
{
	S2D_FreeImage(m_mazeRunner0);
	S2D_FreeImage(m_mazeRunner1);
	S2D_FreeImage(m_mazeRunner2);
	S2D_FreeImage(m_mazeRunner3);
	S2D_FreeImage(m_mazeRunner4);
	S2D_FreeImage(m_mazeRunner5);

	S2D_FreeImage(m_mazeRunnerDead0);
	S2D_FreeImage(m_mazeRunnerDead1);
	S2D_FreeImage(m_mazeRunnerDead2);

	S2D_FreeImage(m_mazeRunner0_);
	S2D_FreeImage(m_mazeRunner1_);
	S2D_FreeImage(m_mazeRunner2_);
	S2D_FreeImage(m_mazeRunner3_);
	S2D_FreeImage(m_mazeRunner4_);
	S2D_FreeImage(m_mazeRunner5_);

	S2D_FreeImage(m_mazeRunnerDead0_);
	S2D_FreeImage(m_mazeRunnerDead1_);
	S2D_FreeImage(m_mazeRunnerDead2_);

	S2D_FreeText(m_text);
}

void CGfxRunner::Initialize()
{
	// initialize members
	m_iFrameCounter1 = 0;
	m_iFrameCounter0 = 0;

	m_iWidth = 0;
	m_iHeight = 0;

	m_iDirection = 1;

	m_mazeRunner0 = nullptr;
	m_mazeRunner1 = nullptr;
	m_mazeRunner2 = nullptr;
	m_mazeRunner3 = nullptr;
	m_mazeRunner4 = nullptr;
	m_mazeRunner5 = nullptr;

	m_mazeRunnerDead0 = nullptr;
	m_mazeRunnerDead1 = nullptr;
	m_mazeRunnerDead2 = nullptr;

	m_mazeRunner0_ = nullptr;
	m_mazeRunner1_ = nullptr;
	m_mazeRunner2_ = nullptr;
	m_mazeRunner3_ = nullptr;
	m_mazeRunner4_ = nullptr;
	m_mazeRunner5_ = nullptr;

	m_mazeRunnerDead0_ = nullptr;
	m_mazeRunnerDead1_ = nullptr;
	m_mazeRunnerDead2_ = nullptr;

	m_text = nullptr;

	// start the timers
	m_Timer0.Start();
	m_Timer1.Start();
}

void CGfxRunner::Load()
{
	Initialize();

	//////////////////////////////////////////////////////////////////////////
	// Load all the external bitmaps
	//////////////////////////////////////////////////////////////////////////
	m_mazeRunner0 = S2D_CreateImage("./gfx/r/r0.png");
	m_mazeRunner1 = S2D_CreateImage("./gfx/r/r1.png");
	m_mazeRunner2 = S2D_CreateImage("./gfx/r/r2.png");
	m_mazeRunner3 = S2D_CreateImage("./gfx/r/r3.png");
	m_mazeRunner4 = S2D_CreateImage("./gfx/r/r4.png");
	m_mazeRunner5 = S2D_CreateImage("./gfx/r/r5.png");

	m_mazeRunnerDead0 = S2D_CreateImage("./gfx/r/d0.png");
	m_mazeRunnerDead1 = S2D_CreateImage("./gfx/r/d1.png");
	m_mazeRunnerDead2 = S2D_CreateImage("./gfx/r/d2.png");

	m_mazeRunner0_ = S2D_CreateImage("./gfx/r/r0_.png");
	m_mazeRunner1_ = S2D_CreateImage("./gfx/r/r1_.png");
	m_mazeRunner2_ = S2D_CreateImage("./gfx/r/r2_.png");
	m_mazeRunner3_ = S2D_CreateImage("./gfx/r/r3_.png");
	m_mazeRunner4_ = S2D_CreateImage("./gfx/r/r4_.png");
	m_mazeRunner5_ = S2D_CreateImage("./gfx/r/r5_.png");

	m_mazeRunnerDead0_ = S2D_CreateImage("./gfx/r/d0_.png");
	m_mazeRunnerDead1_ = S2D_CreateImage("./gfx/r/d1_.png");
	m_mazeRunnerDead2_ = S2D_CreateImage("./gfx/r/d2_.png");
	//////////////////////////////////////////////////////////////////////////

	// load text
	std::string strFont = "./msc/" + g_settings.textFont;
	m_text = S2D_CreateText(strFont.c_str(), "", 12);

	// text color
	m_text->color.r = g_settings.textColor.r;
	m_text->color.g = g_settings.textColor.g;
	m_text->color.b = g_settings.textColor.b;
	m_text->color.a = g_settings.textColor.a;
}

void CGfxRunner::ResizeSprites(int iWidth, int iHeight)
{
	// set dimensions
	m_iWidth = iWidth;
	m_iHeight = iHeight;

	// set width
	m_mazeRunner0->width = iWidth;
	m_mazeRunner1->width = iWidth;
	m_mazeRunner2->width = iWidth;
	m_mazeRunner3->width = iWidth;
	m_mazeRunner4->width = iWidth;
	m_mazeRunner5->width = iWidth;

	m_mazeRunnerDead0->width = iWidth;
	m_mazeRunnerDead1->width = iWidth;
	m_mazeRunnerDead2->width = iWidth;

	m_mazeRunner0_->width = iWidth;
	m_mazeRunner1_->width = iWidth;
	m_mazeRunner2_->width = iWidth;
	m_mazeRunner3_->width = iWidth;
	m_mazeRunner4_->width = iWidth;
	m_mazeRunner5_->width = iWidth;

	m_mazeRunnerDead0_->width = iWidth;
	m_mazeRunnerDead1_->width = iWidth;
	m_mazeRunnerDead2_->width = iWidth;

	// set height
	m_mazeRunner0->height = iHeight;
	m_mazeRunner1->height = iHeight;
	m_mazeRunner2->height = iHeight;
	m_mazeRunner3->height = iHeight;
	m_mazeRunner4->height = iHeight;
	m_mazeRunner5->height = iHeight;

	m_mazeRunnerDead0->height = iHeight;
	m_mazeRunnerDead1->height = iHeight;
	m_mazeRunnerDead2->height = iHeight;

	m_mazeRunner0_->height = iHeight;
	m_mazeRunner1_->height = iHeight;
	m_mazeRunner2_->height = iHeight;
	m_mazeRunner3_->height = iHeight;
	m_mazeRunner4_->height = iHeight;
	m_mazeRunner5_->height = iHeight;

	m_mazeRunnerDead0_->height = iHeight;
	m_mazeRunnerDead1_->height = iHeight;
	m_mazeRunnerDead2_->height = iHeight;
}

void CGfxRunner::Draw(int x, int y, int iDirection, int iState)
{
	if (iState == CAgent::DEAD)
	{
		DrawDead(x, y, iDirection);
	}

	if (iState == CAgent::ALIVE)
	{
		DrawAlive(x, y, iDirection);
	}
}

void CGfxRunner::DrawInfo(int x, int y, void* pInfo)
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

void CGfxRunner::DrawDead(int x, int y, int iDirection)
{
	// 1/3 counter
	int iCounter0 = m_iFrameCounter0 / 3;

	// right facing
	if (iDirection >= 0)
	{
		switch (iCounter0 % 3)
		{
		case 0:
			m_mazeRunnerDead0->x = x;
			m_mazeRunnerDead0->y = y;
			S2D_DrawImage(m_mazeRunnerDead0);
			break;
		case 1:
			m_mazeRunnerDead1->x = x;
			m_mazeRunnerDead1->y = y;
			S2D_DrawImage(m_mazeRunnerDead1);
			break;
		case 2:
			m_mazeRunnerDead2->x = x;
			m_mazeRunnerDead2->y = y;
			S2D_DrawImage(m_mazeRunnerDead2);
			break;
		default:
			break;
		}
	}
	// left facing
	else
	{
		switch (iCounter0 % 3)
		{
		case 0:
			m_mazeRunnerDead0_->x = x;
			m_mazeRunnerDead0_->y = y;
			S2D_DrawImage(m_mazeRunnerDead0_);
			break;
		case 1:
			m_mazeRunnerDead1_->x = x;
			m_mazeRunnerDead1_->y = y;
			S2D_DrawImage(m_mazeRunnerDead1_);
			break;
		case 2:
			m_mazeRunnerDead2_->x = x;
			m_mazeRunnerDead2_->y = y;
			S2D_DrawImage(m_mazeRunnerDead2_);
			break;
		default:
			break;
		}
	}

	if (m_Timer0.GetElapsedTime() > PERIOD_20_FPS)
	{
		m_iFrameCounter0++;
		m_Timer0.Start();
	}
}

void CGfxRunner::DrawAlive(int x, int y, int iDirection)
{
	// set the correct face direction
	if (iDirection == 0)
	{
		iDirection = m_iDirection;
	}

	m_iDirection = iDirection;

	// 1/3 counter
	int iCounter1 = m_iFrameCounter1 / 3;

	// right facing
	if (iDirection >= 0)
	{
		switch (iCounter1 % 6)
		{
		case 0:
			m_mazeRunner0->x = x;
			m_mazeRunner0->y = y;
			S2D_DrawImage(m_mazeRunner0);
			break;
		case 1:
			m_mazeRunner1->x = x;
			m_mazeRunner1->y = y;
			S2D_DrawImage(m_mazeRunner1);
			break;
		case 2:
			m_mazeRunner2->x = x;
			m_mazeRunner2->y = y;
			S2D_DrawImage(m_mazeRunner2);
			break;
		case 3:
			m_mazeRunner3->x = x;
			m_mazeRunner3->y = y;
			S2D_DrawImage(m_mazeRunner3);
			break;
		case 4:
			m_mazeRunner4->x = x;
			m_mazeRunner4->y = y;
			S2D_DrawImage(m_mazeRunner4);
			break;
		case 5:
			m_mazeRunner5->x = x;
			m_mazeRunner5->y = y;
			S2D_DrawImage(m_mazeRunner5);
			break;
		default:
			break;
		}
	}
	// left facing
	else
	{
		switch (iCounter1 % 6)
		{
		case 0:
			m_mazeRunner0_->x = x;
			m_mazeRunner0_->y = y;
			S2D_DrawImage(m_mazeRunner0_);
			break;
		case 1:
			m_mazeRunner1_->x = x;
			m_mazeRunner1_->y = y;
			S2D_DrawImage(m_mazeRunner1_);
			break;
		case 2:
			m_mazeRunner2_->x = x;
			m_mazeRunner2_->y = y;
			S2D_DrawImage(m_mazeRunner2_);
			break;
		case 3:
			m_mazeRunner3_->x = x;
			m_mazeRunner3_->y = y;
			S2D_DrawImage(m_mazeRunner3_);
			break;
		case 4:
			m_mazeRunner4_->x = x;
			m_mazeRunner4_->y = y;
			S2D_DrawImage(m_mazeRunner4_);
			break;
		case 5:
			m_mazeRunner5_->x = x;
			m_mazeRunner5_->y = y;
			S2D_DrawImage(m_mazeRunner5_);
			break;
		default:
			break;
		}
	}

	if (m_Timer1.GetElapsedTime() > PERIOD_30_FPS)
	{
		m_iFrameCounter1++;
		m_Timer1.Start();
	}
}
