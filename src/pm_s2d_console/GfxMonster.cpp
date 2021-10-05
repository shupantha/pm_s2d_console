#include "stdafx.h"
#include "GfxMonster.h"
#include "Settings.h"
#include "Agent.h"

// settings
extern CSettings g_settings;

// current level
extern unsigned int g_uiLevel;

CGfxMonster::CGfxMonster(int iID)
	: IGfxElement(iID)
{
	Initialize();

	m_iID = iID;
}

CGfxMonster::~CGfxMonster()
{
	// facing right
	//monster images
	S2D_FreeImage(m_monster0);
	S2D_FreeImage(m_monster1);
	S2D_FreeImage(m_monster2);
	S2D_FreeImage(m_monster3);
	S2D_FreeImage(m_monster4);
	S2D_FreeImage(m_monster5);

	// edible monsters
	S2D_FreeImage(m_monsterEdible0);
	S2D_FreeImage(m_monsterEdible1);
	S2D_FreeImage(m_monsterEdible2);
	S2D_FreeImage(m_monsterEdible3);
	S2D_FreeImage(m_monsterEdible4);
	S2D_FreeImage(m_monsterEdible5);

	// dead monsters
	S2D_FreeImage(m_monsterDead0);
	S2D_FreeImage(m_monsterDead1);
	S2D_FreeImage(m_monsterDead2);
	S2D_FreeImage(m_monsterDead3);
	S2D_FreeImage(m_monsterDead4);
	S2D_FreeImage(m_monsterDead5);

	// facing left
	// monster images
	S2D_FreeImage(m_monster0_);
	S2D_FreeImage(m_monster1_);
	S2D_FreeImage(m_monster2_);
	S2D_FreeImage(m_monster3_);
	S2D_FreeImage(m_monster4_);
	S2D_FreeImage(m_monster5_);

	// edible monsters
	S2D_FreeImage(m_monsterEdible0_);
	S2D_FreeImage(m_monsterEdible1_);
	S2D_FreeImage(m_monsterEdible2_);
	S2D_FreeImage(m_monsterEdible3_);
	S2D_FreeImage(m_monsterEdible4_);
	S2D_FreeImage(m_monsterEdible5_);

	// dead monsters
	S2D_FreeImage(m_monsterDead0_);
	S2D_FreeImage(m_monsterDead1_);
	S2D_FreeImage(m_monsterDead2_);
	S2D_FreeImage(m_monsterDead3_);
	S2D_FreeImage(m_monsterDead4_);
	S2D_FreeImage(m_monsterDead5_);

	S2D_FreeText(m_text);
}

void CGfxMonster::Initialize()
{
	// initialize members
	m_iFrameCounter0 = 0;
	m_iFrameCounter1 = 0;
	m_iFrameCounter2 = 0;

	m_iWidth = 0;
	m_iHeight = 0;

	m_iDirection = 1;

	// facing right
	//monster images
	m_monster0 = nullptr;
	m_monster1 = nullptr;
	m_monster2 = nullptr;
	m_monster3 = nullptr;
	m_monster4 = nullptr;
	m_monster5 = nullptr;

	// edible monsters
	m_monsterEdible0 = nullptr;
	m_monsterEdible1 = nullptr;
	m_monsterEdible2 = nullptr;
	m_monsterEdible3 = nullptr;
	m_monsterEdible4 = nullptr;
	m_monsterEdible5 = nullptr;

	// dead monsters
	m_monsterDead0 = nullptr;
	m_monsterDead1 = nullptr;
	m_monsterDead2 = nullptr;
	m_monsterDead3 = nullptr;
	m_monsterDead4 = nullptr;
	m_monsterDead5 = nullptr;

	// facing left
	// monster images
	m_monster0_ = nullptr;
	m_monster1_ = nullptr;
	m_monster2_ = nullptr;
	m_monster3_ = nullptr;
	m_monster4_ = nullptr;
	m_monster5_ = nullptr;

	// edible monsters
	m_monsterEdible0_ = nullptr;
	m_monsterEdible1_ = nullptr;
	m_monsterEdible2_ = nullptr;
	m_monsterEdible3_ = nullptr;
	m_monsterEdible4_ = nullptr;
	m_monsterEdible5_ = nullptr;

	// dead monsters
	m_monsterDead0_ = nullptr;
	m_monsterDead1_ = nullptr;
	m_monsterDead2_ = nullptr;
	m_monsterDead3_ = nullptr;
	m_monsterDead4_ = nullptr;
	m_monsterDead5_ = nullptr;

	// text
	m_text = nullptr;

	// start the timers
	m_Timer0.Start();
	m_Timer1.Start();
	m_Timer2.Start();
}

void CGfxMonster::Load()
{
	Initialize();

	//////////////////////////////////////////////////////////////////////////
	// Load all the external bitmaps
	//////////////////////////////////////////////////////////////////////////
	switch (m_iID)
	{
	case GINKY:	// ginky
		m_monster0 = S2D_CreateImage("./gfx/monsters/ginky0.png");
		m_monster1 = S2D_CreateImage("./gfx/monsters/ginky1.png");
		m_monster2 = S2D_CreateImage("./gfx/monsters/ginky2.png");
		m_monster3 = S2D_CreateImage("./gfx/monsters/ginky3.png");
		m_monster4 = S2D_CreateImage("./gfx/monsters/ginky4.png");
		m_monster5 = S2D_CreateImage("./gfx/monsters/ginky5.png");

		m_monster0_ = S2D_CreateImage("./gfx/monsters/ginky0_.png");
		m_monster1_ = S2D_CreateImage("./gfx/monsters/ginky1_.png");
		m_monster2_ = S2D_CreateImage("./gfx/monsters/ginky2_.png");
		m_monster3_ = S2D_CreateImage("./gfx/monsters/ginky3_.png");
		m_monster4_ = S2D_CreateImage("./gfx/monsters/ginky4_.png");
		m_monster5_ = S2D_CreateImage("./gfx/monsters/ginky5_.png");
		break;

	case BINKY:	// binky
		m_monster0 = S2D_CreateImage("./gfx/monsters/binky0.png");
		m_monster1 = S2D_CreateImage("./gfx/monsters/binky1.png");
		m_monster2 = S2D_CreateImage("./gfx/monsters/binky2.png");
		m_monster3 = S2D_CreateImage("./gfx/monsters/binky3.png");
		m_monster4 = S2D_CreateImage("./gfx/monsters/binky4.png");
		m_monster5 = S2D_CreateImage("./gfx/monsters/binky5.png");

		m_monster0_ = S2D_CreateImage("./gfx/monsters/binky0_.png");
		m_monster1_ = S2D_CreateImage("./gfx/monsters/binky1_.png");
		m_monster2_ = S2D_CreateImage("./gfx/monsters/binky2_.png");
		m_monster3_ = S2D_CreateImage("./gfx/monsters/binky3_.png");
		m_monster4_ = S2D_CreateImage("./gfx/monsters/binky4_.png");
		m_monster5_ = S2D_CreateImage("./gfx/monsters/binky5_.png");
		break;

	case YINKY:	// yinky
		m_monster0 = S2D_CreateImage("./gfx/monsters/yinky0.png");
		m_monster1 = S2D_CreateImage("./gfx/monsters/yinky1.png");
		m_monster2 = S2D_CreateImage("./gfx/monsters/yinky2.png");
		m_monster3 = S2D_CreateImage("./gfx/monsters/yinky3.png");
		m_monster4 = S2D_CreateImage("./gfx/monsters/yinky4.png");
		m_monster5 = S2D_CreateImage("./gfx/monsters/yinky5.png");

		m_monster0_ = S2D_CreateImage("./gfx/monsters/yinky0_.png");
		m_monster1_ = S2D_CreateImage("./gfx/monsters/yinky1_.png");
		m_monster2_ = S2D_CreateImage("./gfx/monsters/yinky2_.png");
		m_monster3_ = S2D_CreateImage("./gfx/monsters/yinky3_.png");
		m_monster4_ = S2D_CreateImage("./gfx/monsters/yinky4_.png");
		m_monster5_ = S2D_CreateImage("./gfx/monsters/yinky5_.png");
		break;

	case RINKY:	// rinky
		m_monster0 = S2D_CreateImage("./gfx/monsters/rinky0.png");
		m_monster1 = S2D_CreateImage("./gfx/monsters/rinky1.png");
		m_monster2 = S2D_CreateImage("./gfx/monsters/rinky2.png");
		m_monster3 = S2D_CreateImage("./gfx/monsters/rinky3.png");
		m_monster4 = S2D_CreateImage("./gfx/monsters/rinky4.png");
		m_monster5 = S2D_CreateImage("./gfx/monsters/rinky5.png");

		m_monster0_ = S2D_CreateImage("./gfx/monsters/rinky0_.png");
		m_monster1_ = S2D_CreateImage("./gfx/monsters/rinky1_.png");
		m_monster2_ = S2D_CreateImage("./gfx/monsters/rinky2_.png");
		m_monster3_ = S2D_CreateImage("./gfx/monsters/rinky3_.png");
		m_monster4_ = S2D_CreateImage("./gfx/monsters/rinky4_.png");
		m_monster5_ = S2D_CreateImage("./gfx/monsters/rinky5_.png");
		break;

	default:
		break;
	}

	m_monsterEdible0 = S2D_CreateImage("./gfx/monsters/edible0.png");
	m_monsterEdible1 = S2D_CreateImage("./gfx/monsters/edible1.png");
	m_monsterEdible2 = S2D_CreateImage("./gfx/monsters/edible2.png");
	m_monsterEdible3 = S2D_CreateImage("./gfx/monsters/edible3.png");
	m_monsterEdible4 = S2D_CreateImage("./gfx/monsters/edible4.png");
	m_monsterEdible5 = S2D_CreateImage("./gfx/monsters/edible5.png");

	m_monsterEdible0_ = S2D_CreateImage("./gfx/monsters/edible0_.png");
	m_monsterEdible1_ = S2D_CreateImage("./gfx/monsters/edible1_.png");
	m_monsterEdible2_ = S2D_CreateImage("./gfx/monsters/edible2_.png");
	m_monsterEdible3_ = S2D_CreateImage("./gfx/monsters/edible3_.png");
	m_monsterEdible4_ = S2D_CreateImage("./gfx/monsters/edible4_.png");
	m_monsterEdible5_ = S2D_CreateImage("./gfx/monsters/edible5_.png");

	m_monsterDead0 = S2D_CreateImage("./gfx/monsters/dead0.png");
	m_monsterDead1 = S2D_CreateImage("./gfx/monsters/dead1.png");
	m_monsterDead2 = S2D_CreateImage("./gfx/monsters/dead2.png");
	m_monsterDead3 = S2D_CreateImage("./gfx/monsters/dead3.png");
	m_monsterDead4 = S2D_CreateImage("./gfx/monsters/dead4.png");
	m_monsterDead5 = S2D_CreateImage("./gfx/monsters/dead5.png");

	m_monsterDead0_ = S2D_CreateImage("./gfx/monsters/dead0_.png");
	m_monsterDead1_ = S2D_CreateImage("./gfx/monsters/dead1_.png");
	m_monsterDead2_ = S2D_CreateImage("./gfx/monsters/dead2_.png");
	m_monsterDead3_ = S2D_CreateImage("./gfx/monsters/dead3_.png");
	m_monsterDead4_ = S2D_CreateImage("./gfx/monsters/dead4_.png");
	m_monsterDead5_ = S2D_CreateImage("./gfx/monsters/dead5_.png");
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

void CGfxMonster::ResizeSprites(int iWidth, int iHeight)
{
	// set dimensions
	m_iWidth = iWidth;
	m_iHeight = iHeight;

	// set width
	m_monster0->width = iWidth;
	m_monster1->width = iWidth;
	m_monster2->width = iWidth;
	m_monster3->width = iWidth;
	m_monster4->width = iWidth;
	m_monster5->width = iWidth;

	m_monsterEdible0->width = iWidth;
	m_monsterEdible1->width = iWidth;
	m_monsterEdible2->width = iWidth;
	m_monsterEdible3->width = iWidth;
	m_monsterEdible4->width = iWidth;
	m_monsterEdible5->width = iWidth;

	m_monsterDead0->width = iWidth;
	m_monsterDead1->width = iWidth;
	m_monsterDead2->width = iWidth;
	m_monsterDead3->width = iWidth;
	m_monsterDead4->width = iWidth;
	m_monsterDead5->width = iWidth;

	m_monster0_->width = iWidth;
	m_monster1_->width = iWidth;
	m_monster2_->width = iWidth;
	m_monster3_->width = iWidth;
	m_monster4_->width = iWidth;
	m_monster5_->width = iWidth;

	m_monsterEdible0_->width = iWidth;
	m_monsterEdible1_->width = iWidth;
	m_monsterEdible2_->width = iWidth;
	m_monsterEdible3_->width = iWidth;
	m_monsterEdible4_->width = iWidth;
	m_monsterEdible5_->width = iWidth;

	m_monsterDead0_->width = iWidth;
	m_monsterDead1_->width = iWidth;
	m_monsterDead2_->width = iWidth;
	m_monsterDead3_->width = iWidth;
	m_monsterDead4_->width = iWidth;
	m_monsterDead5_->width = iWidth;

	// set height
	m_monster0->height = iHeight;
	m_monster1->height = iHeight;
	m_monster2->height = iHeight;
	m_monster3->height = iHeight;
	m_monster4->height = iHeight;
	m_monster5->height = iHeight;

	m_monsterEdible0->height = iHeight;
	m_monsterEdible1->height = iHeight;
	m_monsterEdible2->height = iHeight;
	m_monsterEdible3->height = iHeight;
	m_monsterEdible4->height = iHeight;
	m_monsterEdible5->height = iHeight;

	m_monsterDead0->height = iHeight;
	m_monsterDead1->height = iHeight;
	m_monsterDead2->height = iHeight;
	m_monsterDead3->height = iHeight;
	m_monsterDead4->height = iHeight;
	m_monsterDead5->height = iHeight;

	m_monster0_->height = iHeight;
	m_monster1_->height = iHeight;
	m_monster2_->height = iHeight;
	m_monster3_->height = iHeight;
	m_monster4_->height = iHeight;
	m_monster5_->height = iHeight;

	m_monsterEdible0_->height = iHeight;
	m_monsterEdible1_->height = iHeight;
	m_monsterEdible2_->height = iHeight;
	m_monsterEdible3_->height = iHeight;
	m_monsterEdible4_->height = iHeight;
	m_monsterEdible5_->height = iHeight;

	m_monsterDead0_->height = iHeight;
	m_monsterDead1_->height = iHeight;
	m_monsterDead2_->height = iHeight;
	m_monsterDead3_->height = iHeight;
	m_monsterDead4_->height = iHeight;
	m_monsterDead5_->height = iHeight;
}

void CGfxMonster::Draw(int x, int y, int iDirection, int iState)
{
	if (iState == CAgent::DEAD)
	{
		DrawDead(x, y, iDirection);
	}

	if (iState == CAgent::VULNERABLE)
	{
		DrawEdible(x, y, iDirection);
	}

	if (iState == CAgent::ALIVE)
	{
		DrawAlive(x, y, iDirection);
	}
}

void CGfxMonster::DrawInfo(int x, int y, void* pInfo)
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

void CGfxMonster::DrawDead(int x, int y, int iDirection)
{
	// set the correct face direction
	if (iDirection == 0)
	{
		iDirection = m_iDirection;
	}

	m_iDirection = iDirection;

	// 1/3 counter
	int iCounter0 = m_iFrameCounter0 / 3;

	if (iDirection >= 0)
	{
		switch (iCounter0 % 6)
		{
		case 0:
			m_monsterDead0->x = x;
			m_monsterDead0->y = y;
			S2D_DrawImage(m_monsterDead0);
			break;
		case 1:
			m_monsterDead1->x = x;
			m_monsterDead1->y = y;
			S2D_DrawImage(m_monsterDead1);
			break;
		case 2:
			m_monsterDead2->x = x;
			m_monsterDead2->y = y;
			S2D_DrawImage(m_monsterDead2);
			break;
		case 3:
			m_monsterDead3->x = x;
			m_monsterDead3->y = y;
			S2D_DrawImage(m_monsterDead3);
			break;
		case 4:
			m_monsterDead4->x = x;
			m_monsterDead4->y = y;
			S2D_DrawImage(m_monsterDead4);
			break;
		case 5:
			m_monsterDead5->x = x;
			m_monsterDead5->y = y;
			S2D_DrawImage(m_monsterDead5);
			break;
		default:
			break;
		}
	}
	else
	{
		switch (iCounter0 % 6)
		{
		case 0:
			m_monsterDead0_->x = x;
			m_monsterDead0_->y = y;
			S2D_DrawImage(m_monsterDead0_);
			break;
		case 1:
			m_monsterDead1_->x = x;
			m_monsterDead1_->y = y;
			S2D_DrawImage(m_monsterDead1_);
			break;
		case 2:
			m_monsterDead2_->x = x;
			m_monsterDead2_->y = y;
			S2D_DrawImage(m_monsterDead2_);
			break;
		case 3:
			m_monsterDead3_->x = x;
			m_monsterDead3_->y = y;
			S2D_DrawImage(m_monsterDead3_);
			break;
		case 4:
			m_monsterDead4_->x = x;
			m_monsterDead4_->y = y;
			S2D_DrawImage(m_monsterDead4_);
			break;
		case 5:
			m_monsterDead5_->x = x;
			m_monsterDead5_->y = y;
			S2D_DrawImage(m_monsterDead5_);
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

void CGfxMonster::DrawEdible(int x, int y, int iDirection)
{
	// set the correct face direction
	if (iDirection == 0)
	{
		iDirection = m_iDirection;
	}

	m_iDirection = iDirection;

	// 1/3 counter
	int iCounter1 = m_iFrameCounter1 / 3;

	if (iDirection >= 0)
	{
		switch (iCounter1 % 6)
		{
		case 0:
			m_monsterEdible0->x = x;
			m_monsterEdible0->y = y;
			S2D_DrawImage(m_monsterEdible0);
			break;
		case 1:
			m_monsterEdible1->x = x;
			m_monsterEdible1->y = y;
			S2D_DrawImage(m_monsterEdible1);
			break;
		case 2:
			m_monsterEdible2->x = x;
			m_monsterEdible2->y = y;
			S2D_DrawImage(m_monsterEdible2);
			break;
		case 3:
			m_monsterEdible3->x = x;
			m_monsterEdible3->y = y;
			S2D_DrawImage(m_monsterEdible3);
			break;
		case 4:
			m_monsterEdible4->x = x;
			m_monsterEdible4->y = y;
			S2D_DrawImage(m_monsterEdible4);
			break;
		case 5:
			m_monsterEdible5->x = x;
			m_monsterEdible5->y = y;
			S2D_DrawImage(m_monsterEdible5);
			break;
		default:
			break;
		}
	}
	else
	{
		switch (iCounter1 % 6)
		{
		case 0:
			m_monsterEdible0_->x = x;
			m_monsterEdible0_->y = y;
			S2D_DrawImage(m_monsterEdible0_);
			break;
		case 1:
			m_monsterEdible1_->x = x;
			m_monsterEdible1_->y = y;
			S2D_DrawImage(m_monsterEdible1_);
			break;
		case 2:
			m_monsterEdible2_->x = x;
			m_monsterEdible2_->y = y;
			S2D_DrawImage(m_monsterEdible2_);
			break;
		case 3:
			m_monsterEdible3_->x = x;
			m_monsterEdible3_->y = y;
			S2D_DrawImage(m_monsterEdible3_);
			break;
		case 4:
			m_monsterEdible4_->x = x;
			m_monsterEdible4_->y = y;
			S2D_DrawImage(m_monsterEdible4_);
			break;
		case 5:
			m_monsterEdible5_->x = x;
			m_monsterEdible5_->y = y;
			S2D_DrawImage(m_monsterEdible5_);
			break;
		default:
			break;
		}
	}

	if (m_Timer1.GetElapsedTime() > PERIOD_20_FPS)
	{
		m_iFrameCounter1++;
		m_Timer1.Start();
	}
}

void CGfxMonster::DrawAlive(int x, int y, int iDirection)
{
	// set the correct face direction
	if (iDirection == 0)
	{
		iDirection = m_iDirection;
	}

	m_iDirection = iDirection;

	// 1/3 counter
	int iCounter2 = m_iFrameCounter2 / 3;

	if (iDirection >= 0)
	{
		switch (iCounter2 % 6)
		{
		case 0:
			m_monster0->x = x;
			m_monster0->y = y;
			S2D_DrawImage(m_monster0);
			break;
		case 1:
			m_monster1->x = x;
			m_monster1->y = y;
			S2D_DrawImage(m_monster1);
			break;
		case 2:
			m_monster2->x = x;
			m_monster2->y = y;
			S2D_DrawImage(m_monster2);
			break;
		case 3:
			m_monster3->x = x;
			m_monster3->y = y;
			S2D_DrawImage(m_monster3);
			break;
		case 4:
			m_monster4->x = x;
			m_monster4->y = y;
			S2D_DrawImage(m_monster4);
			break;
		case 5:
			m_monster5->x = x;
			m_monster5->y = y;
			S2D_DrawImage(m_monster5);
			break;
		default:
			break;
		}
	}
	else
	{
		switch (iCounter2 % 6)
		{
		case 0:
			m_monster0_->x = x;
			m_monster0_->y = y;
			S2D_DrawImage(m_monster0_);
			break;
		case 1:
			m_monster1_->x = x;
			m_monster1_->y = y;
			S2D_DrawImage(m_monster1_);
			break;
		case 2:
			m_monster1_->x = x;
			m_monster1_->y = y;
			S2D_DrawImage(m_monster1_);
			break;
		case 3:
			m_monster3_->x = x;
			m_monster3_->y = y;
			S2D_DrawImage(m_monster3_);
			break;
		case 4:
			m_monster4_->x = x;
			m_monster4_->y = y;
			S2D_DrawImage(m_monster4_);
			break;
		case 5:
			m_monster5_->x = x;
			m_monster5_->y = y;
			S2D_DrawImage(m_monster5_);
			break;
		default:
			break;
		}
	}

	if (m_Timer2.GetElapsedTime() > PERIOD_20_FPS)
	{
		m_iFrameCounter2++;
		m_Timer2.Start();
	}
}
