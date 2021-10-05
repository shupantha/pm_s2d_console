#include "stdafx.h"
#include "Agent.h"

#include <mutex>

// ugly global mutex, but it's the simplest solution
std::mutex g_agentMutex;

_Point CAgent::Move(int iCellSize, double dTimeElapsed)
{
	// lock
	g_agentMutex.lock();

	// start point in screen coordinates
	_Point _ptStart = m_ptStart;
	_ptStart.x = _ptStart.x / 2;
	_ptStart.x = _ptStart.x * iCellSize;
	_ptStart.y = _ptStart.y / 2;
	_ptStart.y = _ptStart.y * iCellSize;

	// end point in screen coordinates
	_Point _ptEnd = m_ptEnd;
	_ptEnd.x = _ptEnd.x / 2;
	_ptEnd.x = _ptEnd.x * iCellSize;
	_ptEnd.y = _ptEnd.y / 2;
	_ptEnd.y = _ptEnd.y * iCellSize;

	// if done moving or end point has been reached, then return end point
	if (m_bDoneMoving || _ptStart == _ptEnd || m_ptStart == m_ptEnd)
	{
		m_bDoneMoving = true;

		m_agent.position = _ptEnd;
		SetDirection();

		// unlock before returning
		g_agentMutex.unlock();

		return _ptEnd;
	}

	// convert speed: cells per second to pixels per second, in screen coordinates
	double dSpeed = (int)(m_dSpeed * iCellSize);

	_Point _ptDistance;
	_ptDistance.x = (int)(dSpeed * dTimeElapsed);
	_ptDistance.y = (int)(dSpeed * dTimeElapsed);

	directions d = dNONE;

	if (_ptEnd.y > _ptStart.y)
	{
		d = SOUTH;
		_ptDistance.x = 0;
	}

	if (_ptEnd.y < _ptStart.y)
	{
		d = NORTH;
		_ptDistance.x = 0;
		_ptDistance.y = -_ptDistance.y;
	}

	if (_ptEnd.x > _ptStart.x)
	{
		d = EAST;
		_ptDistance.y = 0;
	}

	if (_ptEnd.x < _ptStart.x)
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
		if (_ptNew.y < _ptEnd.y)
		{
			_ptNew = _ptEnd;
			m_bDoneMoving = true;
		}
		break;
	case EAST:
		if (_ptNew.x > _ptEnd.x)
		{
			_ptNew = _ptEnd;
			m_bDoneMoving = true;
		}
		break;
	case SOUTH:
		if (_ptNew.y > _ptEnd.y)
		{
			_ptNew = _ptEnd;
			m_bDoneMoving = true;
		}
		break;
	case WEST:
		if (_ptNew.x < _ptEnd.x)
		{
			_ptNew = _ptEnd;
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
