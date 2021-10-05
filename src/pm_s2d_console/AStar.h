#pragma once

#include "Map2D.h"

#include <vector>

class CAStar
{
private:
	//////////////////////////////////////////////////////////////////////////
	int m_iMapWidth;
	int m_iMapHeight;
	
	int m_iPathFinderCount;

	int m_iOnClosedList;

	bool m_bStop;

	bool m_bUseCUDA;
	//////////////////////////////////////////////////////////////////////////

public:
	void Stop();
	void Start();

public:
	// Path related constants
	static const int NOT_STARTED	= 0;

	static const int FOUND			= 1;
	static const int NON_EXISTENT	= 2;

	// Walk ability constants
	static const int WALKABLE		= 0;
	static const int UNWALKABLE		= 1;
	static const int OCCUPIED		= 2;

private:
	//////////////////////////////////////////////////////////////////////////
	// Used for path finding

	// 2D map of size ([m_iMapWidth][m_iMapHeight]) to hold the map
	CMap2D m_Map2DWalkability;

	// 1D array of size (m_iMapWidth * m_iMapHeight + 2) holding ID# of open list items
	int* m_piOpenListIDs;

	// 2D map of size ([m_iMapWidth + 1][m_iMapHeight + 1]) used to record whether a cell is on the open list or on the closed list
	CMap2D m_Map2DWhichList;

	// 1D array of size (m_iMapWidth * m_iMapHeight + 2) to store the (x, y) location of an item on the open list
	int* m_piptOpenListCoordinates_x;
	int* m_piptOpenListCoordinates_y;

	// 2D map of size ([m_iMapWidth + 1][m_iMapHeight + 1]) to store parent of each cell (x, y)
	CMap2D m_Map2DCoordinatesParents_x;
	CMap2D m_Map2DCoordinatesParents_y;

	// 1D array of size (m_iMapWidth * m_iMapHeight + 2) to store F cost of a cell on the open list
	int* m_piFCosts;

	// 2D map of size ([m_iMapWidth + 1][m_iMapHeight + 1]) to store G cost for each cell
	CMap2D m_Map2DGCosts;

	// 1D array of size (m_iMapWidth * m_iMapHeight + 2) to store H cost of a cell on the open list
	int* m_piHCosts;

	// 1D array of size (m_iPathFinderCount + 1) to store length of the found paths
	int* m_piPathLengths;

	// 1D array of size (m_iPathFinderCount + 1) to store current position of a agent along the chosen path
	int* m_piPathLocations;

	// 2D array of size ([m_iPathFinderCount + 1][4]) for the path bank
	int** m_ppiPathBank;

	// 1D array of size (m_iPathFinderCount + 1) to store the x & y coordinates of the first path step
	int* m_piPathBank_x;
	int* m_piPathBank_y;

	// 1D array of size (m_iPathFinderCount + 1) for the path status
	int* m_piPathStatus;

	// 1D array of size (m_iPathFinderCount + 1) for the path coordinates
	int* m_piptPath_x;
	int* m_piptPath_y;

	// 1D array of size (m_iPathFinderCount + 1) for start locations
	int* m_piStartLocations_x;
	int* m_piStartLocations_y;

	// 1D array of size (m_iPathFinderCount + 1) for target locations
	int* m_piTargetLocations_x;
	int* m_piTargetLocations_y;
	//////////////////////////////////////////////////////////////////////////

public:
	CAStar(void);
	~CAStar(void);

public:
	CAStar & operator = (CAStar& AStar)
	{
		if(this == &AStar)	// Both objects are the same
		{
			return *this;	// Skip the assignment, and just return *this
		}

		this->Initialize(AStar.GetWidth(), AStar.GetHeight(), AStar.GetPathFinderCount());

		return *this;
	};

public:
	bool IsReady();

	bool IsValid(_Point _pt);

private:
	void Initialize();

public:
	void Initialize(int iMapWidth, int iMapHeight, int iPathFinderCount, int iWalkability = WALKABLE);

	void Destroy();

public:
	_Point GetCurrentPath(int iPathFinderID);

	bool GetEntirePath(int iPathFinderID, std::vector<_Point>& vecptEntirePath);

	void SetStartLocation(int iPathFinderID, _Point& _pt);
	_Point GetStartLocation(int iPathFinderID);

	void SetTargetLocation(int iPathFinderID, _Point& _pt);
	_Point GetTargetLocation(int iPathFinderID);

public:
	void FindPath(int iPathFinderID, _Point& _ptStartLocation, _Point& _ptTargetLocation);

private:
	void ReadPath(int iPathFinderID, _Point& _ptCurrentLocation);

public:
	void SetWidth(int iMapWidth);
	int GetWidth();

	void SetHeight(int iMapHeight);
	int GetHeight();

	void SetPathFinderCount(int iPathFinderCount);
	int GetPathFinderCount();

	void SetMapWalkability(_Point& _pt, int iValue);

	int GetMapWalkability(_Point& _pt);

	void DrawLine(_Point& _ptPrevious, _Point& _ptNext, int iValue, bool bIncludeEnd);

	void FloodFill(_Point& _pt, int iValue);

public:
	bool IsValidLocation(_Point& _pt);

	bool IsDeadEnd(_Point& _pt);

	_Point GetLocation();
	_Point GetLocation(int iValue);

	_Point GetLocationOdd();
	_Point GetLocationOdd(int iValue);

	_Point GetLocationEven();
	_Point GetLocationEven(int iValue);

	void PrintWalkability(const char* pstrFilePath = nullptr);
};
