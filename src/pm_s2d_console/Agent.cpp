#include "stdafx.h"
#include "Agent.h"

#include <mutex>

// ugly global mutex, but it's the simplest solution
std::mutex g_agentMutex;

// ugly global for simplicity :-P
static int g_iCellSize = 1;

#pragma warning(disable : 26812)

_Point CAgent::Move(int iCellSize, double dTimeElapsed, bool bUpdateStartLocation /* = true*/)
{
	// lock
	g_agentMutex.lock();

	// update cell size
	g_iCellSize = iCellSize;

	// start point in screen coordinates
	_Point _ptStart = position();
	if (bUpdateStartLocation)
	{
		_ptStart = ToScreen(m_ptStart);
	}

	// next point in screen coordinates
	_Point _ptNext = ToScreen(m_ptNext);

	// if done moving or end point has been reached, then return end point
	if (m_bDoneMoving || _ptStart == _ptNext)
	{
		m_bDoneMoving = true;

		m_agent.position = _ptNext;
		SetDirection();

		// unlock before returning
		g_agentMutex.unlock();

		return _ptNext;
	}

	// convert speed: cells per second to pixels per second, in screen coordinates
	double dSpeed = (int)(m_dSpeed * iCellSize);

	_Point _ptDistance;
	_ptDistance.x = (int)(dSpeed * dTimeElapsed);
	_ptDistance.y = (int)(dSpeed * dTimeElapsed);

	directions d = dNONE;

	if (_ptNext.y > _ptStart.y)
	{
		d = SOUTH;
		_ptDistance.x = 0;
	}

	if (_ptNext.y < _ptStart.y)
	{
		d = NORTH;
		_ptDistance.x = 0;
		_ptDistance.y = -_ptDistance.y;
	}

	if (_ptNext.x > _ptStart.x)
	{
		d = EAST;
		_ptDistance.y = 0;
	}

	if (_ptNext.x < _ptStart.x)
	{
		d = WEST;
		_ptDistance.x = -_ptDistance.x;
		_ptDistance.y = 0;
	}

	_Point _ptNew = _ptStart;
	_ptNew.x += _ptDistance.x;
	_ptNew.y += _ptDistance.y;

	if (dTimeElapsed == 0.0)
	{
		_ptNew = position();
	}

	switch (d)
	{
	case NORTH:
		if (_ptNew.y < _ptNext.y)
		{
			_ptNew = _ptNext;
			m_bDoneMoving = true;
		}
		break;
	case EAST:
		if (_ptNew.x > _ptNext.x)
		{
			_ptNew = _ptNext;
			m_bDoneMoving = true;
		}
		break;
	case SOUTH:
		if (_ptNew.y > _ptNext.y)
		{
			_ptNew = _ptNext;
			m_bDoneMoving = true;
		}
		break;
	case WEST:
		if (_ptNew.x < _ptNext.x)
		{
			_ptNew = _ptNext;
			m_bDoneMoving = true;
		}
		break;
	default:
		break;
	}

	m_agent.position = _ptNew;
	SetDirection();

	// unlock before returning
	g_agentMutex.unlock();

	return _ptNew;
}

_Point CAgent::ToGrid(_Point ptScreen)
{
	_Point _ptGrid(0, 0);

	if (g_iCellSize <= 0)
	{
		return _ptGrid;
	}

	_ptGrid.x = ptScreen.x / g_iCellSize;
	_ptGrid.y = ptScreen.y / g_iCellSize;

	_ptGrid.x = 2 * _ptGrid.x + 1;
	_ptGrid.y = 2 * _ptGrid.y + 1;

	return _ptGrid;
}

_Point CAgent::ToScreen(_Point ptGrid)
{
	_Point _ptScreen = ptGrid;
	_ptScreen.x = _ptScreen.x / 2;
	_ptScreen.x = _ptScreen.x * g_iCellSize;
	_ptScreen.y = _ptScreen.y / 2;
	_ptScreen.y = _ptScreen.y * g_iCellSize;
	return _ptScreen;
}

bool CAgent::IsInTransit(_Point ptPos)
{
	_Point ptGrid = ToGrid(ptPos);
	_Point ptScreen = ToScreen(ptGrid);

	return !ptPos.IsNearFast(ptScreen, 2);
}

bool CAgent::IsDoneMoving()
{
	return m_bDoneMoving;
}

void CAgent::ProjectMazeToScreen(std::vector<_Point>& vptMaze, std::vector<_Point>& vptScreen)
{
	vptScreen.clear();
	for (int i = 0; i < (int)vptMaze.size(); ++i)
	{
		// point in maze coordinates
		_Point pt = vptMaze[i];

		// point in screen coordinates
		_Point _pt = ToScreen(pt);

		vptScreen.push_back(_pt);
	}
}

string CAgent::GetLog()
{
	string strLog = "";

	strLog += "l: ";
	strLog += "(";
	strLog += CUtil::toStringA(m_ptStart.x / 2);
	strLog += ", ";
	strLog += CUtil::toStringA(m_ptStart.y / 2);
	strLog += ") -> ";

	strLog += "(";
	strLog += CUtil::toStringA(m_ptNext.x / 2);
	strLog += ", ";
	strLog += CUtil::toStringA(m_ptNext.y / 2);
	strLog += ") -> ";

	strLog += "(";
	strLog += CUtil::toStringA(m_ptTarget.x / 2);
	strLog += ", ";
	strLog += CUtil::toStringA(m_ptTarget.y / 2);
	strLog += ")";
	strLog += "; ";

	strLog += "s: ";
	strLog += CUtil::toStringA(m_dSpeed, 2);
	strLog += "; ";

	strLog += "p: ";
	strLog += "(";
	strLog += CUtil::toStringA(position().x);
	strLog += ", ";
	strLog += CUtil::toStringA(position().y);
	strLog += "); ";

	strLog += IsInTransit(position()) ? "T" : "..";
	strLog += " ";

	strLog += !IsDoneMoving() ? "M" : "..";
	strLog += " ";

	return strLog;
}
