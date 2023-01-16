#pragma once

#include "Util.h"
#include "AStar.h"
#include "Mazes.h"
#include "Agent.h"

#include "SfxManager.h"

#include "GfxWorld.h"

#include <thread>

#pragma warning(disable : 26812)

class CGameWorld
{
public:
	enum DIFFICULTY
	{
		EASY,
		MEDIUM,
		HARD
	};

public:
	CGameWorld();
	~CGameWorld();

private:
	void Initialize();

public:
	bool IsReady();

private:
	void SetInitialLocations();

	void Spawn(int iAgentIndex);

	void SavePillLocations(string strFilePath);

public:
	CAgent& GetAgent(int iAgentIndex);

	void SetMonsterTarget(int iAgentIndex);

	void SetBonusEffects(CAgent& agent);

	void SetRunnerSpeed(CAgent& agent);

	bool MoveAgent(int iAgentIndex);

	void MoveAgents();

	void HandleCollisions();

	bool IsExitVisible();

	vector<CAgent*> CollisionCheck();

	void HandleMonsterCollision(CAgent* pAgent);
	void HandleDoorCollision(CAgent* pAgent);
	void HandlePillCollision(CAgent* pAgent);

	void UpdateStates();

	void LevelComplete();

public:
	void LoadLevel(unsigned int uiLevel, S2D_Window* pGameWindow);

public:
	void Draw(bool bgOnly, bool bNoExtras);

public:
	void InitializeGameWorld();

private:
	bool Load(int iWidth, int iHeight, int iCellSize);

	bool LoadMaze(const unsigned char* pMaze, int iMazeSizeX, int iMazeSizeY);

private:
	CGfxWorld m_gfxWorld;

	CSfxManager m_sfx;

private:
	// location data for all agents, both maze and screen coordinates
	vector<CAgent> m_vAgents;

	// for collision testing for pills, in maze coordinates, NOT screen coordinates
	map<_Point, CAgent*> m_mapPillLocations;

private:
	CAStar m_Maze;

	mazes::mazeGenerator m_mg;

private:
	bool m_bIsDemo;
	bool m_bIsCheatActive;

	// Easy:   god mode, stupid monsters; goal: find exit
	// Medium: god mode, faster runner, stupid monsters; goal: eat all fruits, and find exit
	// Hard:   normal game play; goal: stay alive, and find exit
	DIFFICULTY m_eDifficulty;

	bool m_bLevelComplete;

	unsigned int m_uiRunnerSpawnTime;	// in milliseconds
	bool m_bGameOver;

public:
	void ToggleDemoMode();
	void SetDemo(bool bIsDemo);

	void ToggleCheat() { m_bIsCheatActive = !m_bIsCheatActive; };
	bool IsCheatActive() { return m_bIsCheatActive; };

	void SetEasy() { m_eDifficulty = DIFFICULTY::EASY; };
	void SetMedium() { m_eDifficulty = DIFFICULTY::MEDIUM; };
	void SetHard() { m_eDifficulty = DIFFICULTY::HARD; };
	
	void SetDifficulty(DIFFICULTY eDifficulty) { m_eDifficulty = eDifficulty; };
	DIFFICULTY GetDifficulty() { return m_eDifficulty; };

	std::string GetDifficulty(DIFFICULTY eDifficulty);

	bool IsLevelComplete() { return m_bLevelComplete; };

	unsigned int GetSpawnTime() { return m_uiRunnerSpawnTime; };

	bool IsGameOver() { return m_bGameOver; };

	void ToggleSound() { m_sfx.ToggleSound(); m_sfx.PlayBGMusic(m_uiLevel); };
	void EnableSound(bool bEnableSound) { m_sfx.Mute(!bEnableSound); m_sfx.PlayBGMusic(m_uiLevel); };

private:
	// total score
	unsigned int m_uiScore;

	// lives remaining
	unsigned int m_uiLives;

	// total lives in game session
	unsigned int m_uiTotalLives;

	// current level
	unsigned int m_uiLevel;

	// bonuses: orange fruit - % permanent speed bonus
	bool m_bSpeedBoost;

	// bonuses: kiwi fruit - % extra power pill time
	bool m_bTimeBoost;

	// bonuses: mango fruit - kill 1 or 2 random monsters permanently
	bool m_bPermaKill;

	// bonuses: dragon fruit - show path to exit for a set amount of time
	std::vector<_Point> m_vptPath;
	bool m_bShowPathToExit;

	CTimer m_timerDragon;

private:
	void AddScore(unsigned int uiScore) { m_uiScore += uiScore; UpdateLives(); };
	void UpdateLives();

public:
	unsigned int GetScore() { return m_uiScore; };
	unsigned int GetLives() { return m_uiLives; };
	unsigned int GetTotalLives() { return m_uiTotalLives; };
	unsigned int GetLevel() { return m_uiLevel; };

	void SetSavedData(unsigned int uiScore, unsigned int uiLives, unsigned int uiTotalLives, unsigned int uiLevel);

	_tstring GetPMDirectory();

public:
	int GetCellSize() { return m_mg.getCellSize(); };

	int GetMazeWidth() { return m_mg.getMazeSizeX(); };
	int GetMazeHeight() { return m_mg.getMazeSizeY(); };

	int GetWidth() { return m_mg.getWidth(); };
	int GetHeight() { return m_mg.getHeight(); };

private:
	bool m_bIsRunning;
	bool m_bPause;

public:
	bool IsRunning() { return m_bIsRunning; };

	void TogglePause() { m_bPause = !m_bPause; };
	void Pause() { m_bPause = true; };
	void Resume() { m_bPause = false; };

	bool IsPaused() { return m_bPause; };

public:
	bool Move(CAgent::directions direction);
	bool Move(AGENT_ID aID, _Point ptGrid);

private:
	bool IsIntersection(_Point pt, CAgent::directions direction);
};
