/*
//////////////////////////////////////////////////////////////////////////
A high resolution timer - 1 us (10 ** -6) (microsecond) accuracy
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
COPYRIGHT NOTICE, DISCLAIMER, and LICENSE:
//////////////////////////////////////////////////////////////////////////

CUtil : Copyright (C) 2008, Shupantha Kazi Imam (shupantha@yahoo.com)

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

V 1.1
*/

#include "stdafx.h"

#include "Timer.h"

///////////////////////////////////////////////////////////////////////////////
// constructor
///////////////////////////////////////////////////////////////////////////////
CTimer::CTimer()
{
	Reset();
}

CTimer::~CTimer()
{
	// Do nothing!
}

void CTimer::Start()
{
	m_bStop = false;

	m_start = std::chrono::high_resolution_clock::now();
	m_end = m_start;
}

void CTimer::Stop()
{
	m_bStop = true;

	m_end = std::chrono::high_resolution_clock::now();
}

void CTimer::Reset()
{
	m_bStop = true;

	m_start = std::chrono::high_resolution_clock::now();
	m_end = m_start;
}

double CTimer::GetElapsedTime()
{
	if(!m_bStop)
	{
		m_end = std::chrono::high_resolution_clock::now();
	}

	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(m_end - m_start);

	return (elapsed.count() * 0.000001);
}
