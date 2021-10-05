#pragma once

#include "..\simple2d\simple2d.h"

class CSfxManager
{
public:
	enum SFX
	{
		SFX_START_LEVEL	= 0,
		SFX_END_LEVEL,
		SFX_GAME_OVER,
		SFX_EAT_PILL,
		SFX_EAT_POWER,
		SFX_EAT_ORANGE,
		SFX_EAT_KIWI,
		SFX_EAT_MANGO,
		SFX_EAT_DRAGON,
		SFX_EAT_MONSTER,
		SFX_KILL_RUNNER,
		SFX_COUNT
	};

public:
	CSfxManager();
	~CSfxManager();

private:
	void Load();
	void Unload();

public:
	void Play(unsigned int uiSoundID);

	void PlayBGMusic(unsigned int uiLevel);

	void ToggleSound();
	void Mute(bool bMute);

	bool IsMute() { return m_bMute; };

private:
	bool m_bMute;

	S2D_Sound* m_pStartLevel;
	S2D_Sound* m_pEndLevel;
	S2D_Sound* m_pGameOver;
	S2D_Sound* m_pEatPill;
	S2D_Sound* m_pEatPower;
	S2D_Sound* m_pEatOrange;
	S2D_Sound* m_pEatKiwi;
	S2D_Sound* m_pEatMango;
	S2D_Sound* m_pEatDragon;
	S2D_Sound* m_pEatMonster;
	S2D_Sound* m_pKillRunner;

	S2D_Music* m_pMusic1;
	S2D_Music* m_pMusic2;
	S2D_Music* m_pMusic3;
	S2D_Music* m_pMusic4;
	S2D_Music* m_pMusic5;

	S2D_Music* m_pCredits;
};
