#include "stdafx.h"
#include "GameWorld.h"
#include "Settings.h"

#include "Timer.h"
#include <algorithm>

#ifdef min
#undef min
#endif 

#pragma warning(disable : 26812)

////////
// settings
extern CSettings g_settings;

// current level
unsigned int g_uiLevel = 0;

// bonuses: orange fruit - % permanent speed bonus
bool g_bSpeedBoost = false;

// bonuses: kiwi fruit - % extra power pill time
bool g_bTimeBoost = false;

// bonuses: mango fruit - kill 1 or 2 random monsters permanently
bool g_bPermaKill = false;

// bonuses: dragon fruit - show path to exit for a set amount of time
bool g_bShowPathToExit = false;

// cheat: click - show path
bool g_bShowPathToClick = false;

// lazy hack on fruit pills
bool IsAteAllFruits()
{
	return (g_bSpeedBoost && g_bTimeBoost && g_bPermaKill && g_bShowPathToExit);
}

CGameWorld::CGameWorld()
{
	Initialize();
}

CGameWorld::~CGameWorld()
{
	Initialize();
}

void CGameWorld::Initialize()
{
	m_bIsDemo = false;
	m_bIsCheatActive = false;

	m_eDifficulty = DIFFICULTY::EASY;

	m_bLevelComplete = false;

	m_uiRunnerSpawnTime = 0;

	m_bGameOver = false;

	m_uiScore = 0;
	
	m_uiLives = START_LIVES;
	m_uiTotalLives = START_LIVES;

	m_uiLevel = MIN_LEVEL;
	g_uiLevel = MIN_LEVEL;

	m_bSpeedBoost = false;
	m_bTimeBoost = false;
	m_bPermaKill = false;
	m_bShowPathToExit = false;

	g_bSpeedBoost = false;
	g_bTimeBoost = false;
	g_bPermaKill = false;
	g_bShowPathToExit = false;
	g_bShowPathToClick = false;

	m_bIsRunning = false;
	m_bPause = false;

	srand((unsigned int)GetTickCount64());
}

bool CGameWorld::IsReady()
{
	return m_Maze.IsReady();
}

void CGameWorld::ToggleDemoMode()
{
	SetDemo(!m_bIsDemo);
}

void CGameWorld::SetDemo(bool bIsDemo)
{
	if (!m_bIsRunning)
	{
		return;
	}

	// set
	m_bIsDemo = bIsDemo;

	// locations for the runner
	_Point ptCurrent = GetAgent(RUNNER).GetCurrentLocation();
	_Point ptNext = GetAgent(RUNNER).GetNextLocation();
	_Point ptTarget = GetAgent(EXIT).GetCurrentLocation();

	// set the new runner positions
	GetAgent(RUNNER).SetLocations(ptCurrent, ptNext);
	GetAgent(RUNNER).SetTarget(ptTarget);
	GetAgent(RUNNER).Move(GetCellSize(), 0.0);
	GetAgent(RUNNER).find() = true;

	// in non-demo / player mode, the runner stops moving automatically
	if (!bIsDemo)
	{
		for (int i = 0; i < CAgent::dCOUNT; ++i)
		{
			if (Move((CAgent::directions) i))
			{
				break;
			}
		}
	}
}

std::string CGameWorld::GetDifficulty(CGameWorld::DIFFICULTY eDifficulty)
{
	switch (eDifficulty)
	{
	case DIFFICULTY::EASY:
		return std::string("Easy");
		break;
	case DIFFICULTY::MEDIUM:
		return std::string("Medium");
		break;
	case DIFFICULTY::HARD:
		return std::string("Hard");
		break;
	default:
		return std::string("");
		break;
	}
	return std::string("");
}

void CGameWorld::UpdateLives()
{
	unsigned int uiRequiredScore = (m_uiTotalLives * m_uiLevel * 2500);

	if (m_uiScore >= uiRequiredScore)
	{
		m_uiTotalLives++;
		m_uiLives++;
	}
}

void CGameWorld::SetSavedData(unsigned int uiScore, unsigned int uiLives, unsigned int uiTotalLives, unsigned int uiLevel)
{
	m_uiScore = uiScore;
	m_uiLives = uiLives;
	m_uiTotalLives = uiTotalLives;
	m_uiLevel = uiLevel;
	g_uiLevel = uiLevel;
}

_tstring CGameWorld::GetPMDirectory()
{
	// create the maze file path
	_tstring strPMDirectory = CUtil::getAppDataDirectory();
	strPMDirectory = CUtil::addDirectoryEnding(strPMDirectory);
	strPMDirectory += _T("PM");
	strPMDirectory = CUtil::addDirectoryEnding(strPMDirectory);

	if (!CUtil::isDirectory(strPMDirectory))
	{
		CUtil::makeDirectory(strPMDirectory);
	}

	return strPMDirectory;
}

void CGameWorld::SetInitialLocations()
{
	for (int i = 0; i < (int)m_vAgents.size(); ++i)
	{
		if (i == ENTRANCE)
		{
			// doors
			_Point ptEntrance(1, 1);

			GetAgent(i).SetLocations(ptEntrance, ptEntrance);
			GetAgent(i).SetTarget(ptEntrance);

			m_Maze.SetMapWalkability(ptEntrance, CAStar::OCCUPIED);

			GetAgent(i).Move(GetCellSize(), 0.0);
		}

		if (i == EXIT)
		{
			// doors
			_Point ptExit(m_Maze.GetWidth() - 2, m_Maze.GetHeight() - 2);

			if (m_eDifficulty == DIFFICULTY::MEDIUM)
			{
				// random exit location
				ptExit = m_Maze.GetLocationOdd();
			}

			GetAgent(i).SetLocations(ptExit, ptExit);
			GetAgent(i).SetTarget(ptExit);

			m_Maze.SetMapWalkability(ptExit, CAStar::OCCUPIED);

			GetAgent(i).Move(GetCellSize(), 0.0);
		}

		if (i == ORANGE || i == KIWI || i == MANGO || i == DRAGON)
		{
			// bonus pills
			_Point ptCurrent = m_Maze.GetLocationOdd(CAStar::WALKABLE);

			GetAgent(i).SetLocations(ptCurrent, ptCurrent);
			GetAgent(i).SetTarget(ptCurrent);
			GetAgent(i).SetState(CAgent::ALIVE);

			m_Maze.SetMapWalkability(ptCurrent, CAStar::OCCUPIED);

			GetAgent(i).Move(GetCellSize(), 0.0);
		}

		if (i >= UNIQUE_ID_COUNT && i < UNIQUE_ID_COUNT + m_gfxWorld.GetPowerPillCount())
		{
			// power pills
			_Point ptCurrent = m_Maze.GetLocationOdd(CAStar::WALKABLE);

			GetAgent(i).SetLocations(ptCurrent, ptCurrent);
			GetAgent(i).SetTarget(ptCurrent);
			GetAgent(i).SetState(CAgent::ALIVE);

			m_Maze.SetMapWalkability(ptCurrent, CAStar::OCCUPIED);

			GetAgent(i).Move(GetCellSize(), 0.0);

			// store the pill position for collision testing
			m_mapPillLocations[ptCurrent] = &GetAgent(i);
		}

		if (i >= UNIQUE_ID_COUNT + m_gfxWorld.GetPowerPillCount())
		{
			// pills
			_Point ptCurrent = m_Maze.GetLocationOdd(CAStar::WALKABLE);

			GetAgent(i).SetLocations(ptCurrent, ptCurrent);
			GetAgent(i).SetTarget(ptCurrent);
			GetAgent(i).SetState(CAgent::ALIVE);

			m_Maze.SetMapWalkability(ptCurrent, CAStar::OCCUPIED);

			GetAgent(i).Move(GetCellSize(), 0.0);

			// store the pill position for collision testing
			m_mapPillLocations[ptCurrent] = &GetAgent(i);
		}
	}

	// runner
	Spawn(RUNNER);
	SetRunnerSpeed(GetAgent(RUNNER));

	// monsters
	Spawn(GINKY);
	Spawn(BINKY);
	Spawn(YINKY);
	Spawn(RINKY);
}

void CGameWorld::Spawn(int iAgentIndex)
{
	if (iAgentIndex == RUNNER)
	{
		// set location to start
		_Point ptStart = GetAgent(ENTRANCE).GetCurrentLocation();

		// locations for the runner
		_Point ptTarget = GetAgent(EXIT).GetCurrentLocation();

		// set new runner locations
		GetAgent(RUNNER).SetLocations(ptStart, ptStart);
		GetAgent(RUNNER).SetTarget(ptTarget);

		// make alive
		GetAgent(RUNNER).state(CAgent::ALIVE, false);

		GetAgent(RUNNER).Move(GetCellSize(), 0.0);
		GetAgent(RUNNER).find() = true;
	}

	if (iAgentIndex == GINKY || iAgentIndex == BINKY || iAgentIndex == YINKY || iAgentIndex == RINKY)
	{
		// monsters
		_Point ptRunner = GetAgent(RUNNER).GetCurrentLocation();
		_Point ptMonsterSpawnLocation(1, 1);

		// minimum distance threshold factor: 1.00, 0.95, 0.90, 0.85, 0.80...
		// every next level allows monsters to be spawned closer
		double dFactor = (1.00 - (((double)m_uiLevel - 1.00) * 0.05));
		if (dFactor < 0.25)
		{
			dFactor = 0.25;
		}

		// minimum distance monsters are allowed to spawn from the runner
		double dMinDistance = (double)std::min(m_Maze.GetWidth(), m_Maze.GetHeight()) * dFactor;

		// spawn distance
		double dDistance = 0.0;
		do
		{
			ptMonsterSpawnLocation = m_Maze.GetLocationOdd();

			dDistance = ptRunner.GetDistanceFrom(ptMonsterSpawnLocation);
		}
		while (dDistance < dMinDistance);
		
		double dSpeed = RUNNER_SPEED - (RUNNER_SPEED * (double)iAgentIndex * MONSTER_SPEED_FACTOR);

		GetAgent(iAgentIndex).SetLocations(ptMonsterSpawnLocation, ptMonsterSpawnLocation);
		GetAgent(iAgentIndex).SetTarget(ptRunner);

		GetAgent(iAgentIndex).SetSpeed(dSpeed);
		GetAgent(iAgentIndex).state(CAgent::ALIVE, false);

		GetAgent(iAgentIndex).Move(GetCellSize(), 0.0);
		GetAgent(iAgentIndex).find() = true;
	}
}

void CGameWorld::SavePillLocations(string strFilePath)
{
	if (strFilePath.empty())
	{
		strFilePath = "__pills.txt";
	}

	string strFileData = "";

	strFileData += "agents: ";
	strFileData += CUtil::toStringA((int)m_vAgents.size());
	strFileData += "\r\n";
	strFileData += "\r\n";

	strFileData += "pill positions: (map coordinates) -> (scr coordinates)";
	strFileData += "\r\n";
	strFileData += "\r\n";

	map<_Point, int> mapAgents;
	map<_Point, int> _mapAgents;

	for (int i = 0; i < (int)m_vAgents.size(); ++i)
	{
		if (i < UNIQUE_ID_COUNT)
		{
			continue;
		}

		CAgent& agent = GetAgent(i);
		_Point pt = agent.GetCurrentLocation();

		bool bAgent = false;
		if (mapAgents[pt]++ != 0)
		{
			bAgent = true;
		}

		_Point _pt = agent.position();

		bool _bAgent = false;
		if (_mapAgents[_pt]++ != 0)
		{
			_bAgent = true;
		}

		strFileData += "(";
		strFileData += CUtil::toStringA((int)pt.x);
		strFileData += " ,";
		strFileData += CUtil::toStringA((int)pt.y);
		strFileData += ")";
		strFileData += " -> ";
		strFileData += "(";
		strFileData += CUtil::toStringA((int)_pt.x);
		strFileData += " ,";
		strFileData += CUtil::toStringA((int)_pt.y);
		strFileData += ")";

		if (bAgent || _bAgent)
		{
			strFileData += " : collision - ";

			if (bAgent)
			{
				strFileData += "map ";
			}

			if (_bAgent)
			{
				strFileData += "scr ";
			}
		}

		strFileData += "\r\n";
	}

	CUtil::writeFile(strFilePath, strFileData);
}

CAgent& CGameWorld::GetAgent(int iAgentIndex)
{
	if (iAgentIndex == INVALID_VALUE || iAgentIndex < 0 || iAgentIndex >= (int)m_vAgents.size())
	{
		static CAgent agent(INVALID_VALUE);
		return agent;
	}

	CAgent& agent = m_vAgents.at(iAgentIndex);
	return agent;
}

void CGameWorld::SetMonsterTarget(int iAgentIndex)
{
	// check
	if (iAgentIndex != GINKY && iAgentIndex != BINKY && iAgentIndex != YINKY && iAgentIndex != RINKY)
	{
		return;
	}

	// determine which agent
	CAgent& agent = GetAgent(iAgentIndex);
	if (!agent.IsValid())
	{
		return;
	}

	if (!agent.IsDoneMoving())
	{
		return;
	}

	// monster target
	_Point ptTarget(1, 1);

	if (agent.state() == CAgent::ALIVE && m_eDifficulty == DIFFICULTY::HARD)
	{
		// set the new targets for the monsters to chase the runner
		// makes the game too hard
		// ptTarget = GetAgent(RUNNER).GetCurrentLocation();

		// monsters simply move around randomly, and avoid dead-ends |_|
		while (true)
		{
			ptTarget = m_Maze.GetLocationOdd();

			if (!m_Maze.IsDeadEnd(ptTarget))
			{
				break;
			}
		}
	}
	else
	{
		// other wise, monster is in panic mode and picks a random location to go to
		ptTarget = m_Maze.GetLocationOdd();
	}

	if (ptTarget != agent.GetTarget())
	{
		agent.SetTarget(ptTarget);
		m_Maze.SetTargetLocation(iAgentIndex, ptTarget);
		agent.find() = true;
	}
}

void CGameWorld::SetBonusEffects(CAgent& agent)
{
	// check
	if (agent.id() != RUNNER)
	{
		return;
	}

	// orange bonus effect
	SetRunnerSpeed(agent);

	// kiwi bonus effect
	if (m_bTimeBoost)
	{
		// set time boost
		// power pill time increased by 25% permanently
	}

	// mango bonus effect
	if (m_bPermaKill)
	{
		// kill 1 or 2 monsters, that are still alive, permanently
		int iKill = (rand() % 2) + 1;
		for (int i = 0; i < MONSTER_COUNT; ++i)
		{
			switch (i)
			{
			case 0:
				if (GetAgent(GINKY).state() != CAgent::DEAD)
				{
					GetAgent(GINKY).state(CAgent::DEAD, false);
					iKill--;
				}
				break;
			case 1:
				if (GetAgent(BINKY).state() != CAgent::DEAD)
				{
					GetAgent(BINKY).state(CAgent::DEAD, false);
					iKill--;
				}
				break;
			case 2:
				if (GetAgent(YINKY).state() != CAgent::DEAD)
				{
					GetAgent(YINKY).state(CAgent::DEAD, false);
					iKill--;
				}
				break;
			case 3:
				if (GetAgent(RINKY).state() != CAgent::DEAD)
				{
					GetAgent(RINKY).state(CAgent::DEAD, false);
					iKill--;
				}
				break;
			default:
				break;
			}

			if (iKill <= 0)
			{
				break;
			}
		}

		m_bPermaKill = false;
	}

	// dragon bonus effect
	if (m_bShowPathToExit)
	{
		// find the path from runner to exit
		_Point ptStart = GetAgent(RUNNER).GetCurrentLocation();
		_Point ptTarget = GetAgent(EXIT).GetCurrentLocation();
		m_Maze.FindPath(DRAGON, ptStart, ptTarget);

		// path in maze coordinates
		std::vector<_Point> vptEntirePath;
		m_Maze.GetEntirePath(DRAGON, vptEntirePath);

		// convert maze coordinates into screen coordinates
		CAgent::ProjectMazeToScreen(vptEntirePath, m_vptPath, GetCellSize());

		// reset effect
		int iElapsedTime = (int)(m_timerDragon.GetElapsedTime() * 1000.0);

		if (iElapsedTime >= BONUS_EFFECT_TIMEOUT)
		{
			m_bShowPathToExit = false;

			m_vptPath.clear();
		}
	}
	else if (g_bShowPathToClick)
	{
		// find the path of the runner
		_Point ptStart = GetAgent(RUNNER).GetCurrentLocation();
		_Point ptTarget = GetAgent(RUNNER).GetTarget();
		m_Maze.FindPath(RUNNER, ptStart, ptTarget);

		// path in maze coordinates
		std::vector<_Point> vptEntirePath;
		m_Maze.GetEntirePath(RUNNER, vptEntirePath);

		// convert maze coordinates into screen coordinates
		CAgent::ProjectMazeToScreen(vptEntirePath, m_vptPath, GetCellSize());

		// reset effect
		if (vptEntirePath.size() <= 1)
		{
			g_bShowPathToClick = false;

			m_vptPath.clear();
		}
	}
}

void CGameWorld::SetRunnerSpeed(CAgent& agent)
{
	// check
	if (agent.id() != RUNNER)
	{
		return;
	}

	if (m_eDifficulty == DIFFICULTY::MEDIUM)
	{
		GetAgent(RUNNER).SetSpeed(RUNNER_SPEED_FAST);
	}
	else
	{
		GetAgent(RUNNER).SetSpeed(RUNNER_SPEED);
	}

	// orange bonus effect
	if (m_bSpeedBoost)
	{
		// set speed boost
		// speed increased by 25% permanently
		if (m_eDifficulty == DIFFICULTY::MEDIUM)
		{
			GetAgent(RUNNER).SetSpeed(RUNNER_SPEED_FAST * SPEED_BOOST);
		}
		else
		{
			GetAgent(RUNNER).SetSpeed(RUNNER_SPEED * SPEED_BOOST);
		}

		m_bSpeedBoost = false;
	}
}

bool CGameWorld::MoveAgent(int iAgentIndex)
{
	// determine which agent
	CAgent& agent = GetAgent(iAgentIndex);
	if (!agent.IsValid())
	{
		return false;
	}

	// check if agent is mobile
	if (iAgentIndex >= UNIQUE_ID_COUNT)
	{
		return false;
	}

	// dead runner does not need to move
	if (agent.id() == RUNNER && agent.state() == CAgent::DEAD)
	{
		return true;
	}

	// set any bonus effects
	SetBonusEffects(agent);

	// path finding is an expensive operation
	// use only when needed
	if (agent.find())
	{
		_Point ptStart = agent.GetCurrentLocation();
		_Point ptTarget = agent.GetTarget();
		m_Maze.FindPath(iAgentIndex, ptStart, ptTarget);
	}

	// path in maze coordinates
	std::vector<_Point> vptEntirePath;
	m_Maze.GetEntirePath(iAgentIndex, vptEntirePath);

	// the path must be at least 2 cells long
	if (vptEntirePath.size() >= 1)
	{
		// set the next location
		agent.SetLocations(agent.GetCurrentLocation(), vptEntirePath.at(0));

		// get the elapsed time
		double dTime = agent.timer().GetElapsedTime();

		// move and set the screen position
		agent.Move(GetCellSize(), dTime);

		if (agent.IsDoneMoving() || dTime == 0.0)
		{
			// reset the timer and location information
			agent.timer().Start();
			m_Maze.SetStartLocation(iAgentIndex, vptEntirePath.at(0));

			if (vptEntirePath.size() >= 2)
			{
				agent.SetLocations(vptEntirePath.at(0), vptEntirePath.at(1));
			}
			else
			{
				agent.SetLocations(vptEntirePath.at(0), vptEntirePath.at(0));
			}

			// if current and target positions are the same,
			// there is no need to find path
			agent.find() = (agent.GetCurrentLocation() != agent.GetTarget());
		}
		else
		{
			// no need to find path if move to next block hasn't completed yet
			agent.find() = false;
		}
	}
	else
	{
		// no need to find path if the path length is just 1
		agent.find() = false;
	}

	return true;
}

void CGameWorld::MoveAgents()
{
	// move all agents and check for collision
	for (int i = 0; i < (int)m_vAgents.size(); ++i)
	{
		// if an agent hasn't moved, its probably a pill, so break
		if (!MoveAgent(i))
		{
			break;
		}
	}
}

void CGameWorld::HandleCollisions()
{
	// check which agent the runner collided with
	vector<CAgent*> vAgents = CollisionCheck();

	// handle the different collisions
	for (int i = 0; i < (int)vAgents.size(); ++i)
	{
		CAgent* pAgent = vAgents.at(i);
		if (!pAgent)
		{
			continue;
		}

		HandleMonsterCollision(pAgent);
		HandleDoorCollision(pAgent);
		HandlePillCollision(pAgent);
	}
}

bool CGameWorld::IsExitVisible()
{
	if (m_eDifficulty == DIFFICULTY::MEDIUM)
	{
		if (IsAteAllFruits())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return true;
}

vector<CAgent*> CGameWorld::CollisionCheck()
{
	vector<CAgent*> vAgents;

	_Point ptRunner = GetAgent(RUNNER).position();
	for (int i = 1; i < (int)m_vAgents.size(); ++i)
	{
		// pill collisions are not done in this loop
		if (i >= UNIQUE_ID_COUNT)
		{
			break;
		}

		CAgent& agent = m_vAgents.at(i);
		_Point ptAgent = agent.position();

		if (ptRunner.IsNear(ptAgent, (double)GetCellSize() / 2.0))
		{
			vAgents.push_back(&agent);
		}
	}

	// check for pill collisions
	auto it = m_mapPillLocations.find(GetAgent(RUNNER).GetCurrentLocation());
	if (it != m_mapPillLocations.end())
	{
		vAgents.push_back(it->second);
	}

	return vAgents;
}

void CGameWorld::HandleMonsterCollision(CAgent* pAgent)
{
	// monsters eaten counter
	static int iMonsters = 0;
	if (iMonsters >= MONSTER_COUNT)
	{
		iMonsters = 0;
	}

	// check
	if (!pAgent)
	{
		return;
	}

	// ginky, binky, yinky, rinky
	if (pAgent->id() == GINKY || pAgent->id() == BINKY || pAgent->id() == YINKY || pAgent->id() == RINKY)
	{
		switch (pAgent->state())
		{
		case CAgent::ALIVE:
			if (m_bIsDemo || m_eDifficulty != DIFFICULTY::HARD)
			{
				// runner is in god mode
			}
			else
			{
				// runner is killed
				if (GetAgent(RUNNER).state(CAgent::DEAD, true))
				{
					m_uiLives--;

					// play sound
					m_sfx.Play(CSfxManager::SFX_KILL_RUNNER);
				}
			}
			break;
		case CAgent::VULNERABLE:
			if (pAgent->state(CAgent::DEAD, true))
			{
				// increment score
				if (pAgent->id() == GINKY)
				{
					AddScore(++iMonsters * 100);
				}

				if (pAgent->id() == BINKY)
				{
					AddScore(++iMonsters * 100);
				}

				if (pAgent->id() == YINKY)
				{
					AddScore(++iMonsters * 100);
				}

				if (pAgent->id() == RINKY)
				{
					AddScore(++iMonsters * 100);
				}

				// play sound
				m_sfx.Play(CSfxManager::SFX_EAT_MONSTER);
			}
			break;
		case CAgent::DEAD:
			break;
		default:
			break;
		}
	}
}

void CGameWorld::HandleDoorCollision(CAgent* pAgent)
{
	// check
	if (!pAgent)
	{
		return;
	}

	// in demo mode simply change exit location in the current level
	if (m_bIsDemo)
	{
		// randomly generate another location when exit location is reached
		if (pAgent->id() == EXIT)
		{
			_Point ptTarget = m_Maze.GetLocationOdd();

			GetAgent(RUNNER).SetTarget(ptTarget);
			m_Maze.SetTargetLocation(RUNNER, ptTarget);

			// set the entrance to the current exit location
			{
				// entrance
				_Point ptCurrent = m_Maze.GetCurrentPath(EXIT);

				GetAgent(ENTRANCE).SetLocations(ptCurrent, ptCurrent);
				GetAgent(ENTRANCE).SetTarget(ptCurrent);

				m_Maze.SetStartLocation(ENTRANCE, ptCurrent);
				m_Maze.FindPath(ENTRANCE, ptCurrent, ptCurrent);
				m_Maze.SetTargetLocation(ENTRANCE, ptCurrent);

				GetAgent(ENTRANCE).Move(GetCellSize(), 0.0);
			}

			// set the exit to the current runner target location
			{
				// exit
				GetAgent(EXIT).SetLocations(ptTarget, ptTarget);
				GetAgent(EXIT).SetTarget(ptTarget);

				m_Maze.SetStartLocation(EXIT, ptTarget);
				m_Maze.FindPath(EXIT, ptTarget, ptTarget);
				m_Maze.SetTargetLocation(EXIT, ptTarget);

				GetAgent(EXIT).Move(GetCellSize(), 0.0);
			}

			// play sound
			m_sfx.Play(CSfxManager::SFX_START_LEVEL);
		}
	}
	// go to next level when exit is reached
	else
	{
		if (pAgent->id() == EXIT && pAgent->state() == CAgent::ALIVE && !m_bLevelComplete)
		{
			LevelComplete();

			// play sound
			m_sfx.Play(CSfxManager::SFX_END_LEVEL);
		}
	}
}

void CGameWorld::HandlePillCollision(CAgent* pAgent)
{
	// bonus pill eaten counter
	static int iBonusPills = 0;
	if (iBonusPills >= BONUS_PILL_COUNT)
	{
		iBonusPills = 0;
	}

	// check
	if (!pAgent)
	{
		return;
	}

	// orange
	if (pAgent->id() == ORANGE)
	{
		if (pAgent->state(CAgent::DEAD))
		{
			// increment score, apply bonus effect
			AddScore(++iBonusPills * 100);

			m_bSpeedBoost = true;
			g_bSpeedBoost = true;

			// play sound
			m_sfx.Play(CSfxManager::SFX_EAT_ORANGE);
		}
	}

	// kiwi
	if (pAgent->id() == KIWI)
	{
		if (pAgent->state(CAgent::DEAD))
		{
			// increment score, apply bonus effect
			AddScore(++iBonusPills * 100);

			m_bTimeBoost = true;
			g_bTimeBoost = true;

			// play sound
			m_sfx.Play(CSfxManager::SFX_EAT_KIWI);
		}
	}

	// mango
	if (pAgent->id() == MANGO)
	{
		if (pAgent->state(CAgent::DEAD))
		{
			// increment score, apply bonus effect
			AddScore(++iBonusPills * 100);

			m_bPermaKill = true;
			g_bPermaKill = true;

			// play sound
			m_sfx.Play(CSfxManager::SFX_EAT_MANGO);
		}
	}

	// dragon
	if (pAgent->id() == DRAGON)
	{
		if (pAgent->state(CAgent::DEAD))
		{
			// increment score, apply bonus effect
			AddScore(++iBonusPills * 100);

			m_bShowPathToExit = true;
			g_bShowPathToExit = true;

			m_timerDragon.Start();

			// play sound
			m_sfx.Play(CSfxManager::SFX_EAT_DRAGON);
		}
	}

	// power
	if (pAgent->id() == POWER)
	{
		if (pAgent->state(CAgent::DEAD))
		{
			// increment score, and change monster state to edible and reset state timer
			AddScore(5);

			if (GetAgent(GINKY).state() != CAgent::DEAD)
			{
				GetAgent(GINKY).state(CAgent::VULNERABLE, true, true);
			}

			if (GetAgent(BINKY).state() != CAgent::DEAD)
			{
				GetAgent(BINKY).state(CAgent::VULNERABLE, true, true);
			}

			if (GetAgent(YINKY).state() != CAgent::DEAD)
			{
				GetAgent(YINKY).state(CAgent::VULNERABLE, true, true);
			}

			if (GetAgent(RINKY).state() != CAgent::DEAD)
			{
				GetAgent(RINKY).state(CAgent::VULNERABLE, true, true);
			}

			// play sound
			m_sfx.Play(CSfxManager::SFX_EAT_POWER);
		}
	}

	// pill
	if (pAgent->id() == PILL)
	{
		if (pAgent->state(CAgent::DEAD))
		{
			// increment score
			AddScore(1);

			// play sound
			m_sfx.Play(CSfxManager::SFX_EAT_PILL);
		}
	}
}

void CGameWorld::UpdateStates()
{
	for (int i = 0; i < (int)m_vAgents.size(); ++i)
	{
		// pill collisions are not done in this loop
		if (i >= UNIQUE_ID_COUNT)
		{
			break;
		}

		CAgent& agent = m_vAgents.at(i);

		if (agent.id() == RUNNER)
		{
			// check time of death!
			if (agent.state() == CAgent::DEAD)
			{
				// check if there are any lives left
				if (m_uiLives <= 0)
				{
					m_uiRunnerSpawnTime = 0;
					m_bGameOver = true;

					// play sound
					m_sfx.Play(CSfxManager::SFX_GAME_OVER);
				}
				else
				{
					m_uiRunnerSpawnTime = agent.state_time();

					if (m_uiRunnerSpawnTime >= RUNNER_SPAWN_TIMEOUT)
					{
						m_uiRunnerSpawnTime = 0;

						// runner
						Spawn(RUNNER);
						SetRunnerSpeed(agent);

						// monsters
						Spawn(GINKY);
						Spawn(BINKY);
						Spawn(YINKY);
						Spawn(RINKY);

						// reset demo / play mode
						SetDemo(m_bIsDemo);

						// play sound
						m_sfx.Play(CSfxManager::SFX_START_LEVEL);
					}
				}
			}
		}

		if (agent.id() == GINKY || agent.id() == BINKY || agent.id() == YINKY || agent.id() == RINKY)
		{
			// check if time bonus is active, and set monster time out accordingly
			unsigned int uiStateTime = m_bTimeBoost ? (unsigned int)(MONSTER_VULNERABLE_TIMEOUT * TIME_BOOST) : MONSTER_VULNERABLE_TIMEOUT;

			if (agent.state() == CAgent::VULNERABLE && agent.state_time() >= uiStateTime)
			{
				agent.state(CAgent::ALIVE, false);
			}
		}

		if (agent.id() == EXIT)
		{
			if (IsExitVisible())
			{
				agent.state() = CAgent::ALIVE;
			}
			else
			{
				agent.state() = CAgent::DEAD;
			}
		}
	}
}

void CGameWorld::LevelComplete()
{
	// level complete score
	AddScore((m_uiLevel + m_uiLives) * 1000);

	// increment level
	m_uiLevel++;
	g_uiLevel = m_uiLevel;

	m_bLevelComplete = true;
}

void CGameWorld::LoadLevel(unsigned int uiLevel, S2D_Window* pGameWindow)
{
	// check
	if (pGameWindow == nullptr)
	{
		return;
	}

	// check level limits
	if (uiLevel < MIN_LEVEL)
	{
		uiLevel = MIN_LEVEL;
	}

	if (uiLevel > MAX_LEVEL)
	{
		uiLevel = MAX_LEVEL;
	}

	// check
	if (IsRunning())
	{
		// nothing to do!
	}

	// current number of rows/columns
	int iNumberOfRowColumn = LEVEL_1_GRID_SIZE + uiLevel;

	// set dimension info
	static int iWidth = pGameWindow->width - g_settings.wallSize;
	static int iHeight = pGameWindow->height - BOTTOM_BAR_SIZE;
	int iCellSize = std::min(iWidth, iHeight) / iNumberOfRowColumn;

	// reset game over
	if (IsGameOver())
	{
		m_bLevelComplete = false;
		m_uiRunnerSpawnTime = 0;
		m_bGameOver = false;

		uiLevel = MIN_LEVEL;

		SetSavedData(0, START_LIVES, START_LIVES, MIN_LEVEL);
	}

	m_uiLevel = uiLevel;
	g_uiLevel = uiLevel;

	// initialize the game world
	Load(iWidth, iHeight, iCellSize);
}

void CGameWorld::Draw(bool bgOnly, bool bNoExtras)
{
	m_gfxWorld.Draw(m_vAgents, m_vptPath, bgOnly, bNoExtras);
}

void CGameWorld::InitializeGameWorld()
{
	// initialize bonus flags and timers
	m_bSpeedBoost = false;
	m_bTimeBoost = false;
	m_bPermaKill = false;
	m_bShowPathToExit = false;

	g_bSpeedBoost = false;
	g_bTimeBoost = false;
	g_bPermaKill = false;
	g_bShowPathToExit = false;

	// set the flags
	m_bIsRunning = true;
	m_bPause = false;

	// set level complete flag
	m_bLevelComplete = false;

	m_uiRunnerSpawnTime = 0;

	// clear the path to exit, and reset timer
	m_vptPath.clear();
	m_timerDragon.Reset();

	//////////////////////////////////////////////////////////////////////////
	// set start locations
	//////////////////////////////////////////////////////////////////////////
	SetInitialLocations();
	//////////////////////////////////////////////////////////////////////////

	// reset demo / play mode
	SetDemo(m_bIsDemo);

	// save pill locations
	// SavePillLocations("__pills.txt");

	// play bg music
	m_sfx.PlayBGMusic(m_uiLevel);
}

bool CGameWorld::Load(int iWidth, int iHeight, int iCellSize)
{
	// generate a maze of given size
	m_mg.generate(iWidth, iHeight, iCellSize);

	if (!LoadMaze(m_mg.getMaze(), m_mg.getMazeSizeX(), m_mg.getMazeSizeY()))
	{
		return false;
	}

	// save the maze
	// m_Maze.PrintWalkability("__maze.txt");

	// load the world
	m_gfxWorld.Load(GetWidth(), GetHeight(), GetCellSize());
	m_gfxWorld.SetMaze(m_mg.getMaze(), m_mg.getMazeSizeX(), m_mg.getMazeSizeY(), m_mg.getCellSize());
	m_gfxWorld.ResizeSprites();

	// create agents
	m_vAgents.clear();
	for (int i = 0; i < m_gfxWorld.GetElementCount(); ++i)
	{
		AGENT_ID id = (AGENT_ID)i;

		if (id >= 0 && id < UNIQUE_ID_COUNT)
		{
			CAgent agent(id);
			m_vAgents.push_back(agent);
		}

		if (id >= UNIQUE_ID_COUNT && id < UNIQUE_ID_COUNT + m_gfxWorld.GetPowerPillCount())
		{
			CAgent agent(POWER);
			m_vAgents.push_back(agent);
		}

		if (id >= UNIQUE_ID_COUNT + m_gfxWorld.GetPowerPillCount())
		{
			CAgent agent(PILL);
			m_vAgents.push_back(agent);
		}
	}

	return true;
}

bool CGameWorld::LoadMaze(const unsigned char* pMaze, int iMazeSizeX, int iMazeSizeY)
{
	// check
	if (!pMaze || iMazeSizeX <= 0 || iMazeSizeY <= 0)
	{
		return false;
	}

	// at least 2 pixels required for wall and path
	int iCellSize = 2;

	// number of path finders, 1 maze runner, 4 monsters, and all other elements
	m_Maze.Initialize(iMazeSizeX * iCellSize + 1, iMazeSizeY * iCellSize + 1, iMazeSizeX * iMazeSizeY, CAStar::WALKABLE);

	// check
	if (!m_Maze.IsReady())
	{
		return false;
	}

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
				_Point ptPrevious(nx, ny);
				_Point ptNext(nx + iCellSize + 1, ny);
				m_Maze.DrawLine(ptPrevious, ptNext, CAStar::UNWALKABLE, false);
			}
			if (!(cell & mazes::EAST))
			{
				_Point ptPrevious(nx + iCellSize, ny);
				_Point ptNext(nx + iCellSize, ny + iCellSize + 1);
				m_Maze.DrawLine(ptPrevious, ptNext, CAStar::UNWALKABLE, false);
			}
			if (!(cell & mazes::SOUTH))
			{
				_Point ptPrevious(nx, ny + iCellSize);
				_Point ptNext(nx + iCellSize + 1, ny + iCellSize);
				m_Maze.DrawLine(ptPrevious, ptNext, CAStar::UNWALKABLE, false);
			}
			if (!(cell & mazes::WEST))
			{
				_Point ptPrevious(nx, ny);
				_Point ptNext(nx, ny + iCellSize + 1);
				m_Maze.DrawLine(ptPrevious, ptNext, CAStar::UNWALKABLE, false);
			}
		}
	}

	return true;
}

bool CGameWorld::Move(CAgent::directions direction)
{
	// check
	if (m_bIsDemo)
	{
		return false;
	}

	// nth runner position starts at current runner position
	_Point ptNextn = GetAgent(RUNNER).GetCurrentLocation();

	// flag
	bool bHasMoved = false;

	// find the last valid cell or intersection in the direction of travel
	while (true)
	{
		// create the next position
		switch (direction)
		{
		case CAgent::NORTH:
			ptNextn.y--;
			break;
		case CAgent::EAST:
			ptNextn.x++;
			break;
		case CAgent::SOUTH:
			ptNextn.y++;
			break;
		case CAgent::WEST:
			ptNextn.x--;
			break;
		default:
			return false;
			break;
		}

		if (!m_Maze.IsValidLocation(ptNextn))
		{
			break;
		}

		// set the new runner positions
		GetAgent(RUNNER).SetEnd(ptNextn);
		GetAgent(RUNNER).SetTarget(ptNextn);
		GetAgent(RUNNER).Move(GetCellSize(), 0.0, false);

		bHasMoved = true;

		// if an intersection is found, stop moving
		if (IsIntersection(ptNextn, direction))
		{
			break;
		}
	}

	return bHasMoved;
}

bool CGameWorld::Move(AGENT_ID aID, int gridX, int gridY)
{
	// check
	if (m_bIsDemo)
	{
		return false;
	}

	// get the current agent position
	_Point ptCurrent = GetAgent(aID).GetCurrentLocation();

	// nth runner position
	_Point ptNext = ptCurrent;
	ptNext.x = gridX;
	ptNext.y = gridY;

	if (!m_Maze.IsValidLocation(ptNext))
	{
		return false;
	}

	// set the new agent positions
	GetAgent(aID).SetLocations(ptCurrent, ptNext);
	GetAgent(aID).SetTarget(ptNext);
	GetAgent(aID).Move(GetCellSize(), 0.0);

	g_bShowPathToClick = true;

	return true;
}

bool CGameWorld::IsIntersection(_Point pt, CAgent::directions direction)
{
	_Point ptNorth = pt;
	ptNorth.y--;

	_Point ptEast = pt;
	ptEast.x++;

	_Point ptSouth = pt;
	ptSouth.y++;

	_Point ptWest = pt;
	ptWest.x--;

	CAgent::directions opposite_direction = CAgent::dNONE;
	switch (direction)
	{
	case CAgent::NORTH:
		opposite_direction = CAgent::SOUTH;
		break;
	case CAgent::EAST:
		opposite_direction = CAgent::WEST;
		break;
	case CAgent::SOUTH:
		opposite_direction = CAgent::NORTH;
		break;
	case CAgent::WEST:
		opposite_direction = CAgent::EAST;
		break;
	}

	for (int i = CAgent::dNONE; i < CAgent::dCOUNT; ++i)
	{
		auto d = (CAgent::directions) i;
		if (d == direction || d == opposite_direction)
		{
			continue;
		}

		switch (d)
		{
		case CAgent::NORTH:
			if (m_Maze.IsValidLocation(ptNorth))
			{
				return true;
			}
			break;
		case CAgent::EAST:
			if (m_Maze.IsValidLocation(ptEast))
			{
				return true;
			}
			break;
		case CAgent::SOUTH:
			if (m_Maze.IsValidLocation(ptSouth))
			{
				return true;
			}
			break;
		case CAgent::WEST:
			if (m_Maze.IsValidLocation(ptWest))
			{
				return true;
			}
			break;
		}
	}

	return false;
}
