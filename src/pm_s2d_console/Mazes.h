/*
//////////////////////////////////////////////////////////////////////////
COPYRIGHT NOTICE, DISCLAIMER, and LICENSE:
//////////////////////////////////////////////////////////////////////////

mazeGenerator : Copyright (C) 2014, Shupantha Kazi Imam (shupantha@yahoo.com)

//////////////////////////////////////////////////////////////////////////
Covered code is provided under this license on an "as is" basis, without
warranty of any kind, either expressed or implied, including, without
limitation, warranties that the covered code is free of defects,
merchantable, fit for a particular purpose or non-infringing. The entire
risk as to the quality and performance of the covered code is with you.
Should any covered code prove defective in any respect, you (not the
initial developer or any other contributor) assume the cost of any
necessary servicing, repair or correction. This disclaimer of warranty
constitutes an essential part of this license. No use of any covered code
is authorized hereunder except under this disclaimer.

Permission is hereby granted to use, copy, modify, and distribute this
source code, or portions hereof, for any purpose, including commercial
applications, freely and without fee, subject to the following
restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. This notice may not be removed or altered from any source distribution.
//////////////////////////////////////////////////////////////////////////

V 1.0
*/

#pragma once

#include <stdio.h>
#include <math.h>

namespace mazes
{
	//--------------------------------------------------------------------------------------------------
	enum directions { NONE = 0, NORTH = 1, EAST = 2, SOUTH = 4, WEST = 8 };

	enum tiles
	{
		cross_road = 0,
		t_junction_north,
		t_junction_east,
		corner_north_east,
		t_junction_south,
		road_horizontal,
		corner_south_east,
		deadend_east,
		t_junction_west,
		corner_north_west,
		road_vertical,
		deadend_north,
		corner_south_west,
		deadend_west,
		deadend_south,
		invalid
	};

	//--------------------------------------------------------------------------------------------------
	class mazeGenerator
	{
	private:
		unsigned char* _pWorld;
		int _sX;
		int _sY;
		int _ptX;
		int _ptY;
		int _cellSize;
		int _width;
		int _height;

	public:
		mazeGenerator()
		{
			_pWorld = nullptr;

			_sX = 0;
			_sY = 0;

			_ptX = 0;
			_ptY = 0;

			_cellSize = 0;

			_width = 0;
			_height = 0;
		}

		~mazeGenerator() { killArray(); };

		void generate(int w, int h, int c)
		{
			killArray();

			if (w < 2) w = 2;
			if (h < 2) h = 2;
			if (c < 2) c = 2;

			_sX = w / c;
			_sY = h / c;

			_cellSize = c;

			_width = _sX * c + 1;
			_height = _sY * c + 1;

			generate();
		}

		tiles getTile(int x, int y)
		{
			if (x >= _sX || y >= _sY)
			{
				return tiles::invalid;
			}

			if (_pWorld == nullptr)
			{
				return tiles::invalid;
			}

			unsigned char b = _pWorld[x + _sX * y];

			return (tiles)(~b & 0x0f);
		}

		const unsigned char* getMaze() { return _pWorld; };

		int getCellSize() { return _cellSize; };

		int getMazeSizeX() { return _sX; };
		int getMazeSizeY() { return _sY; };

		int getWidth() { return _width; };
		int getHeight() { return _height; };

	private:
		void generate()
		{
			killArray();
			_pWorld = new unsigned char[(_sX * _sY) + 1];
			memset(_pWorld, 0, (_sX * _sY) + 1);
			_ptX = rand() % _sX;
			_ptY = rand() % _sY;
			carve();
		}

		void carve()
		{
			while (true)
			{
				int d = getDirection();
				if (d < NORTH)
				{
					return;
				}

				int index = _ptX + _sX * _ptY;
				if (index < 0 || index >= (_sX * _sY))
				{
					return;
				}

				switch (d)
				{
				case NORTH:
					_pWorld[_ptX + _sX * _ptY] |= NORTH;
					_ptY--;
					_pWorld[_ptX + _sX * _ptY] = SOUTH | SOUTH << 4;
					break;
				case EAST:
					_pWorld[_ptX + _sX * _ptY] |= EAST;
					_ptX++;
					_pWorld[_ptX + _sX * _ptY] = WEST | WEST << 4;
					break;
				case SOUTH:
					_pWorld[_ptX + _sX * _ptY] |= SOUTH;
					_ptY++;
					_pWorld[_ptX + _sX * _ptY] = NORTH | NORTH << 4;
					break;
				case WEST:
					_pWorld[_ptX + _sX * _ptY] |= WEST;
					_ptX--;
					_pWorld[_ptX + _sX * _ptY] = EAST | EAST << 4;
				}
			}
		}

		int getDirection()
		{
			int d = 1 << rand() % 4;
			while (true)
			{
				for (int x = 0; x < 4; ++x)
				{
					if (testDir(d))
					{
						return d;
					}
					d <<= 1;

					if (d > 8)
					{
						d = 1;
					}
				}

				int index = _ptX + _sX * _ptY;
				if (index < 0 || index >= (_sX * _sY))
				{
					return -1;
				}

				d = (_pWorld[index] & 0xf0) >> 4;
				if (d == NONE)
				{
					return -1;
				}

				switch (d)
				{
				case NORTH: _ptY--;
					break;
				case EAST: _ptX++;
					break;
				case SOUTH: _ptY++;
					break;
				case WEST: _ptX--;
					break;
				}
				d = 1 << rand() % 4;
			}
		}

		bool testDir(int d)
		{
			switch (d)
			{
			case NORTH: return (_ptY - 1 > -1 && !_pWorld[_ptX + _sX * (_ptY - 1)]);
			case EAST: return (_ptX + 1 < _sX && !_pWorld[_ptX + 1 + _sX * _ptY]);
			case SOUTH: return (_ptY + 1 < _sY && !_pWorld[_ptX + _sX * (_ptY + 1)]);
			case WEST: return (_ptX - 1 > -1 && !_pWorld[_ptX - 1 + _sX * _ptY]);
			}
			return false;
		}

		void killArray()
		{
			if (_pWorld)
			{
				delete[] _pWorld;
			}

			_pWorld = nullptr;
		}
	};
}
