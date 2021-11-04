#pragma once

#include "Util.h"
#include "Timer.h"
#include "_Structures.h"

#define RUNNER_SPEED				3.33	// cells per seconds
#define RUNNER_SPEED_FAST			4.00	// cells per seconds
#define MONSTER_SPEED_FACTOR		0.1333	// % difference of speed between monsters, compared to normal runner speed

#define RUNNER_SPAWN_TIMEOUT		2500	// milliseconds

#define MONSTER_VULNERABLE_TIMEOUT	10000	// milliseconds

#define BONUS_EFFECT_TIMEOUT		13333	// milliseconds

#define SPEED_BOOST					1.25
#define TIME_BOOST					1.33

#define MONSTER_COUNT				4
#define BONUS_PILL_COUNT			4

#define LEVEL_1_GRID_SIZE			5

#define START_LIVES					2
#define MIN_LEVEL					1
#define MAX_LEVEL					99
#define NO_EXTRAS					((MAX_LEVEL + 1) / 4)

#define POWER_PILL_FACTOR			0.066	// 6.6%

#define PAUSE_TIMEOUT				250

#define BOTTOM_BAR_SIZE				16

enum AGENT_ID
{
	RUNNER		= 0,
	GINKY,
	BINKY,
	YINKY,
	RINKY,
	ENTRANCE,
	EXIT,
	ORANGE,
	KIWI,
	MANGO,
	DRAGON,
	UNIQUE_ID_COUNT,

	POWER		= 100,
	PILL		= 101
};

class CAgent
{
public:
	enum states { sNONE = 0, ALIVE, DEAD, VULNERABLE, sCOUNT };
	enum directions { dNONE = 0, NORTH, EAST, SOUTH, WEST, dCOUNT };
	enum face_directions { LEFT = -1, fNONE = 0, RIGHT = 1 };

public:
	struct _Agent
	{
		// agent id
		int id;

		// current screen position
		_Point position;

		// face direction
		int direction;

		// state
		int state;

		bool find;

		CTimer timer;

		CTimer timer_state;

		_Agent(int iID)
		{
			id = iID;
			position.x = -1;
			position.y = -1;
			direction = fNONE;
			state = sNONE;

			find = true;

			timer.Start();
		};

		bool IsValid()
		{
			if (id == INVALID_VALUE || id < 0 || position.x < 0 || position.y < 0)
			{
				return false;
			}

			return true;
		};
	};

private:
	_Agent m_agent;

public:
	int& id() { return m_agent.id; };
	_Point& position() { return m_agent.position; };
	int& direction() { return m_agent.direction; };
	int& state() { return m_agent.state; };
	bool& find() { return m_agent.find; };
	CTimer& timer() { return m_agent.timer; };

	// set the state and check for state change
	bool state(int _state)
	{
		if (m_agent.state == _state)
		{
			return false;
		}

		m_agent.state = _state;
		return true;
	}

	// set state and start/stop state timer
	bool state(int _state, bool _start, bool _ignoreStateChange = false)
	{
		bool result = state(_state);

		if (_ignoreStateChange)
		{
			if (_start)
			{
				m_agent.timer_state.Start();
			}
			else
			{
				m_agent.timer_state.Reset();
				m_agent.timer_state.Stop();
			}
		}
		else
		{
			if (result && _start)
			{
				m_agent.timer_state.Start();
			}

			if (!_start)
			{
				m_agent.timer_state.Reset();
				m_agent.timer_state.Stop();
			}
		}

		return result;
	}

	// returns the time elapsed in milliseconds
	unsigned int state_time()
	{
		return (unsigned int)(m_agent.timer_state.GetElapsedTime() * 1000.0);
	}

	// all coordinates and units below are in A* grids, unless otherwise mentioned
private:
	_Point m_ptStart;
	_Point m_ptEnd;

private:
	_Point m_ptTarget;

private:
	double m_dSpeed;

	bool m_bDoneMoving;

	int m_iViewRadius;

public:
	CAgent(int iID) : m_agent(iID)
	{
		m_dSpeed = 0;

		m_bDoneMoving = false;

		m_iViewRadius = 0;
	};

	~CAgent(void) {};

	bool IsValid() { return m_agent.IsValid(); };

public:
	void SetLocations(_Point _ptStart, _Point _ptEnd)
	{
		m_ptStart = _ptStart;
		m_ptEnd = _ptEnd;

		m_bDoneMoving = (_ptStart == _ptEnd);
	};

	_Point GetCurrentLocation()	{ return m_ptStart; };
	_Point GetNextLocation() { return m_ptEnd; };

public:
	void SetTarget(_Point _ptTarget) { m_ptTarget = _ptTarget; };
	_Point GetTarget() { return m_ptTarget; };

public:
	void SetSpeed(double dSpeed) { m_dSpeed = dSpeed; };
	double GetSpeed() { return m_dSpeed; };

public:
	void SetViewRadius(int iViewRadius) { m_iViewRadius = iViewRadius; };
	int GetViewRadius() { return m_iViewRadius;	};

public:
	void SetState(int iState) { m_agent.state = iState; };
	int GetState() { return m_agent.state; };

private:
	void SetDirection()
	{
		if (m_ptEnd.x > m_ptStart.x)
		{
			m_agent.direction = RIGHT;
			return;
		}

		if (m_ptEnd.x < m_ptStart.x)
		{
			m_agent.direction = LEFT;
			return;
		}

		m_agent.direction = fNONE;
	};

public:
	string GetLog()
	{
		string strLog = "";

		strLog += "Locations: ";
		strLog += "(";
		strLog += CUtil::toStringA(m_ptStart.x);
		strLog += ", ";
		strLog += CUtil::toStringA(m_ptStart.y);
		strLog += ") -> ";

		strLog += "(";
		strLog += CUtil::toStringA(m_ptTarget.x);
		strLog += ", ";
		strLog += CUtil::toStringA(m_ptTarget.y);
		strLog += ")";
		strLog += "\r\n";

		strLog += "Speed: ";
		strLog += CUtil::toStringA(m_dSpeed, 4);
		strLog += "\r\n";

		return strLog;
	};

public:
	_Point Move(int iCellSize, double dTimeElapsed);

	bool IsDoneMoving()	{ return m_bDoneMoving; };

	static void ProjectMazeToScreen(std::vector<_Point>& vptMaze, std::vector<_Point>& vptScreen, int iCellSize)
	{
		vptScreen.clear();
		for (int i = 0; i < (int)vptMaze.size(); ++i)
		{
			// point in maze coordinates
			_Point pt = vptMaze[i];

			// point in screen coordinates
			_Point _pt = pt;
			_pt.x = _pt.x / 2;
			_pt.x = _pt.x * iCellSize;
			_pt.y = _pt.y / 2;
			_pt.y = _pt.y * iCellSize;

			vptScreen.push_back(_pt);
		}
	};
};