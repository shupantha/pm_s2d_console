#pragma once

#include <math.h>
#include <cmath>
#include <cstring>

#define EPSILON			0.000000001
#define INVALID_VALUE	0xffffffff

//////////////////////////////////////////////////////////////////////////
struct _Color
{
	float r, g, b, a;

public:
	_Color(void)
	{
		r = 0.0f;
		g = 0.0f;
		b = 0.0f;
		a = 0.0f;
	};

	_Color(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a)
	{
		Set(_r, _g, _b, _a);
	};

	~_Color(void) {};

public:
	void Set(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a)
	{
		r = (float)_r / 255.0f;
		g = (float)_g / 255.0f;
		b = (float)_b / 255.0f;
		a = (float)_a / 255.0f;
	};

	unsigned char GetR() { return (unsigned char)(r * 255.0f); };
	unsigned char GetG() { return (unsigned char)(g * 255.0f); };
	unsigned char GetB() { return (unsigned char)(b * 255.0f); };
	unsigned char GetA() { return (unsigned char)(a * 255.0f); };
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
struct _Point
{
	int x;
	int y;

	_Point() { x = 0; y = 0; };
	_Point(int iX, int iY) { x = iX; y = iY; };

	~_Point() { };

public:
	bool operator == (const _Point& _pt) const
	{
		if(x == _pt.x && y == _pt.y)
		{
			return true;
		}

		return false;
	};

	bool operator != (const _Point& _pt) const
	{
		return !(*this == _pt);
	};

	bool operator < (const _Point& _pt) const
	{
		// practical hash, not robust
		int n0 = x * 1000 + y;
		int n1 = _pt.x * 1000 + _pt.y;

		if (n0 < n1)
		{
			return true;
		}

		return false;
	};

public:
	void Scale(double dScaleX, double dScaleY)
	{
		x = (int) (x * dScaleX);
		y = (int) (y * dScaleY);
	};

public:
	double GetDistanceFrom(const _Point& _pt) const
	{
		double dDistance = ((double)x - _pt.x) * ((double)x - _pt.x) + ((double)y - _pt.y) * ((double)y - _pt.y);
		dDistance = sqrt(dDistance);

		return dDistance;
	};

	bool IsNear(const _Point& _pt, double dDistance = 1.0) const
	{
		return (GetDistanceFrom(_pt) <= dDistance);
	};

	bool IsNearFast(const _Point& _pt, int iD = 1) const
	{
		int iDistance = (x - _pt.x) * (x - _pt.x) + (y - _pt.y) * (y - _pt.y);

		return (iDistance <= (iD * iD));
	}
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Vertex
class _VERTEX3D
{
public:
	double x, y, z;

public:
	_VERTEX3D(void)
	{
		x = 0.0;
		y = 0.0;
		z = 0.0;
	};

	_VERTEX3D(double dx, double dy, double dz)
	{
		x = dx;
		y = dy;
		z = dz;
	};

	~_VERTEX3D(void) {};

private:
	bool IsEqual(double A, double B) const
	{
		if(A == B)
		{
			return true;
		}

		if(abs(A - B) <= EPSILON)
		{
			return true;
		}

		return false;
	};

public:
	bool operator == (const _VERTEX3D& _v) const
	{
		if(IsEqual(x, _v.x) && IsEqual(y, _v.y) && IsEqual(z, _v.z))
		{
			return true;
		}

		return false;
	};

	bool operator != (const _VERTEX3D& _v) const
	{
		return !(*this == _v);
	};

	bool operator < (const _VERTEX3D& _v) const
	{
		// practical hash, not robust
		double n0 = x * 1000000 + y * 1000 + z;
		double n1 = _v.x * 1000000 + _v.y * 1000 + _v.z;

		if (n0 < n1)
		{
			return true;
		}

		return false;
	};

public:
	void Scale(double dScaleX, double dScaleY, double dScaleZ)
	{
		x = x * dScaleX;
		y = y * dScaleY;
		z = z * dScaleZ;
	};

public:
	double GetDistanceFrom(const _VERTEX3D& _v) const
	{
		double dDistance = (x - _v.x) * (x - _v.x) + (y - _v.y) * (y - _v.y) + (z - _v.z) * (z - _v.z);
		dDistance = sqrt(dDistance);

		return dDistance;
	};

	bool IsNear(const _VERTEX3D& _v, double dDistance = 1.0) const
	{
		return (GetDistanceFrom(_v) <= dDistance);
	};
};
//////////////////////////////////////////////////////////////////////////