#include "stdafx.h"
#include "SfxManager.h"

CSfxManager::CSfxManager()
{
	Load();

	m_bMute = false;
}

CSfxManager::~CSfxManager()
{
	m_bMute = true;

	Unload();
}

void CSfxManager::Load()
{
	m_pStartLevel = S2D_CreateSound("./sfx/startlevel.wav");
	m_pEndLevel = S2D_CreateSound("./sfx/endlevel.wav");
	m_pGameOver = S2D_CreateSound("./sfx/gameover.wav");
	m_pEatPill = S2D_CreateSound("./sfx/eatpill.wav");
	m_pEatPower = S2D_CreateSound("./sfx/eatpower.wav");
	m_pEatOrange = S2D_CreateSound("./sfx/eatorange.wav");
	m_pEatKiwi = S2D_CreateSound("./sfx/eatkiwi.wav");
	m_pEatMango = S2D_CreateSound("./sfx/eatmango.wav");
	m_pEatDragon = S2D_CreateSound("./sfx/eatdragon.wav");
	m_pEatMonster = S2D_CreateSound("./sfx/eatmonster.wav");
	m_pKillRunner = S2D_CreateSound("./sfx/killrunner.wav");

	m_pMusic1 = S2D_CreateMusic("./sfx/bg01.WAV");
	m_pMusic2 = S2D_CreateMusic("./sfx/bg02.WAV");
	m_pMusic3 = S2D_CreateMusic("./sfx/bg03.WAV");
	m_pMusic4 = S2D_CreateMusic("./sfx/bg04.WAV");
	m_pMusic5 = S2D_CreateMusic("./sfx/bg05.WAV");

	m_pCredits = S2D_CreateMusic("./sfx/credits.WAV");
}

void CSfxManager::Unload()
{
	S2D_FreeSound(m_pStartLevel);
	S2D_FreeSound(m_pEndLevel);
	S2D_FreeSound(m_pGameOver);
	S2D_FreeSound(m_pEatPill);
	S2D_FreeSound(m_pEatPower);
	S2D_FreeSound(m_pEatOrange);
	S2D_FreeSound(m_pEatKiwi);
	S2D_FreeSound(m_pEatMango);
	S2D_FreeSound(m_pEatDragon);
	S2D_FreeSound(m_pEatMonster);
	S2D_FreeSound(m_pKillRunner);

	S2D_StopMusic();

	S2D_FreeMusic(m_pMusic1);
	S2D_FreeMusic(m_pMusic2);
	S2D_FreeMusic(m_pMusic3);
	S2D_FreeMusic(m_pMusic4);
	S2D_FreeMusic(m_pMusic5);

	S2D_FreeMusic(m_pCredits);
}

void CSfxManager::Play(unsigned int uiSoundID)
{
	if (m_bMute)
	{
		return;
	}

	if (uiSoundID >= SFX::SFX_COUNT)
	{
		return;
	}

	switch (uiSoundID)
	{
	case SFX::SFX_START_LEVEL:
		S2D_PlaySound(m_pStartLevel);
		break;
	case SFX::SFX_END_LEVEL:
		S2D_PlaySound(m_pEndLevel);
		break;
	case SFX::SFX_GAME_OVER:
		S2D_PlaySound(m_pGameOver);
		break;
	case SFX::SFX_EAT_PILL:
		S2D_PlaySound(m_pEatPill);
		break;
	case SFX::SFX_EAT_POWER:
		S2D_PlaySound(m_pEatPower);
		break;
	case SFX::SFX_EAT_ORANGE:
		S2D_PlaySound(m_pEatOrange);
		break;
	case SFX::SFX_EAT_KIWI:
		S2D_PlaySound(m_pEatKiwi);
		break;
	case SFX::SFX_EAT_MANGO:
		S2D_PlaySound(m_pEatMango);
		break;
	case SFX::SFX_EAT_DRAGON:
		S2D_PlaySound(m_pEatDragon);
		break;
	case SFX::SFX_EAT_MONSTER:
		S2D_PlaySound(m_pEatMonster);
		break;
	case SFX::SFX_KILL_RUNNER:
		S2D_PlaySound(m_pKillRunner);
		break;
	default:
		break;
	}
}

void CSfxManager::PlayBGMusic(unsigned int uiLevel)
{
	// check
	if (IsMute() || uiLevel == 0)
	{
		S2D_FadeOutMusic(1000);
		return;
	}

	// play background music
	int iBGMusicIndex = ((uiLevel - 1) % 6);
	switch (iBGMusicIndex)
	{
	case 0:
		S2D_PlayMusic(m_pMusic1, true);	// play on a loop
		break;
	case 1:
		S2D_PlayMusic(m_pMusic2, true);	// play on a loop
		break;
	case 2:
		S2D_PlayMusic(m_pMusic3, true);	// play on a loop
		break;
	case 3:
		S2D_PlayMusic(m_pMusic4, true);	// play on a loop
		break;
	case 4:
		S2D_PlayMusic(m_pMusic5, true);	// play on a loop
		break;
	case 5:
		S2D_PlayMusic(m_pCredits, true);	// play on a loop
		break;
	default:
		S2D_FadeOutMusic(1000);
		break;
	}
}

void CSfxManager::ToggleSound()
{
	Mute(!m_bMute);
}

void CSfxManager::Mute(bool bMute)
{
	m_bMute = bMute;

	PlayBGMusic(0);
}
