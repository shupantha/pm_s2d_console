#pragma once

#include "_Structures.h"

//////////////////////////////////////////////////////////////////////////
class CMap2D
{
private:
	int* m_piMap;

	int m_iWidth;
	int m_iHeight;

public:
	CMap2D(void)	{ m_piMap = nullptr; };
	CMap2D(int iWidth, int iHeight) { Create(iWidth, iHeight); };

	~CMap2D(void) { Destroy(); };

public:
	CMap2D & operator = (CMap2D& M)
	{
		if(this == &M)		// Both objects are the same
		{
			return *this;	// Skip the assignment, and just return *this
		}

		int iWidth = M.GetWidth();
		int iHeight = M.GetHeight();

		if(m_iWidth == iWidth && m_iHeight == iHeight && m_iWidth > 0 && m_iHeight > 0)
		{
			memcpy(m_piMap, M.GetMemory(), m_iHeight * m_iWidth * sizeof(int));

			return *this;
		}

		if(IsLoaded())
		{
			Destroy();
		}

		Create(iWidth, iHeight);

		memcpy(m_piMap, M.GetMemory(), m_iHeight * m_iWidth * sizeof(int));

		return *this;
	};

	int & operator () (int iX, int iY)
	{
		if(!IsOK(iX, iY))
		{
			throw;
		}

		return m_piMap[iX + (iY * m_iWidth)];
	};

	int operator () (int iX, int iY) const
	{
		if(!IsOK(iX, iY))
		{
			throw;
		}

		return m_piMap[iX + (iY * m_iWidth)];
	};

	int & operator () (_Point& _pt)
	{
		if(!IsOK(_pt))
		{
			throw;
		}

		return m_piMap[_pt.x + (_pt.y * m_iWidth)];
	};

	int operator () (_Point& _pt) const
	{
		if(!IsOK(_pt))
		{
			throw;
		}

		return m_piMap[_pt.x + (_pt.y * m_iWidth)];
	};

public:
	int* GetMemory() { return m_piMap; };

public:
	bool IsLoaded()
	{
		if(m_piMap == nullptr)
		{
			return false;
		}

		if(m_iWidth <= 0 || m_iHeight <= 0)
		{
			return false;
		}

		return true;
	};

	bool IsOK(int iX, int iY) const
	{
		if(m_piMap == nullptr || m_iWidth <= 0 || m_iHeight <= 0)
		{
			return false;
		}

		if(iX < 0 || iY < 0 || iX >= m_iWidth || iY >= m_iHeight)
		{
			return false;
		}

		return true;
	};

	bool IsOK(_Point& _pt) const
	{
		if(m_piMap == nullptr || m_iWidth <= 0 || m_iHeight <= 0)
		{
			return false;
		}

		if(_pt.x < 0 || _pt.y < 0 || _pt.x >= m_iWidth || _pt.y >= m_iHeight)
		{
			return false;
		}

		return true;
	};

public:
	long GetWidth() { return m_iWidth; };
	long GetHeight() { return m_iHeight; };

	bool Create(int iWidth, int iHeight, int iInitialValue = 0)
	{
		if(iWidth <= 0 || iHeight <= 0)
		{
			return false;
		}

		if(IsLoaded())
		{
			Destroy();
		}

		m_piMap = new int[iWidth * iHeight];

		if(m_piMap == nullptr)
		{
			return false;
		}

		m_iWidth = iWidth;
		m_iHeight = iHeight;

		Reset(iInitialValue);

		return true;
	};

	void Destroy()
	{
		if(m_piMap != nullptr)
		{
			delete [] m_piMap;

			m_piMap = nullptr;
		}

		m_iWidth = 0;
		m_iHeight = 0;
	};

	void SetEntry(int iX, int iY, int iValue)
	{
		if(!IsOK(iX, iY))
		{
			throw;
		}

		m_piMap[iX + (iY * m_iWidth)] = iValue;
	};

	void SetEntry(_Point& _pt, int iValue)
	{
		if(!IsOK(_pt))
		{
			throw;
		}

		m_piMap[_pt.x + (_pt.y * m_iWidth)] = iValue;
	};

	int GetEntry(int iX, int iY)
	{
		if(!IsOK(iX, iY))
		{
			throw;
		}

		return m_piMap[iX + (iY * m_iWidth)];
	};

	int GetEntry(_Point& _pt)
	{
		if(!IsOK(_pt))
		{
			throw;
		}

		return m_piMap[_pt.x + (_pt.y * m_iWidth)];
	};

	void Reset(int iValue)
	{
		if(!IsLoaded())
		{
			return;
		}

		memset(m_piMap, iValue, m_iWidth * m_iHeight * sizeof(int));
	};

	void DrawLine(_Point _ptStart, _Point _ptEnd, int iColor, bool bIncludeEnd = true)
	{
		if(!IsLoaded())
		{
			return;
		}

		int iDistance = 0;

		int iXError = 0;
		int iYError = 0;
		
		int iDeltaX = 0;
		int iDeltaY = 0;

		int iCX = 0;
		int iCY = 0;

		// calculate the distances in both directions
		iDeltaX = _ptEnd.x - _ptStart.x;
		iDeltaY = _ptEnd.y - _ptStart.y;

		// calculate the direction of the increment, an increment of 0 means either a horizontal or vertical line
		if(iDeltaX > 0)
		{
			iCX = 1;
		}
		else
		if(iDeltaX == 0)
		{
			iCX = 0;
		}
		else
		{
			iCX = -1;
		}

		if(iDeltaY > 0)
		{
			iCY = 1;
		}
		else
		if(iDeltaY == 0)
		{
			iCY = 0;
		}
		else
		{
			iCY = -1;
		}

		// determine which distance is greater
		iDeltaX = abs(iDeltaX);
		iDeltaY = abs(iDeltaY);

		if(iDeltaX > iDeltaY)
		{
			iDistance = iDeltaX;
		}
		else
		{
			iDistance = iDeltaY;
		}

		// if the end point is included, then the effective distance is +1
		int iEffectiveDistance = bIncludeEnd ? iDistance + 1 : iDistance;

		// draw the line
		for(int i = 0; i <= iEffectiveDistance; i++)
		{
			SetEntry(_ptStart.x, _ptStart.y, iColor);

			iXError += iDeltaX;
			iYError += iDeltaY;

			if(iXError > iDistance)
			{
				iXError -= iDistance;

				_ptStart.x += iCX;
			}

			if(iYError > iDistance)
			{
				iYError -= iDistance;

				_ptStart.y += iCY;
			}
		}
	};

	void FloodFill(_Point& _pt, int iColor)
	{
		if (_pt.y < 0 || _pt.y > m_iHeight - 1 || _pt.x < 0 || _pt.x > m_iWidth - 1)
		{
			return;
		}

		SeedFill(_pt.x, _pt.y, iColor);
	};

private:
	void SeedFill(int x, int y, int iFillColor)
	{
		if(iFillColor != GetEntry(x, y))
		{
			_Point _pt = _Point(x, y);
			SetEntry(_pt, iFillColor);

			SeedFill(x - 1, y, iFillColor);
			SeedFill(x + 1, y, iFillColor);
			SeedFill(x, y - 1, iFillColor);
			SeedFill(x, y + 1, iFillColor);
		}
	};
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
class CMap2DCoordinates
{
private:
	_Point* m_pptMap;

	int m_iWidth;
	int m_iHeight;

public:
	CMap2DCoordinates(void)	{ m_pptMap = nullptr; };
	CMap2DCoordinates(int iWidth, int iHeight) { Create(iWidth, iHeight, _Point()); };

	~CMap2DCoordinates(void) { Destroy(); };

public:
	CMap2DCoordinates & operator = (CMap2DCoordinates& M)
	{
		if(this == &M)		// Both objects are the same
		{
			return *this;	// Skip the assignment, and just return *this
		}

		int iWidth = M.GetWidth();
		int iHeight = M.GetHeight();

		if(m_iWidth == iWidth && m_iHeight == iHeight && m_iWidth > 0 && m_iHeight > 0)
		{
			memcpy(m_pptMap, M.GetMemory(), m_iHeight * m_iWidth * sizeof(_Point));

			return *this;
		}

		if(IsLoaded())
		{
			Destroy();
		}

		Create(iWidth, iHeight, _Point());

		memcpy(m_pptMap, M.GetMemory(), m_iHeight * m_iWidth * sizeof(_Point));

		return *this;
	};

	_Point & operator () (int iX, int iY)
	{
		if(!IsOK(iX, iY))
		{
			throw;
		}

		return m_pptMap[iX + (iY * m_iWidth)];
	};

	_Point operator () (int iX, int iY) const
	{
		if(!IsOK(iX, iY))
		{
			throw;
		}

		return m_pptMap[iX + (iY * m_iWidth)];
	};

	_Point & operator () (_Point& _pt)
	{
		if(!IsOK(_pt))
		{
			throw;
		}

		return m_pptMap[_pt.x + (_pt.y * m_iWidth)];
	};

	_Point operator () (_Point& _pt) const
	{
		if(!IsOK(_pt))
		{
			throw;
		}

		return m_pptMap[_pt.x + (_pt.y * m_iWidth)];
	};

public:
	_Point* GetMemory() { return m_pptMap; };

public:
	bool IsLoaded()
	{
		if(m_pptMap == nullptr)
		{
			return false;
		}

		if(m_iWidth <= 0 || m_iHeight <= 0)
		{
			return false;
		}

		return true;
	};

	bool IsOK(int iX, int iY) const
	{
		if(m_pptMap == nullptr || m_iWidth <= 0 || m_iHeight <= 0)
		{
			return false;
		}

		if(iX < 0 || iY < 0 || iX >= m_iWidth || iY >= m_iHeight)
		{
			return false;
		}

		return true;
	};

	bool IsOK(_Point& _pt) const
	{
		if(m_pptMap == nullptr || m_iWidth <= 0 || m_iHeight <= 0)
		{
			return false;
		}

		if(_pt.x < 0 || _pt.y < 0 || _pt.x >= m_iWidth || _pt.y >= m_iHeight)
		{
			return false;
		}

		return true;
	};

public:
	long GetWidth() { return m_iWidth; };
	long GetHeight() { return m_iHeight; };

	bool Create(int iWidth, int iHeight, _Point _ptInitialValue)
	{
		if(iWidth <= 0 || iHeight <= 0)
		{
			return false;
		}

		if(IsLoaded())
		{
			Destroy();
		}

		m_pptMap = new _Point[iWidth * iHeight];

		if(m_pptMap == nullptr)
		{
			return false;
		}

		m_iWidth = iWidth;
		m_iHeight = iHeight;

		Reset(_ptInitialValue);

		return true;
	};

	void Destroy()
	{
		if(m_pptMap != nullptr)
		{
			delete [] m_pptMap;

			m_pptMap = nullptr;
		}

		m_iWidth = 0;
		m_iHeight = 0;
	};

	void SetEntry(int iX, int iY, _Point _ptValue)
	{
		if(!IsOK(iX, iY))
		{
			throw;
		}

		m_pptMap[iX + (iY * m_iWidth)] = _ptValue;
	};

	void SetEntry(_Point& _pt, _Point _ptValue)
	{
		if(!IsOK(_pt))
		{
			throw;
		}

		m_pptMap[_pt.x + (_pt.y * m_iWidth)] = _ptValue;
	};

	_Point GetEntry(int iX, int iY)
	{
		if(!IsOK(iX, iY))
		{
			throw;
		}

		return m_pptMap[iX + (iY * m_iWidth)];
	};

	_Point GetEntry(_Point& _pt)
	{
		if(!IsOK(_pt))
		{
			throw;
		}

		return m_pptMap[_pt.x + (_pt.y * m_iWidth)];
	};

	void Reset(_Point& _ptValue)
	{
		if(!IsLoaded())
		{
			return;
		}

		int iIndex = 0;
		int iCount = m_iWidth * m_iHeight;

		for(iIndex = 0; iIndex < iCount; iIndex++)
		{
			this->m_pptMap[iIndex] = _ptValue;
		}
	};

	void DrawLine(_Point _ptStart, _Point _ptEnd, _Point _ptColor)
	{
		if(!IsLoaded())
		{
			return;
		}

		int iDistance = 0;

		int iXError = 0;
		int iYError = 0;
		
		int iDeltaX = 0;
		int iDeltaY = 0;

		int iCX = 0;
		int iCY = 0;

		// calculate the distances in both directions
		iDeltaX = _ptEnd.x - _ptStart.x;
		iDeltaY = _ptEnd.y - _ptStart.y;

		// calculate the direction of the increment, an increment of 0 means either a horizontal or vertical line
		if(iDeltaX > 0)
		{
			iCX = 1;
		}
		else
		if(iDeltaX == 0)
		{
			iCX = 0;
		}
		else
		{
			iCX = -1;
		}

		if(iDeltaY > 0)
		{
			iCY = 1;
		}
		else
		if(iDeltaY == 0)
		{
			iCY = 0;
		}
		else
		{
			iCY = -1;
		}

		// determine which distance is greater
		iDeltaX = abs(iDeltaX);
		iDeltaY = abs(iDeltaY);

		if(iDeltaX > iDeltaY)
		{
			iDistance = iDeltaX;
		}
		else
		{
			iDistance = iDeltaY;
		}

		// draw the line
		for(int i = 0; i <= iDistance + 1; i++)
		{
			SetEntry(_ptStart.x, _ptStart.y, _ptColor);

			iXError += iDeltaX;
			iYError += iDeltaY;

			if(iXError > iDistance)
			{
				iXError -= iDistance;

				_ptStart.x += iCX;
			}

			if(iYError > iDistance)
			{
				iYError -= iDistance;

				_ptStart.y += iCY;
			}
		}
	};
};
//////////////////////////////////////////////////////////////////////////
