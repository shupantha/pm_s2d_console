#include "stdafx.h"

#include "AStar.h"

#include "math.h"

#include "Util.h"

// Path related constants
// const int CAStar::NOT_STARTED;
// 
// const int CAStar::FOUND;
// const int CAStar::NON_EXISTENT;
// 
// Walk ability constants
// const int CAStar::WALKABLE;
// const int CAStar::UNWALKABLE;
// const int CAStar::OCCUPIED;

CAStar::CAStar(void)
{
	Initialize();
}

CAStar::~CAStar(void)
{
	Destroy();
}

bool CAStar::IsReady()
{
	if(m_iMapWidth <= 0 || m_iMapHeight <= 0)
	{
		return false;
	}

	if(m_iPathFinderCount <= 0 || m_iOnClosedList < 0)
	{
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	// Used by CPU

	// 2D map of size ([m_iMapWidth][m_iMapHeight]) to hold the map
	if(!m_Map2DWalkability.IsLoaded())
	{
		return false;
	}

	// 1D array of size (m_iMapWidth * m_iMapHeight + 2) holding ID# of open list items
	if(m_piOpenListIDs == nullptr)
	{
		return false;
	}

	// 2D map of size ([m_iMapWidth + 1][m_iMapHeight + 1]) used to record whether a cell is on the open list or on the closed list
	if(!m_Map2DWhichList.IsLoaded())
	{
		return false;
	}

	// 1D array of size (m_iMapWidth * m_iMapHeight + 2) to store the (x, y) location of an item on the open list
	if(m_piptOpenListCoordinates_x == nullptr || m_piptOpenListCoordinates_y == nullptr)
	{
		return false;
	}

	// 2D map of size ([m_iMapWidth + 1][m_iMapHeight + 1]) to store parent of each cell (x, y)
	if(!m_Map2DCoordinatesParents_x.IsLoaded() || !m_Map2DCoordinatesParents_y.IsLoaded())
	{
		return false;
	}

	// 1D array of size (m_iMapWidth * m_iMapHeight + 2) to store F cost of a cell on the open list
	if(m_piFCosts == nullptr)
	{
		return false;
	}

	// 2D map of size ([m_iMapWidth + 1][m_iMapHeight + 1]) to store G cost for each cell
	if(!m_Map2DGCosts.IsLoaded())
	{
		return false;
	}

	// 1D array of size (m_iMapWidth * m_iMapHeight + 2) to store H cost of a cell on the open list
	if(m_piHCosts == nullptr)
	{
		return false;
	}

	// 1D array of size (m_iPathFinderCount + 1) to store length of the found paths
	if(m_piPathLengths == nullptr)
	{
		return false;
	}

	// 1D array of size (m_iPathFinderCount + 1) to store current position of a agent along the chosen path
	if(m_piPathLocations == nullptr)
	{
		return false;
	}

	// 2D array of size ([m_iPathFinderCount + 1][4]) for the path bank
	if(m_ppiPathBank == nullptr)
	{
		return false;
	}

	// 1D array of size (m_iPathFinderCount + 1) to store the x & y coordinates of the first path step
	if(m_piPathBank_x == nullptr || m_piPathBank_y == nullptr)
	{
		return false;
	}

	// 1D array of size (m_iPathFinderCount + 1) for the path status
	if(m_piPathStatus == nullptr)
	{
		return false;
	}

	// 1D array of size (m_iPathFinderCount + 1) for the path coordinates
	if(m_piptPath_x == nullptr || m_piptPath_y == nullptr)
	{
		return false;
	}

	// 1D array of size (m_iPathFinderCount + 1) for start locations
	if(m_piStartLocations_x == nullptr || m_piStartLocations_y == nullptr)
	{
		return false;
	}

	// 1D array of size (m_iPathFinderCount + 1) for target locations
	if(m_piTargetLocations_x == nullptr || m_piTargetLocations_y == nullptr)
	{
		return false;
	}
	//////////////////////////////////////////////////////////////////////////

	return true;
}

bool CAStar::IsValid(_Point _pt)
{
	if (_pt.x == INVALID_VALUE || _pt.y == INVALID_VALUE)
	{
		return false;
	}

	if (_pt.x < 0 || _pt.y < 0)
	{
		return false;
	}

	return true;
}

void CAStar::Initialize()
{
	m_iMapWidth = 0;
	m_iMapHeight = 0;

	m_iPathFinderCount = 0;

	m_iOnClosedList = 0;

	m_bStop = false;

	//////////////////////////////////////////////////////////////////////////
	// Used by CPU

	// 2D map of size ([m_iMapWidth][m_iMapHeight]) to hold the map
	m_Map2DWalkability.Destroy();

	// 1D array of size (m_iMapWidth * m_iMapHeight + 2) holding ID# of open list items
	m_piOpenListIDs = nullptr;

	// 2D map of size ([m_iMapWidth + 1][m_iMapHeight + 1]) used to record whether a cell is on the open list or on the closed list
	m_Map2DWhichList.Destroy();

	// 1D array of size (m_iMapWidth * m_iMapHeight + 2) to store the (x, y) location of an item on the open list
	m_piptOpenListCoordinates_x = nullptr;
	m_piptOpenListCoordinates_y = nullptr;

	// 2D map of size ([m_iMapWidth + 1][m_iMapHeight + 1]) to store parent of each cell (x, y)
	m_Map2DCoordinatesParents_x.Destroy();
	m_Map2DCoordinatesParents_y.Destroy();

	// 1D array of size (m_iMapWidth * m_iMapHeight + 2) to store F cost of a cell on the open list
	m_piFCosts = nullptr;

	// 2D map of size ([m_iMapWidth + 1][m_iMapHeight + 1]) to store G cost for each cell
	m_Map2DGCosts.Destroy();

	// 1D array of size (m_iMapWidth * m_iMapHeight + 2) to store H cost of a cell on the open list
	m_piHCosts = nullptr;

	// 1D array of size (m_iPathFinderCount + 1) to store length of the found paths
	m_piPathLengths = nullptr;

	// 1D array of size (m_iPathFinderCount + 1) to store current position of a agent along the chosen path
	m_piPathLocations = nullptr;

	// 2D array of size ([m_iPathFinderCount + 1][4]) for the path bank
	m_ppiPathBank = nullptr;

	// 1D array of size (m_iPathFinderCount + 1) to store the x & y coordinates of the first path step
	m_piPathBank_x = nullptr;
	m_piPathBank_y = nullptr;

	// 1D array of size (m_iPathFinderCount + 1) for the path status
	m_piPathStatus = nullptr;

	// 1D array of size (m_iPathFinderCount + 1) for the path coordinates
	m_piptPath_x = nullptr;
	m_piptPath_y = nullptr;

	// 1D array of size (m_iPathFinderCount + 1) for start locations
	m_piStartLocations_x = nullptr;
	m_piStartLocations_y = nullptr;

	// 1D array of size (m_iPathFinderCount + 1) for target locations
	m_piTargetLocations_x = nullptr;
	m_piTargetLocations_y = nullptr;
	//////////////////////////////////////////////////////////////////////////
}

void CAStar::Initialize(int iMapWidth, int iMapHeight, int iPathFinderCount, int iWalkability /* = WALKABLE*/)
{
	Destroy();

	m_iMapWidth = iMapWidth;
	m_iMapHeight = iMapHeight;

	m_iPathFinderCount = iPathFinderCount;

	m_iOnClosedList = 10;

	m_bStop = false;

	//////////////////////////////////////////////////////////////////////////
	// Used by CPU

	// 2D map of size ([m_iMapWidth][m_iMapHeight]) to hold the map
	m_Map2DWalkability.Create(m_iMapWidth, m_iMapHeight, iWalkability);

	// 1D array of size (m_iMapWidth * m_iMapHeight + 2) holding ID# of open list items
	m_piOpenListIDs = new int[(size_t)m_iMapWidth * m_iMapHeight + 2];

	for(int i = 0; i < m_iMapWidth * m_iMapHeight + 2; i++)
	{
		m_piOpenListIDs[i] = 0;
	}

	// 2D map of size ([m_iMapWidth + 1][m_iMapHeight + 1]) used to record whether a cell is on the open list or on the closed list
	m_Map2DWhichList.Create(m_iMapWidth + 1, m_iMapHeight + 1, 0);

	// 1D array of size (m_iMapWidth * m_iMapHeight + 2) to store the (x, y) location of an item on the open list
	m_piptOpenListCoordinates_x = new int[(size_t)m_iMapWidth * m_iMapHeight + 2];
	m_piptOpenListCoordinates_y = new int[(size_t)m_iMapWidth * m_iMapHeight + 2];

	// 2D map of size ([m_iMapWidth + 1][m_iMapHeight + 1]) to store parent of each cell (x, y)
	m_Map2DCoordinatesParents_x.Create(m_iMapWidth + 1, m_iMapHeight + 1);
	m_Map2DCoordinatesParents_y.Create(m_iMapWidth + 1, m_iMapHeight + 1);

	// 1D array of size (m_iMapWidth * m_iMapHeight + 2) to store F cost of a cell on the open list
	m_piFCosts = new int[(size_t)m_iMapWidth * m_iMapHeight + 2];

	for(int i = 0; i < m_iMapWidth * m_iMapHeight + 2; i++)
	{
		m_piFCosts[i] = 0;
	}

	// 2D map of size ([m_iMapWidth + 1][m_iMapHeight + 1]) to store G cost for each cell
	m_Map2DGCosts.Create(m_iMapWidth + 1, m_iMapHeight + 1, 0);

	// 1D array of size (m_iMapWidth * m_iMapHeight + 2) to store H cost of a cell on the open list
	m_piHCosts = new int[(size_t)m_iMapWidth * m_iMapHeight + 2];

	for(int i = 0; i < m_iMapWidth * m_iMapHeight + 2; i++)
	{
		m_piHCosts[i] = 0;
	}

	// 1D array of size (m_iPathFinderCount + 1) to store length of the found paths
	m_piPathLengths = new int[(size_t)m_iPathFinderCount + 1];

	for(int i = 0; i < m_iPathFinderCount + 1; i++)
	{
		m_piPathLengths[i] = 0;
	}

	// 1D array of size (m_iPathFinderCount + 1) to store current position of a agent along the chosen path
	m_piPathLocations = new int[(size_t)m_iPathFinderCount + 1];

	for(int i = 0; i < m_iPathFinderCount + 1; i++)
	{
		m_piPathLocations[i] = 0;
	}

	// 2D array of size ([m_iPathFinderCount + 1][4]) for the path bank
	m_ppiPathBank = new int*[(size_t)m_iPathFinderCount + 1];

	int iPathFinderIndex = 0;

	for(iPathFinderIndex = 0; iPathFinderIndex < iPathFinderCount + 1; iPathFinderIndex++)
	{
		m_ppiPathBank[iPathFinderIndex] = new int[4];

		for(int i = 0; i < 4; i++)
		{
			m_ppiPathBank[iPathFinderIndex][i] = 0;
		}
	}

	// 1D array of size (m_iPathFinderCount + 1) to store the x & y coordinates of the first path step
	m_piPathBank_x = new int[(size_t)m_iPathFinderCount + 1];
	m_piPathBank_y = new int[(size_t)m_iPathFinderCount + 1];

	for(int i = 0; i < m_iPathFinderCount + 1; i++)
	{
		m_piPathBank_x[i] = 0;
		m_piPathBank_y[i] = 0;
	}

	// 1D array of size (m_iPathFinderCount + 1) for the path status
	m_piPathStatus = new int[(size_t)m_iPathFinderCount + 1];

	for(int i = 0; i < m_iPathFinderCount + 1; i++)
	{
		m_piPathStatus[i] = 0;
	}

	// 1D array of size (m_iPathFinderCount + 1) for the path coordinates
	m_piptPath_x = new int[(size_t)m_iPathFinderCount + 1];
	m_piptPath_y = new int[(size_t)m_iPathFinderCount + 1];

	for (int i = 0; i < m_iPathFinderCount + 1; i++)
	{
		m_piptPath_x[i] = 0;
		m_piptPath_y[i] = 0;
	}

	// 1D array of size (m_iPathFinderCount + 1) for start locations
	m_piStartLocations_x = new int[(size_t)m_iPathFinderCount + 1];
	m_piStartLocations_y = new int[(size_t)m_iPathFinderCount + 1];

	for (int i = 0; i < m_iPathFinderCount + 1; i++)
	{
		m_piStartLocations_x[i] = 0;
		m_piStartLocations_y[i] = 0;
	}

	// 1D array of size (m_iPathFinderCount + 1) for target locations
	m_piTargetLocations_x = new int[(size_t)m_iPathFinderCount + 1];
	m_piTargetLocations_y = new int[(size_t)m_iPathFinderCount + 1];

	for (int i = 0; i < m_iPathFinderCount + 1; i++)
	{
		m_piTargetLocations_x[i] = 0;
		m_piTargetLocations_y[i] = 0;
	}
	//////////////////////////////////////////////////////////////////////////
}

void CAStar::Destroy()
{
	//////////////////////////////////////////////////////////////////////////
	// Used by CPU

	// 2D map of size ([m_iMapWidth][m_iMapHeight]) to hold the map
	m_Map2DWalkability.Destroy();

	// 1D array of size (m_iMapWidth * m_iMapHeight + 2) holding ID# of open list items
	if(m_piOpenListIDs != nullptr)
	{
		delete [] m_piOpenListIDs;

		m_piOpenListIDs = nullptr;
	}

	// 2D map of size ([m_iMapWidth + 1][m_iMapHeight + 1]) used to record whether a cell is on the open list or on the closed list
	m_Map2DWhichList.Destroy();

	// 1D array of size (m_iMapWidth * m_iMapHeight + 2) to store the (x, y) location of an item on the open list
	if(m_piptOpenListCoordinates_x != nullptr)
	{
		delete [] m_piptOpenListCoordinates_x;

		m_piptOpenListCoordinates_x = nullptr;
	}

	if(m_piptOpenListCoordinates_y != nullptr)
	{
		delete [] m_piptOpenListCoordinates_y;

		m_piptOpenListCoordinates_y = nullptr;
	}

	// 2D map of size ([m_iMapWidth + 1][m_iMapHeight + 1]) to store parent of each cell (x, y)
	m_Map2DCoordinatesParents_x.Destroy();
	m_Map2DCoordinatesParents_y.Destroy();

	// 1D array of size (m_iMapWidth * m_iMapHeight + 2) to store F cost of a cell on the open list
	if(m_piFCosts != nullptr)
	{
		delete [] m_piFCosts;

		m_piFCosts = nullptr;
	}

	// 2D map of size ([m_iMapWidth + 1][m_iMapHeight + 1]) to store G cost for each cell
	m_Map2DGCosts.Destroy();

	// 1D array of size (m_iMapWidth * m_iMapHeight + 2) to store H cost of a cell on the open list
	if(m_piHCosts != nullptr)
	{
		delete [] m_piHCosts;

		m_piHCosts = nullptr;
	}

	// 1D array of size (m_iPathFinderCount + 1) to store length of the found paths
	if(m_piPathLengths != nullptr)
	{
		delete [] m_piPathLengths;

		m_piPathLengths = nullptr;
	}

	// 1D array of size (m_iPathFinderCount + 1) to store current position of a agent along the chosen path
	if(m_piPathLocations != nullptr)
	{
		delete [] m_piPathLocations;

		m_piPathLocations = nullptr;
	}

	// 2D array of size ([m_iPathFinderCount + 1][4]) for the path bank
	if(m_ppiPathBank != nullptr)
	{
		int iPathFinderIndex = 0;
		int iPathFinderCount = m_iPathFinderCount;

		for(iPathFinderIndex = 0; iPathFinderIndex < iPathFinderCount + 1; iPathFinderIndex++)
		{
			if(m_ppiPathBank[iPathFinderIndex] != nullptr)
			{
				delete [] m_ppiPathBank[iPathFinderIndex];

				m_ppiPathBank[iPathFinderIndex] = nullptr;
			}
		}

		delete [] m_ppiPathBank;

		m_ppiPathBank = nullptr;
	}

	// 1D array of size (m_iPathFinderCount + 1) to store the x & y coordinates of the first path step
	if(m_piPathBank_x != nullptr)
	{
		delete [] m_piPathBank_x;

		m_piPathBank_x = nullptr;
	}

	if(m_piPathBank_y != nullptr)
	{
		delete [] m_piPathBank_y;

		m_piPathBank_y = nullptr;
	}

	// 1D array of size (m_iPathFinderCount + 1) for the path status
	if(m_piPathStatus != nullptr)
	{
		delete [] m_piPathStatus;

		m_piPathStatus = nullptr;
	}

	// 1D array of size (m_iPathFinderCount + 1) for the path coordinates
	if(m_piptPath_x != nullptr)
	{
		delete [] m_piptPath_x;

		m_piptPath_x = nullptr;
	}

	if(m_piptPath_y != nullptr)
	{
		delete [] m_piptPath_y;

		m_piptPath_y = nullptr;
	}

	// 1D array of size (m_iPathFinderCount + 1) for start locations
	if(m_piStartLocations_x != nullptr)
	{
		delete [] m_piStartLocations_x;

		m_piStartLocations_x = nullptr;
	}

	if(m_piStartLocations_y != nullptr)
	{
		delete [] m_piStartLocations_y;

		m_piStartLocations_y = nullptr;
	}

	// 1D array of size (m_iPathFinderCount + 1) for target locations
	if(m_piTargetLocations_x != nullptr)
	{
		delete [] m_piTargetLocations_x;

		m_piTargetLocations_x = nullptr;
	}

	if(m_piTargetLocations_y != nullptr)
	{
		delete [] m_piTargetLocations_y;

		m_piTargetLocations_y = nullptr;
	}
	//////////////////////////////////////////////////////////////////////////

	m_iMapWidth = 0;
	m_iMapHeight = 0;

	m_iPathFinderCount = 0;

	m_iOnClosedList = 0;

	m_bStop = false;
}

_Point CAStar::GetCurrentPath(int iPathFinderID)
{
	iPathFinderID++;	// convert 0 based index to 1 based index

	return _Point(m_piptPath_x[iPathFinderID], m_piptPath_y[iPathFinderID]);
}

bool CAStar::GetEntirePath(int iPathFinderID, vector<_Point>& vecptEntirePath)
{
	iPathFinderID++;	// convert 0 based index to 1 based index

	vecptEntirePath.clear();

	// If a path has been found
	if(m_piPathStatus[iPathFinderID] == FOUND)
	{
		_Point _ptNext;

		long lPathIndex = 0;
		long lPathCount = m_piPathLengths[iPathFinderID];

		for(lPathIndex = 0; lPathIndex < lPathCount; lPathIndex++)
		{
			_ptNext.x = m_ppiPathBank[iPathFinderID][lPathIndex * 2 + 0];
			_ptNext.y = m_ppiPathBank[iPathFinderID][lPathIndex * 2 + 1];

			// 			_ptNext.x = m_piPathBank_x[iPathFinderID];
			// 			_ptNext.y = m_piPathBank_y[iPathFinderID];

			vecptEntirePath.push_back(_ptNext);
		}
	}

	if(vecptEntirePath.size() <= 0)
	{
		return false;
	}

	return true;
}

void CAStar::SetStartLocation(int iPathFinderID, _Point& _pt)
{
	iPathFinderID++;	// convert 0 based index to 1 based index

	m_piStartLocations_x[iPathFinderID] = _pt.x;
	m_piStartLocations_y[iPathFinderID] = _pt.y;
}

_Point CAStar::GetStartLocation(int iPathFinderID)
{
	iPathFinderID++;	// convert 0 based index to 1 based index

	return _Point(m_piStartLocations_x[iPathFinderID], m_piStartLocations_y[iPathFinderID]);
}

void CAStar::SetTargetLocation(int iPathFinderID, _Point& _pt)
{
	iPathFinderID++;	// convert 0 based index to 1 based index

	m_piTargetLocations_x[iPathFinderID] = _pt.x;
	m_piTargetLocations_y[iPathFinderID] = _pt.y;
}

_Point CAStar::GetTargetLocation(int iPathFinderID)
{
	iPathFinderID++;	// convert 0 based index to 1 based index

	return _Point(m_piTargetLocations_x[iPathFinderID], m_piTargetLocations_y[iPathFinderID]);
}

void CAStar::FindPath(int iPathFinderID, _Point& _ptStartLocation, _Point& _ptTargetLocation)
{
	iPathFinderID++;	// convert 0 based index to 1 based index

	int iOnOpenList = 0;

	_Point _ptParent;
	_ptParent.x = 0;
	_ptParent.y = 0;

	int a = 0;
	int b = 0;

	int m = 0;

	int u = 0;
	int v = 0;

	int iTemp = 0;
	int iCorner = 0;

	int iOpenListItemsCount = 0;

	int iAddedGCost = 0;
	int iTempGCost = 0;
	int iPath = 0;

	_Point _ptPath;

	int iCellPosition = 0;

	int iNewOpenListItemID = 0;

	bool bExit = false;

	//////////////////////////////////////////////////////////////////////////
	// Copy the location information for easy usage
	int iStartX = _ptStartLocation.x;
	int iStartY = _ptStartLocation.y;

	int iTargetX = _ptTargetLocation.x;
	int iTargetY = _ptTargetLocation.y;

	//////////////////////////////////////////////////////////////////////////
	// Quick path checks :
	// Under some circumstances no path needs to be generated

	// If start and target are in the same location, if target square is un-walkable
	if(iStartX == iTargetX && iStartY == iTargetY && m_piPathLocations[iPathFinderID] > 0)
	{
		m_piptPath_x[iPathFinderID] = iStartX;
		m_piptPath_y[iPathFinderID] = iStartY;

		iPath = FOUND;

		bExit = true;
	}
	else
	if(iStartX == iTargetX && iStartY == iTargetY && m_piPathLocations[iPathFinderID] == 0)
	{
		m_piptPath_x[iPathFinderID] = iStartX;
		m_piptPath_y[iPathFinderID] = iStartY;

		iPath = NON_EXISTENT;

		bExit = true;
	}
	else
	if(m_Map2DWalkability(iTargetX, iTargetY) == UNWALKABLE)
	{
		// If there is no path to the selected target, set the pathfinder's path to its current location & return that the path is nonexistent
		m_piptPath_x[iPathFinderID] = iStartX;
		m_piptPath_y[iPathFinderID] = iStartY;

		iPath = NON_EXISTENT;

		bExit = true;
	}

	//////////////////////////////////////////////////////////////////////////
	// Reset m_Map2DWhichList occasionally
	if(m_iOnClosedList > 1000000)
	{
		m_Map2DWhichList.Reset(0);

		m_iOnClosedList = 10;
	}

	//////////////////////////////////////////////////////////////////////////
	// Changing the values of iOnOpenList and m_iOnClosedList list is faster than redeeming m_Map2DWhichList
	m_iOnClosedList += 2;
	iOnOpenList = m_iOnClosedList - 1;

	m_piPathLengths[iPathFinderID] = NOT_STARTED;
	m_piPathLocations[iPathFinderID] = NOT_STARTED;

	m_Map2DGCosts(iStartX, iStartY) = 0;	// reset starting square's G value to 0

	//////////////////////////////////////////////////////////////////////////
	// Add the starting location to the open list of squares to be checked
	iOpenListItemsCount = 1;

	// Assign it as the top (and currently only) item in the open list, which is maintained as a binary heap (explained below)
	m_piOpenListIDs[1] = 1;
	m_piptOpenListCoordinates_x[1] = iStartX;
	m_piptOpenListCoordinates_y[1] = iStartY;

	// Do the following until a path is found or deemed nonexistent
	do
	{
		if(bExit)
		{
			break;
		}

		// If the open list is not empty, take the first cell off of the list
		// This is the lowest F cost cell on the open list
		if(iOpenListItemsCount != 0)
		{
			// Pop the first item off the open list
			_ptParent.x = m_piptOpenListCoordinates_x[m_piOpenListIDs[1]];
			_ptParent.y = m_piptOpenListCoordinates_y[m_piOpenListIDs[1]];

			// Add the item to the closed list
			m_Map2DWhichList(_ptParent.x, _ptParent.y) = m_iOnClosedList;

			// Open List = Binary Heap: Delete this item from the open list, which is maintained as a binary heap
			iOpenListItemsCount -= 1;

			// Delete the top item in binary heap and reorder the heap, with the lowest F cost item rising to the top
			// Move the last item in the heap up to slot #1
			m_piOpenListIDs[1] = m_piOpenListIDs[iOpenListItemsCount + 1];
			v = 1;

			// Repeat the following until the new item in slot #1 sinks to its proper spot in the heap
			do
			{
				u = v;

				if(2 * u + 1 <= iOpenListItemsCount)	// if both children exist
				{
					// Check if the F cost of the parent is greater than each child
					// Select the lowest of the two children
					if(m_piFCosts[m_piOpenListIDs[u]] >= m_piFCosts[m_piOpenListIDs[2 * u]])
					{
						v = 2 * u;
					}

					if(m_piFCosts[m_piOpenListIDs[v]] >= m_piFCosts[m_piOpenListIDs[2 * u + 1]])
					{
						v = 2 * u + 1;
					}
				}
				else
				{
					// If only child #1 exists
					if(2 * u <= iOpenListItemsCount)
					{
						// Check if the F cost of the parent is greater than child #1	
						if(m_piFCosts[m_piOpenListIDs[u]] >= m_piFCosts[m_piOpenListIDs[2 * u]])
						{
							v = 2 * u;
						}
					}
				}

				// If parent's F is > one of its children, swap them
				if(u != v)
				{
					iTemp = m_piOpenListIDs[u];
					m_piOpenListIDs[u] = m_piOpenListIDs[v];
					m_piOpenListIDs[v] = iTemp;
				}
				else
				{
					break;
				}
			}
			while(!m_bStop);	// Reorder the binary heap

			// Check the adjacent squares
			for(b = _ptParent.y - 1; b <= _ptParent.y + 1; b++)
			{
				for(a = _ptParent.x - 1; a <= _ptParent.x + 1; a++)
				{
					// Check if its not off the map
					if(a >= 0 && b >= 0 && a < m_iMapWidth && b < m_iMapHeight)
					{
						// If not already on the closed list (items on the closed list have already been considered and can now be ignored)
						if(m_Map2DWhichList(a, b) != m_iOnClosedList)
						{
							// If not a wall / obstacle square
							if(m_Map2DWalkability(a, b) != UNWALKABLE)
							{
								// Don't cut across corners
								iCorner = WALKABLE;

								if(a == _ptParent.x - 1) 
								{
									if(b == _ptParent.y - 1)
									{
										if(	m_Map2DWalkability(_ptParent.x - 1, _ptParent.y) == UNWALKABLE ||
											m_Map2DWalkability(_ptParent.x, _ptParent.y - 1) == UNWALKABLE)
										{
											iCorner = UNWALKABLE;
										}
									}
									else
									if(b == _ptParent.y + 1)
									{
										if(	m_Map2DWalkability(_ptParent.x, _ptParent.y + 1) == UNWALKABLE ||
											m_Map2DWalkability(_ptParent.x - 1, _ptParent.y) == UNWALKABLE)
										{
											iCorner = UNWALKABLE;
										}
									}
								}
								else
								if(a == _ptParent.x + 1)
								{
									if(b == _ptParent.y - 1)
									{
										if(	m_Map2DWalkability(_ptParent.x, _ptParent.y - 1) == UNWALKABLE ||
											m_Map2DWalkability(_ptParent.x + 1, _ptParent.y) == UNWALKABLE)
										{
											iCorner = UNWALKABLE;
										}
									}
									else
									if(b == _ptParent.y + 1)
									{
										if(	m_Map2DWalkability(_ptParent.x + 1, _ptParent.y) == UNWALKABLE ||
											m_Map2DWalkability(_ptParent.x, _ptParent.y + 1) == UNWALKABLE)
										{
											iCorner = UNWALKABLE;
										}
									}
								}

								if(iCorner == WALKABLE)
								{
									// If not already on the open list, add it to the open list
									if(m_Map2DWhichList(a, b) != iOnOpenList) 
									{	
										// Create a new open list item in the binary heap
										// Each new item has a unique ID#
										iNewOpenListItemID += 1;

										m = iOpenListItemsCount + 1;

										// Place the new open list item (actually, its ID#) at the bottom of the heap
										m_piOpenListIDs[m] = iNewOpenListItemID;

										// Record the x and y coordinates of the new item
										m_piptOpenListCoordinates_x[iNewOpenListItemID] = a;
										m_piptOpenListCoordinates_y[iNewOpenListItemID] = b;

										// Calculate its G cost
										if(abs(a - _ptParent.x) == 1 && abs(b - _ptParent.y) == 1)
										{
											// Cost of going to diagonal squares
											iAddedGCost = 14;

											if(m_Map2DWalkability(a, b) == OCCUPIED)
											{
												iAddedGCost = 141;
											}
										}
										else
										{
											// Cost of going to non-diagonal squares
											iAddedGCost = 10;

											if(m_Map2DWalkability(a, b) == OCCUPIED)
											{
												iAddedGCost = 100;
											}
										}

										m_Map2DGCosts(a, b) = m_Map2DGCosts(_ptParent.x, _ptParent.y) + iAddedGCost;

										// Calculate its H and F costs and parent
										m_piHCosts[m_piOpenListIDs[m]] = 10 * (abs(a - iTargetX) + abs(b - iTargetY));
										m_piFCosts[m_piOpenListIDs[m]] = m_Map2DGCosts(a, b) + m_piHCosts[m_piOpenListIDs[m]];

										m_Map2DCoordinatesParents_x(a, b) = _ptParent.x;
										m_Map2DCoordinatesParents_y(a, b) = _ptParent.y;

										// Move the new open list item to the proper place in the binary heap
										// While item hasn't bubbled to the top (m = 1)
										while(m != 1)
										{
											// Check if child's F cost is < parent's F cost, if so, swap them
											if(m_piFCosts[m_piOpenListIDs[m]] <= m_piFCosts[m_piOpenListIDs[m / 2]])
											{
												iTemp = m_piOpenListIDs[m / 2];
												m_piOpenListIDs[m / 2] = m_piOpenListIDs[m];
												m_piOpenListIDs[m] = iTemp;
												m = m / 2;
											}
											else
											{
												break;
											}
										}

										// Add one to the number of items in the heap
										iOpenListItemsCount += 1;

										// Change m_Map2DWhichList to show that the new item is on the open list
										m_Map2DWhichList(a, b) = iOnOpenList;
									}

									// If adjacent cell is already on the open list,
									// check to see if this path to that cell from the starting location is a better one,
									// if so, change the parent of the cell and its G and F costs
									else // if(m_Map2DWhichList(a,b) == iOnOpenList)
									{
										// Calculate the G cost of this possible new path
										if(abs(a - _ptParent.x) == 1 && abs(b - _ptParent.y) == 1)
										{
											// Cost of going to diagonal tiles
											iAddedGCost = 14;

											if(m_Map2DWalkability(a, b) == OCCUPIED)
											{
												iAddedGCost += 50;
											}
										}
										else
										{
											// Cost of going to non-diagonal tiles
											iAddedGCost = 10;

											if(m_Map2DWalkability(a, b) == OCCUPIED)
											{
												iAddedGCost += 35;
											}
										}

										iTempGCost = m_Map2DGCosts(_ptParent.x, _ptParent.y) + iAddedGCost;

										// If this path is shorter (G cost is lower)
										// then change the parent cell, G cost and F cost
										if(iTempGCost < m_Map2DGCosts(a, b))
										{
											// Change the square's parent
											m_Map2DCoordinatesParents_x(a, b) = _ptParent.x;
											m_Map2DCoordinatesParents_y(a, b) = _ptParent.y;

											m_Map2DGCosts(a, b) = iTempGCost;	// Change the G cost

											// Changing the G cost also changes the F cost,
											for(int x = 1; x <= iOpenListItemsCount; x++) //look for the item in the heap
											{
												 // Item found
												if(m_piptOpenListCoordinates_x[m_piOpenListIDs[x]] == a && m_piptOpenListCoordinates_y[m_piOpenListIDs[x]] == b)
												{
													// Change the F cost
													m_piFCosts[m_piOpenListIDs[x]] = m_Map2DGCosts(a, b) + m_piHCosts[m_piOpenListIDs[x]];

													// See if changing the F score bubbles the item up from it's current location in the heap
													m = x;

													// While item hasn't bubbled to the top (m = 1)
													while(m != 1)
													{
														// Check if child is < parent, if so, swap them
														if(m_piFCosts[m_piOpenListIDs[m]] < m_piFCosts[m_piOpenListIDs[m / 2]])
														{
															iTemp = m_piOpenListIDs[m / 2];
															m_piOpenListIDs[m / 2] = m_piOpenListIDs[m];
															m_piOpenListIDs[m] = iTemp;
															m = m / 2;
														}
														else
														{
															break;
														}
													}

													break;
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}

		// If open list is empty then there is no path
		else
		{
			iPath = NON_EXISTENT;
			break;
		}  

		// If target is added to open list then path has been found
		if(m_Map2DWhichList(iTargetX, iTargetY) == iOnOpenList)
		{
			iPath = FOUND;
			break;
		}
	}
	while(!m_bStop);	// Do until path is found or deemed nonexistent

	// Save the path if it exists
	if(iPath == FOUND && !bExit)
	{
		// Working backwards from the target to the starting location by checking each cell's parent,
		// calculating the length of the path
		_ptPath.x = iTargetX;
		_ptPath.y = iTargetY;

		int iX = 0;
		int iY = 0;

		do
		{
			// Look up the parent of the current cell
			iX = m_Map2DCoordinatesParents_x(_ptPath.x, _ptPath.y);
			iY = m_Map2DCoordinatesParents_y(_ptPath.x, _ptPath.y);

			_ptPath.x = iX;
			_ptPath.y = iY;

			// Calculate the path length
			m_piPathLengths[iPathFinderID] += 1;
		}
		while(_ptPath.x != iStartX || _ptPath.y != iStartY);

		// Resize the data bank to the right size in bytes
		if(m_ppiPathBank[iPathFinderID] != nullptr)
		{
			delete [] m_ppiPathBank[iPathFinderID];
		}

		m_ppiPathBank[iPathFinderID] = new int[(size_t)m_piPathLengths[iPathFinderID] * 2];

		// Now copy the path information over to the path bank
		// Since we are working backwards from the target to the start location,
		// we copy the information to the path bank in reverse order
		// The result is a properly ordered set of path data,
		// from the first step to the last
		_ptPath.x = iTargetX;
		_ptPath.y = iTargetY;

		iCellPosition = m_piPathLengths[iPathFinderID] * 2;	// Start at the end

		do
		{
			iCellPosition -= 2;	// Work backwards 2 integers

			m_ppiPathBank[iPathFinderID][iCellPosition] = _ptPath.x;
			m_ppiPathBank[iPathFinderID][iCellPosition + 1] = _ptPath.y;

			m_piPathBank_x[iPathFinderID] = _ptPath.x;
			m_piPathBank_y[iPathFinderID] = _ptPath.y;

			// Look up the parent of the current cell
			iX = m_Map2DCoordinatesParents_x(_ptPath.x, _ptPath.y);
			iY = m_Map2DCoordinatesParents_y(_ptPath.x, _ptPath.y);

			_ptPath.x = iX;
			_ptPath.y = iY;

			// If we have reached the starting square, exit the loop
		}
		while(_ptPath.x != iStartX || _ptPath.y != iStartY);

		// Read the first path step
		ReadPath(iPathFinderID, _ptStartLocation);
	}

	m_piPathStatus[iPathFinderID] = iPath;
}

void CAStar::ReadPath(int iPathFinderID, _Point& _ptCurrentLocation)
{
	// note: iPathFinderID is a 1 based index

	// If a path has been found
	if(m_piPathStatus[iPathFinderID] == FOUND)
	{
		// If path finder is just starting a new path or has reached the
		// center of the current path square (and the end of the path hasn't been reached),
		// look up the next path square
		if(m_piPathLocations[iPathFinderID] < m_piPathLengths[iPathFinderID])
		{
			int iNextPathLocation = m_piPathLocations[iPathFinderID] + 1;

			_Point _ptNext;

			_ptNext.x = m_ppiPathBank[iPathFinderID][iNextPathLocation * 2 - 2];
			_ptNext.y = m_ppiPathBank[iPathFinderID][iNextPathLocation * 2 - 1];

// 			_ptNext.x = m_piPathBank_x[iPathFinderID];
// 			_ptNext.y = m_piPathBank_y[iPathFinderID];

			if(m_Map2DWalkability(_ptNext) != OCCUPIED)
			{
				m_piPathLocations[iPathFinderID] += 1;

				m_piptPath_x[iPathFinderID] = _ptNext.x;
				m_piptPath_y[iPathFinderID] = _ptNext.y;
			}
			else
			{
				m_piptPath_x[iPathFinderID] = _ptCurrentLocation.x;
				m_piptPath_y[iPathFinderID] = _ptCurrentLocation.y;
			}

			m_Map2DWalkability(m_piptPath_x[iPathFinderID], m_piptPath_y[iPathFinderID]) = OCCUPIED;
		}

		// If the last path square on the path has been reached then reset
		if(m_piPathLocations[iPathFinderID] == m_piPathLengths[iPathFinderID])
		{
			m_piPathStatus[iPathFinderID] = NOT_STARTED;
		}
	}

	// If there is no path, simply stay in the current location
	else
	{
		m_piptPath_x[iPathFinderID] = _ptCurrentLocation.x;
		m_piptPath_y[iPathFinderID] = _ptCurrentLocation.y;
	}
}

int CAStar::GetWidth()
{
	return m_iMapWidth;
}

int CAStar::GetHeight()
{
	return m_iMapHeight;
}

int CAStar::GetPathFinderCount()
{
	return m_iPathFinderCount;
}

void CAStar::SetMapWalkability(_Point& _pt, int iValue)
{
	m_Map2DWalkability(_pt) = iValue;
}

int CAStar::GetMapWalkability(_Point& _pt)
{
	return m_Map2DWalkability(_pt);
}

void CAStar::DrawLine(_Point& _ptPrevious, _Point& _ptNext, int iValue, bool bIncludeEnd)
{
	m_Map2DWalkability.DrawLine(_ptPrevious, _ptNext, iValue, bIncludeEnd);
}

void CAStar::FloodFill(_Point& _pt, int iValue)
{
	m_Map2DWalkability.FloodFill(_pt, iValue);
}

bool CAStar::IsValidLocation(_Point& _pt)
{
	if (IsValid(_pt) && GetMapWalkability(_pt) != UNWALKABLE)
	{
		return true;
	}

	return false;
}

bool CAStar::IsDeadEnd(_Point& _pt)
{
	int iWalls = 0;

	_Point ptNorth(_pt.x, _pt.y + 1);
	if (!IsValidLocation(ptNorth))
	{
		iWalls++;
	}

	_Point ptSouth(_pt.x, _pt.y - 1);
	if (!IsValidLocation(ptSouth))
	{
		iWalls++;
	}

	_Point ptEast(_pt.x + 1, _pt.y);
	if (!IsValidLocation(ptEast))
	{
		iWalls++;
	}

	_Point ptWest(_pt.x - 1, _pt.y);
	if (!IsValidLocation(ptWest))
	{
		iWalls++;
	}

	if (IsValidLocation(_pt) && iWalls == 3)
	{
		return true;
	}

	return false;
}

_Point CAStar::GetLocation()
{
	_Point _pt;

	if(!IsReady())
	{
		return _pt;
	}

	do 
	{
		_pt.x = rand() % m_iMapWidth;
		_pt.y = rand() % m_iMapHeight;

		if(GetMapWalkability(_pt) != UNWALKABLE)
		{
			break;
		}
	}
	while(true);

	return _pt;
}

_Point CAStar::GetLocation(int iValue)
{
	_Point _pt;

	if (!IsReady())
	{
		return _pt;
	}

	do
	{
		_pt.x = rand() % m_iMapWidth;
		_pt.y = rand() % m_iMapHeight;

		if (GetMapWalkability(_pt) == iValue)
		{
			break;
		}
	}
	while(true);

	return _pt;
}

_Point CAStar::GetLocationOdd()
{
	_Point _pt;

	if (!IsReady())
	{
		return _pt;
	}

	do
	{
		_pt.x = rand() % m_iMapWidth;
		_pt.y = rand() % m_iMapHeight;

		if (GetMapWalkability(_pt) != UNWALKABLE && (_pt.x % 2) == 1 && (_pt.y % 2) == 1)
		{
			break;
		}
	} while (true);

	return _pt;
}

_Point CAStar::GetLocationOdd(int iValue)
{
	_Point _pt;

	if (!IsReady())
	{
		return _pt;
	}

	do
	{
		_pt.x = rand() % m_iMapWidth;
		_pt.y = rand() % m_iMapHeight;

		if (GetMapWalkability(_pt) == iValue && (_pt.x % 2) == 1 && (_pt.y % 2) == 1)
		{
			break;
		}
	} while (true);

	return _pt;
}

_Point CAStar::GetLocationEven()
{
	_Point _pt;

	if (!IsReady())
	{
		return _pt;
	}

	do
	{
		_pt.x = rand() % m_iMapWidth;
		_pt.y = rand() % m_iMapHeight;

		if (GetMapWalkability(_pt) != UNWALKABLE && (_pt.x % 2) == 0 && (_pt.y % 2) == 0)
		{
			break;
		}
	} while (true);

	return _pt;
}

_Point CAStar::GetLocationEven(int iValue)
{
	_Point _pt;

	if (!IsReady())
	{
		return _pt;
	}

	do
	{
		_pt.x = rand() % m_iMapWidth;
		_pt.y = rand() % m_iMapHeight;

		if (GetMapWalkability(_pt) == iValue && (_pt.x % 2) == 0 && (_pt.y % 2) == 0)
		{
			break;
		}
	} while (true);

	return _pt;
}

void CAStar::PrintWalkability(const char* pstrFilePath /* = nullptr*/)
{
	string strFilePath = "__map.txt";

	if(pstrFilePath != nullptr)
	{
		strFilePath = pstrFilePath;
	}

	string strFileData = "";

	for(int y = 0; y < m_Map2DWalkability.GetHeight(); y++)
	{
		for(int x = 0; x < m_Map2DWalkability.GetWidth(); x++)
		{
			strFileData += CUtil::toStringA(m_Map2DWalkability(x, y));
			strFileData += " ";
		}

		strFileData += "\r\n";
	}

	CUtil::writeFile(strFilePath, strFileData);
}
