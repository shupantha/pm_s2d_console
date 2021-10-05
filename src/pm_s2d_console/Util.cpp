/*
//////////////////////////////////////////////////////////////////////////
COPYRIGHT NOTICE, DISCLAIMER, and LICENSE:
//////////////////////////////////////////////////////////////////////////

Copyright (C) 2016, Shupantha Kazi Imam (shupantha@yahoo.com)

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

v9.00
*/

#include "stdafx.h"

#include "Util.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <locale>
#include <codecvt>
#include <sstream>
#include <iomanip>
#include <memory>
#include <chrono>
#include <thread>

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <malloc.h>
#include <tchar.h>
#include <wchar.h>
#include <time.h>
#include <ctime>
#include <direct.h>
#include <math.h>

#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#include <wininet.h>
#endif

#ifdef _WIN32
#include <io.h>
#include <direct.h>
#define access _access
#define mkdir(a,b) _mkdir(a)
#else
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#define _getcwd getcwd
#endif

CUtil::CUtil(void)
{
}

CUtil::~CUtil(void)
{
}

//////////////////////////////////////////////////////////////////////////
// File IO functions
//////////////////////////////////////////////////////////////////////////
bool CUtil::readFile(const wstring& _strFilePath, wstring& _strFileData)
{
	return readFile(getStringA(_strFilePath), _strFileData);
}

bool CUtil::readFile(const wstring& _strFilePath, string& _strFileData)
{
	return readFile(getStringA(_strFilePath), _strFileData);
}

bool CUtil::readFile(const string& _strFilePath, wstring& _strFileData)
{
	std::wifstream fs(_strFilePath, std::ios::in | std::ios::binary);
	if (fs)
	{
		fs.imbue(std::locale(std::locale::classic(), new std::codecvt_utf8<wchar_t>));
		std::wstringstream ss;
		ss << fs.rdbuf();
		fs.close();
		_strFileData = ss.str();
		return true;
	}
	return false;
}

bool CUtil::readFile(const string& _strFilePath, string& _strFileData)
{
	std::ifstream fs(_strFilePath, std::ios::in | std::ios::binary);
	if (fs)
	{
		fs.imbue(std::locale(""));
		std::stringstream ss;
		ss << fs.rdbuf();
		fs.close();
		_strFileData = ss.str();
		return true;
	}
	return false;
}

bool CUtil::writeFile(const wstring& _strFilePath, const wstring& _strFileData)
{
	return writeFile(getStringA(_strFilePath), _strFileData);
}

bool CUtil::writeFile(const wstring& _strFilePath, const string& _strFileData)
{
	return writeFile(getStringA(_strFilePath), _strFileData);
}

bool CUtil::writeFile(const string& _strFilePath, const wstring& _strFileData)
{
	std::wofstream fs(_strFilePath, std::ios::out | std::ios::binary | std::ios::trunc);
	if (fs)
	{
		fs.imbue(std::locale(std::locale::classic(), new std::codecvt_utf8<wchar_t>));
		fs << _strFileData;
		fs.close();
		return true;
	}
	return false;
}

bool CUtil::writeFile(const string& _strFilePath, const string& _strFileData)
{
	std::ofstream fs(_strFilePath, std::ios::out | std::ios::binary | std::ios::trunc);
	if (fs)
	{
		fs.imbue(std::locale(""));
		fs << _strFileData;
		fs.close();
		return true;
	}
	return false;
}

bool CUtil::appendFile(const wstring& _strFilePath, const wstring& _strFileData)
{
	return appendFile(getStringA(_strFilePath), _strFileData);
}

bool CUtil::appendFile(const wstring& _strFilePath, const string& _strFileData)
{
	return appendFile(getStringA(_strFilePath), _strFileData);
}

bool CUtil::appendFile(const string& _strFilePath, const wstring& _strFileData)
{
	std::wofstream fs(_strFilePath, std::ios::out | std::ios::binary | std::ios::app);
	if (fs)
	{
		fs.imbue(std::locale(std::locale::classic(), new std::codecvt_utf8<wchar_t>));
		fs << _strFileData;
		fs.close();
		return true;
	}
	return false;
}

bool CUtil::appendFile(const string& _strFilePath, const string& _strFileData)
{
	std::ofstream fs(_strFilePath, std::ios::out | std::ios::binary | std::ios::app);
	if (fs)
	{
		fs.imbue(std::locale(""));
		fs << _strFileData;
		fs.close();
		return true;
	}
	return false;
}

int CUtil::getFileSize(const wstring& _strFilePath)
{
	return getFileSize(getStringA(_strFilePath));
}

int CUtil::getFileSize(const string& _strFilePath)
{
	// Get the file size using better/faster method
	int iFileSize = (int)getFileSize64(_strFilePath);

	if (iFileSize > 0)
	{
		return iFileSize;
	}

	// If the previous method fails, try again the old fashioned way
	FILE* pFile = _fsopen(_strFilePath.c_str(), "rb", _SH_DENYNO);

	if (pFile == nullptr)
	{
		// File does not exist
		return -1;
	}
	else
	{
		// Get the file size
		fseek(pFile, 0, SEEK_END);
		iFileSize = ftell(pFile);
		rewind(pFile);

		// Close the file handle and free the buffer
		fclose(pFile);

		return iFileSize;
	}
}

__int64 CUtil::getFileSize64(const wstring& _strFilePath)
{
	return getFileSize64(getStringA(_strFilePath));
}

__int64 CUtil::getFileSize64(const string& _strFilePath)
{
	struct __stat64 fileStat;

	if (_stat64(_strFilePath.c_str(), &fileStat) != 0)
	{
		return -1;
	}

	return fileStat.st_size;
}

bool CUtil::isFile(const wstring& _strFilePath)
{
	return isFile(getStringA(_strFilePath));
}

bool CUtil::isFile(const string& _strFilePath)
{
	std::ifstream ifsFile(_strFilePath.c_str());

	return (!ifsFile.bad());
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// String manipulation functions
//////////////////////////////////////////////////////////////////////////
int CUtil::findReplace(wstring& _strSource, const wstring& _strFind, const wstring& _strReplace)
{
	if (_strFind.empty())
	{
		return -1;
	}

	int iCount = 0;

	wstring::size_type nPos = 0;

	while (wstring::npos != (nPos = _strSource.find(_strFind, nPos)))
	{
		_strSource.replace(nPos, _strFind.size(), _strReplace);
		nPos += _strReplace.size();

		iCount++;
	}

	return iCount;
}

int CUtil::findReplace(string& _strSource, const string& _strFind, const string& _strReplace)
{
	if (_strFind.empty())
	{
		return -1;
	}

	int iCount = 0;

	string::size_type nPos = 0;

	while (string::npos != (nPos = _strSource.find(_strFind, nPos)))
	{
		_strSource.replace(nPos, _strFind.size(), _strReplace);
		nPos += _strReplace.size();

		iCount++;
	}

	return iCount;
}

void CUtil::removeWhiteSpaces(wstring& _strSource, const wstring& _strReplace /* = L""*/)
{
	findReplace(_strSource, L" ", _strReplace);
	findReplace(_strSource, L"\t", _strReplace);
	findReplace(_strSource, L"\r", _strReplace);
	findReplace(_strSource, L"\n", _strReplace);
}

void CUtil::removeWhiteSpaces(string& _strSource, const string& _strReplace /* = ""*/)
{
	findReplace(_strSource, " ", _strReplace);
	findReplace(_strSource, "\t", _strReplace);
	findReplace(_strSource, "\r", _strReplace);
	findReplace(_strSource, "\n", _strReplace);
}

wstring CUtil::createPadding(unsigned int _uiLength, const wstring& _strChars2Pad /* = L" "*/)
{
	wstring strPadding = L"";
	for (unsigned int ui = 0; ui < _uiLength; ui++)
	{
		strPadding += _strChars2Pad;
	}
	return strPadding;
}

string CUtil::createPadding(unsigned int _uiLength, const string& _strChars2Pad /* = " "*/)
{
	string strPadding = "";
	for (unsigned int ui = 0; ui < _uiLength; ui++)
	{
		strPadding += _strChars2Pad;
	}
	return strPadding;
}

wstring CUtil::removeGarbage(wstring& _strSource, wchar_t _wch /* = L'\0'*/)
{
	for (size_t i = 0; i < _strSource.size(); i++)
	{
		if (!isprint(_strSource[i]))
		{
			_strSource[i] = _wch;
		}
	}
	return wstring(_strSource.c_str());
}

string CUtil::removeGarbage(string& _strSource, char _ch /* = '\0'*/)
{
	for (size_t i = 0; i < _strSource.size(); i++)
	{
		if (!isprint(_strSource[i]))
		{
			_strSource[i] = _ch;
		}
	}
	return string(_strSource.c_str());
}

wstring CUtil::trim(wstring& _strSource, const wstring& _strChars2Trim /* = L" \t\r\n"*/, bool _bTrimLeft /* = true*/, bool _bTrimRight /* = true*/)
{
	size_t nStartIndex = _strSource.find_first_not_of(_strChars2Trim);

	if (nStartIndex == wstring::npos)
	{
		_strSource.erase();
		return _strSource;
	}

	if (_bTrimLeft)
	{
		_strSource = _strSource.substr(nStartIndex, _strSource.size() - nStartIndex);
	}

	if (_bTrimRight)
	{
		_strSource = _strSource.substr(0, _strSource.find_last_not_of(_strChars2Trim) + 1);
	}

	return _strSource;
}

string CUtil::trim(string& _strSource, const string& _strChars2Trim /* = " \t\r\n"*/, bool _bTrimLeft /* = true*/, bool _bTrimRight /* = true*/)
{
	size_t nStartIndex = _strSource.find_first_not_of(_strChars2Trim);

	if (nStartIndex == string::npos)
	{
		_strSource.erase();
		return _strSource;
	}

	if (_bTrimLeft)
	{
		_strSource = _strSource.substr(nStartIndex, _strSource.size() - nStartIndex);
	}

	if (_bTrimRight)
	{
		_strSource = _strSource.substr(0, _strSource.find_last_not_of(_strChars2Trim) + 1);
	}

	return _strSource;
}

wstring CUtil::format(const wstring _str, ...)
{
	int iN0 = (int)_str.size() * 8;  // reserve 8 times as much space, just in case :-)
	int iNx = 0;

	std::unique_ptr<wchar_t[]> pstrFormatted;
	va_list va;

	while (true)
	{
		pstrFormatted.reset(new wchar_t[iN0]);
		wcscpy_s(&pstrFormatted[0], iN0, _str.c_str());

		va_start(va, _str);
		iNx = _vsnwprintf_s(&pstrFormatted[0], iN0, iN0, _str.c_str(), va);
		va_end(va);

		if (iNx < 0 || iNx >= iN0)
		{
			iN0 += abs(iNx - iN0 + 1);
		}
		else
		{
			break;
		}
	}

	return wstring(pstrFormatted.get());
}

string CUtil::format(const string _str, ...)
{
	int iN0 = (int)_str.size() * 8;  // reserve 8 times as much space, just in case :-)
	int iNx = 0;

	std::unique_ptr<char[]> pstrFormatted;
	va_list va;

	while (true)
	{
		pstrFormatted.reset(new char[iN0]);
		strcpy_s(&pstrFormatted[0], iN0, _str.c_str());

		va_start(va, _str);
		iNx = _vsnprintf_s(&pstrFormatted[0], iN0, iN0, _str.c_str(), va);
		va_end(va);

		if (iNx < 0 || iNx >= iN0)
		{
			iN0 += abs(iNx - iN0 + 1);
		}
		else
		{
			break;
		}
	}

	return string(pstrFormatted.get());
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Find URL
//////////////////////////////////////////////////////////////////////////
wstring CUtil::findURL(const wstring& _strSource)
{
	wstring strURL = _strSource;

	size_t nIndex = strURL.find(L"http://");
	strURL.erase(0, nIndex);

	if (strURL.empty())
	{
		return L"";
	}

	nIndex = strURL.find_first_of(L" \t\r\n\v\f");
	strURL.erase(nIndex);

	return strURL;
}

string CUtil::findURL(const string& _strSource)
{
	string strURL = _strSource;

	size_t nIndex = strURL.find("http://");
	strURL.erase(0, nIndex);

	if (strURL.empty())
	{
		return "";
	}

	nIndex = strURL.find_first_of(" \t\r\n\v\f");
	strURL.erase(nIndex);

	return strURL;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// String tokenizing functions
//////////////////////////////////////////////////////////////////////////
size_t CUtil::getTokenCount(const wstring& _strSource, const wstring& _strDelimiter)
{
	CTokenizer<wstring> tokenizer(_strSource, _strDelimiter);
	return tokenizer.getTokenCount();
}

size_t CUtil::getTokenCount(const string& _strSource, const string& _strDelimiter)
{
	CTokenizer<string> tokenizer(_strSource, _strDelimiter);
	return tokenizer.getTokenCount();
}

wstring CUtil::getToken(const wstring& _strSource, const wstring& _strDelimiter, size_t _nTokenIndex)
{
	CTokenizer<wstring> tokenizer(_strSource, _strDelimiter);
	return tokenizer.getToken(_nTokenIndex);
}

string CUtil::getToken(const string& _strSource, const string& _strDelimiter, size_t _nTokenIndex)
{
	CTokenizer<string> tokenizer(_strSource, _strDelimiter);
	return tokenizer.getToken(_nTokenIndex);
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Array operations
//////////////////////////////////////////////////////////////////////////
int CUtil::findInArray(const wstring* _parrstr, int _iparrstrSize, const wstring& _strFind)
{
	int ipos = -1;
	for (int i = 0; i < _iparrstrSize; i++)
	{
		if (CUtil::compareNoCase(_strFind, _parrstr[i]) == 0)
		{
			ipos = i;
			break;
		}
	}
	return ipos;
}

int CUtil::findInArray(const string* _parrstr, int _iparrstrSize, const string& _strFind)
{
	int ipos = -1;
	for (int i = 0; i < _iparrstrSize; i++)
	{
		if (CUtil::compareNoCase(_strFind, _parrstr[i]) == 0)
		{
			ipos = i;
			break;
		}
	}
	return ipos;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Clipboard
//////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
bool CUtil::copyToClipboard(const _tstring& _strText)
{
	string strTextAscii = "";
#ifdef UNICODE
	strTextAscii = getStringA(_strText);
#else
	strTextAscii = _strText;
#endif
	bool bSuccess = false;

	if (OpenClipboard(nullptr) == TRUE)
	{
		// Empty & lock the Clipboard & free memory associated with any previous data
		EmptyClipboard();

		// Allocate global memory for the data
		HGLOBAL hClipboardData = GlobalAlloc(GMEM_ZEROINIT, strTextAscii.size() + 1);

		if (hClipboardData != nullptr)
		{
			// Get a pointer to the data associated with the handle
			char* pstrData = (char*)GlobalLock(hClipboardData);

			if (pstrData != nullptr)
			{
				// Copy the local data into the global memory
				strcpy_s(pstrData, strTextAscii.size() + 1, strTextAscii.c_str());

				// Unlock the global memory
				GlobalUnlock(hClipboardData);

				// Specify that the data is ANSI text
				SetClipboardData(CF_TEXT, hClipboardData);

				bSuccess = true;
			}
		}

		// Close the clipboard, unlocking it for access to other applications
		CloseClipboard();
	}

	return bSuccess;
}

_tstring CUtil::pasteFromClipboard()
{
	string strTextAscii = "";

	if (OpenClipboard(nullptr) == TRUE)
	{
		// Retrieve the Clipboard data, specifying ANSI text
		HANDLE hClipboardData = GetClipboardData(CF_TEXT);

		if (hClipboardData != nullptr)
		{
			// Get a pointer to the data associated with the handle
			char* pstrData = (char*)GlobalLock(hClipboardData);

			if (pstrData != nullptr)
			{
				strTextAscii = pstrData;

				// Unlock the global memory
				GlobalUnlock(hClipboardData);
			}
		}

		// Close the clipboard, unlocking it for access to other applications
		CloseClipboard();
	}

	_tstring strText = _T("");
#ifdef UNICODE
	strText = getStringW(strTextAscii);
#else
	strText = strTextAscii;
#endif
	return strText;
}
#endif
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Numeric functions
//////////////////////////////////////////////////////////////////////////
bool CUtil::isNear(double _d1, double _d2, double _dThreshold /* = 0.1*/)
{
	if (abs(_d1 - _d2) <= _dThreshold)
	{
		return true;
	}
	return false;
}

unsigned int CUtil::numberOfDigits(int _iNumber)
{
	return ((unsigned int)log10((double)abs(_iNumber)) + 1);
}

double CUtil::round(double _dNumber2Round, unsigned int _iPlaces)
{
	if (_iPlaces >= 12)
	{
		return _dNumber2Round;
	}

	double dMultiple = 1.0;
	double dResult = _dNumber2Round;

	dMultiple = pow(10.0, (double)_iPlaces);

	// multiply the original number to 'move' the decimal point to the right
	dResult *= dMultiple;

	if (dResult < 0)
	{
		dResult -= 0.5;
	}
	else
	{
		dResult += 0.5;
	}

	// drop off everything past the decimal point
	dResult = floor(dResult);

	// 'move' the decimal point back to where it started
	dResult /= dMultiple;

	return dResult;
}

int CUtil::round(double _dNumber2Round)
{
	double dResult = _dNumber2Round;

	if (dResult < 0)
	{
		dResult -= 0.5;
	}
	else
	{
		dResult += 0.5;
	}

	return (int)dResult;
}

bool CUtil::isPowerOf2(unsigned int _uiNumber)
{
	return ((_uiNumber & (_uiNumber - 1)) == 0);
}

unsigned int CUtil::getNextPowerOf2(unsigned int _uiNumber)
{
	unsigned int n = _uiNumber - 1;
	n |= n >> 1;
	n |= n >> 2;
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;
	n += 1;

	return n;
}

wstring CUtil::toStringW(int _iNumber, unsigned int _uiPrecision)
{
	return getStringW(toStringA(_iNumber, _uiPrecision));
}

string CUtil::toStringA(int _iNumber, unsigned int _uiPrecision)
{
	std::string strNumber = "";
	if (_iNumber < 0)
	{
		strNumber += "-";
	}

	unsigned int uiNumberOfDigits = numberOfDigits(_iNumber);
	for (unsigned int ui = 0; ui < (_uiPrecision - uiNumberOfDigits); ++ui)
	{
		strNumber += "0";
	}
	strNumber += toStringA(abs((int)_iNumber));

	return strNumber;
}

wstring CUtil::toStringW(int _iNumber)
{
	return getStringW(toStringA(_iNumber));
}

string CUtil::toStringA(int _iNumber)
{
	std::ostringstream strStream;
	strStream << _iNumber;

	return strStream.str();
}

wstring CUtil::toStringW(__int64 _i64Number)
{
	return getStringW(toStringA(_i64Number));
}

string CUtil::toStringA(__int64 _i64Number)
{
	std::ostringstream strStream;
	strStream << _i64Number;

	return strStream.str();
}

wstring CUtil::toStringW(double _dNumber, unsigned int _uiPrecision)
{
	return getStringW(toStringA(_dNumber, _uiPrecision));
}

string CUtil::toStringA(double _dNumber, unsigned int _uiPrecision)
{
	std::ostringstream strStream;

	if (_uiPrecision <= 0)
	{
		strStream << _dNumber;
	}
	else
	{
		strStream << std::fixed;
		strStream << std::setprecision(_uiPrecision) << _dNumber;
	}

	return strStream.str();
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// String case converters, upper <-> lower
//////////////////////////////////////////////////////////////////////////
wstring CUtil::toUpper(const wstring& _str)
{
	wstring strUpper = _str;
	std::transform(strUpper.begin(), strUpper.end(), strUpper.begin(), toupper);
	return strUpper;
}

string CUtil::toUpper(const string& _str)
{
	string strUpper = _str;
	std::transform(strUpper.begin(), strUpper.end(), strUpper.begin(), toupper);
	return strUpper;
}

wstring CUtil::toLower(const wstring& _str)
{
	wstring strLower = _str;
	std::transform(strLower.begin(), strLower.end(), strLower.begin(), tolower);
	return strLower;
}

string CUtil::toLower(const string& _str)
{
	string strLower = _str;
	std::transform(strLower.begin(), strLower.end(), strLower.begin(), tolower);
	return strLower;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Compare without case sensitivity
//////////////////////////////////////////////////////////////////////////
int CUtil::compareNoCase(const wstring& _str1, const wstring& _str2)
{
	return toUpper(_str1).compare(toUpper(_str2));
}

int CUtil::compareNoCase(const string& _str1, const string& _str2)
{
	return toUpper(_str1).compare(toUpper(_str2));
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Compare with wildcard
//////////////////////////////////////////////////////////////////////////
bool CUtil::compareWildcard(const wchar_t* _pstr, const wchar_t* _pstrWildcard)
{
	while (*_pstrWildcard)
	{
		if (!*_pstr)
		{
			return *_pstrWildcard == L'*' && !*(_pstrWildcard + 1);
		}

		if (*_pstrWildcard == L'*')
		{
			return compareWildcard(_pstr, _pstrWildcard + 1) || compareWildcard(_pstr + 1, _pstrWildcard);
		}

		if (!(*_pstrWildcard == L'?' || _totupper(*_pstr) == _totupper(*_pstrWildcard)))
		{
			return false;
		}

		_pstr++;
		_pstrWildcard++;
	}

	return !*_pstr;
}

bool CUtil::compareWildcard(const char* _pstr, const char* _pstrWildcard)
{
	while (*_pstrWildcard)
	{
		if (!*_pstr)
		{
			return *_pstrWildcard == '*' && !*(_pstrWildcard + 1);
		}

		if (*_pstrWildcard == '*')
		{
			return compareWildcard(_pstr, _pstrWildcard + 1) || compareWildcard(_pstr + 1, _pstrWildcard);
		}

		if (!(*_pstrWildcard == '?' || _totupper(*_pstr) == _totupper(*_pstrWildcard)))
		{
			return false;
		}

		_pstr++;
		_pstrWildcard++;
	}

	return !*_pstr;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// System time retrieval
//////////////////////////////////////////////////////////////////////////
wstring CUtil::getSystemTimeW(const wstring& _strDateFormat /* = L"%A, %d/%m/%Y, %H:%M:%S - %Z"*/)
{
	time_t ttCurrentTime = 0;
	time(&ttCurrentTime);

	return getSystemTimeW(ttCurrentTime, _strDateFormat);
}

string CUtil::getSystemTimeA(const string& _strDateFormat /* = "%A, %d/%m/%Y, %H:%M:%S - %Z"*/)
{
	time_t ttCurrentTime = 0;
	time(&ttCurrentTime);

	return getSystemTimeA(ttCurrentTime, _strDateFormat);
}

wstring CUtil::getSystemTimeW(const time_t& _ttTime, const wstring& _strDateFormat /* = L"%A, %d/%m/%Y, %H:%M:%S - %Z"*/)
{
	return getStringW(getSystemTimeA(_ttTime, getStringA(_strDateFormat)));
}

string CUtil::getSystemTimeA(const time_t& _ttTime, const string& _strDateFormat /* = "%A, %d/%m/%Y, %H:%M:%S - %Z"*/)
{
	string strTime = "";
	tm tmTime;

	if (localtime_s(&tmTime, &_ttTime) == 0)
	{
		char strBuffer[MAX_PATH];

		strftime(strBuffer, sizeof(strBuffer), _strDateFormat.c_str(), &tmTime);

		strTime = strBuffer;
	}

	return strTime;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Convert given number of seconds to hh:mm:ss and vice versa
//////////////////////////////////////////////////////////////////////////
wstring CUtil::getTimeW(int _iSeconds)
{
	return getStringW(getTimeA(_iSeconds));
}

string CUtil::getTimeA(int _iSeconds)
{
	char strBuffer[16];
	sprintf_s(strBuffer, 16, "%02d:%02d:%02d\0", _iSeconds / 3600, (_iSeconds % 3600) / 60, _iSeconds % 60);

	return string(strBuffer);
}

int CUtil::getTimeW(const wstring& _strTime /* = L"00:00:00"*/)
{
	return getTimeA(getStringA(_strTime));
}

int CUtil::getTimeA(const string& _strTime /* = "00:00:00"*/)
{
	if (getTokenCount(_strTime, ":") != 3)
	{
		return 0;
	}
	std::string strTime = getToken(_strTime, ":", 0);
	int iHour = atol(trim(strTime).c_str());

	strTime = getToken(_strTime, ":", 1);
	int iMinute = atol(trim(strTime).c_str());

	strTime = getToken(_strTime, ":", 2);
	int iSecond = atol(trim(strTime).c_str());

	return ((iHour * 60 * 60) + (iMinute * 60) + iSecond);
}

time_t CUtil::makeTimeW(const wstring& _strDateTime /* = L"00/00/0000, 00:00:00"*/)
{
	return makeTimeA(getStringA(_strDateTime));
}

time_t CUtil::makeTimeA(const string& _strDateTime /* = "00/00/0000, 00:00:00"*/)
{
	string strDate = getToken(_strDateTime, ",", 0);
	removeWhiteSpaces(strDate);

	string strTime = getToken(_strDateTime, ",", 1);
	removeWhiteSpaces(strTime);

	tm tmTime;
	tmTime.tm_isdst = 0;
	tmTime.tm_wday = 0;
	tmTime.tm_yday = 0;

	tmTime.tm_hour = atol(getToken(strTime, ":", 0).c_str());
	tmTime.tm_min = atol(getToken(strTime, ":", 1).c_str());
	tmTime.tm_sec = atol(getToken(strTime, ":", 2).c_str());

	tmTime.tm_mday = atol(getToken(strDate, "/", 0).c_str());
	tmTime.tm_mon = atol(getToken(strDate, "/", 1).c_str()) - 1;
	tmTime.tm_year = atol(getToken(strDate, "/", 2).c_str()) - 1900;

	return mktime(&tmTime);
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Get commonly used directory paths
//////////////////////////////////////////////////////////////////////////
wstring CUtil::getWorkingDirectoryW()
{
	wchar_t strBuffer[MAX_PATH];

	// Get the current working directory
	_wgetcwd(strBuffer, MAX_PATH);

	return wstring(strBuffer);
}

string CUtil::getWorkingDirectoryA()
{
	char strBuffer[MAX_PATH];

	// Get the current working directory
	_getcwd(strBuffer, MAX_PATH);

	return string(strBuffer);
}

#ifdef _WIN32
wstring CUtil::getProgramPathW()
{
	wchar_t strFilePath[MAX_PATH];

	// Get the file path of the current executable
	if (::GetModuleFileNameW(nullptr, strFilePath, MAX_PATH) != 0)
	{
		return wstring(strFilePath);
	}

	return wstring(L"");
}

string CUtil::getProgramPathA()
{
	char strFilePath[MAX_PATH];

	// Get the file path of the current executable
	if (::GetModuleFileNameA(nullptr, strFilePath, MAX_PATH) != 0)
	{
		return string(strFilePath);
	}

	return string("");
}
#endif

#ifdef _WIN32
wstring CUtil::getProgramDirectoryW()
{
	wstring strProgramDirectory = getFileDirectory(getProgramPathW());

	return strProgramDirectory;
}

string CUtil::getProgramDirectoryA()
{
	string strProgramDirectory = getFileDirectory(getProgramPathA());

	return strProgramDirectory;
}
#endif

#ifdef _WIN32
wstring CUtil::getProgramNameW(bool bIncludeExtension /* = false*/)
{
	wstring strProgramName = getFileName(getProgramPathW(), bIncludeExtension);

	return strProgramName;
}

string CUtil::getProgramNameA(bool bIncludeExtension /* = false*/)
{
	string strProgramName = getFileName(getProgramPathA(), bIncludeExtension);

	return strProgramName;
}
#endif

#ifdef _WIN32
_tstring CUtil::getProgramFilesDirectory()
{
	// For backward compatibility, check specific registry location for the path
	_tstring strDirectory = getRegistryInfo(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion"), _T("ProgramFilesDir"));

	// Check if a path was found in the registry
	if (strDirectory.compare(_T("")) == 0)
	{
		TCHAR strPath[MAX_PATH];

		// Since there was no path found in the registry, use the API
		HRESULT hr = SHGetFolderPath(nullptr, CSIDL_PROGRAM_FILES | CSIDL_FLAG_CREATE, nullptr, 0, strPath);

		if (SUCCEEDED(hr))
		{
			strDirectory = strPath;
		}
	}

	return strDirectory;
}
#endif

#ifdef _WIN32
_tstring CUtil::getWindowsDirectory()
{
	_tstring strDirectory = _T("");

	TCHAR strPath[MAX_PATH];

	// Use the API to get windows installation directory path
	HRESULT hr = SHGetFolderPath(nullptr, CSIDL_WINDOWS | CSIDL_FLAG_CREATE, nullptr, 0, strPath);

	if (SUCCEEDED(hr))
	{
		strDirectory = strPath;
	}

	return strDirectory;
}
#endif

#ifdef _WIN32
_tstring CUtil::getSystemDirectory()
{
	_tstring strDirectory = _T("");

	TCHAR strPath[MAX_PATH];

	// Use the API to get windows system directory path
	HRESULT hr = SHGetFolderPath(nullptr, CSIDL_SYSTEM | CSIDL_FLAG_CREATE, nullptr, 0, strPath);

	if (SUCCEEDED(hr))
	{
		strDirectory = strPath;
	}

	return strDirectory;
}
#endif

#ifdef _WIN32
_tstring CUtil::getMyDocumentsDirectory()
{
	// For backward compatibility, check specific registry location for the path
	_tstring strDirectory = getRegistryInfo(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"), _T("Personal"));

	if (strDirectory.compare(_T("")) == 0)
	{
		TCHAR strPath[MAX_PATH];

		// Since there was no path found in the registry, use the API
		HRESULT hr = SHGetFolderPath(nullptr, CSIDL_PERSONAL | CSIDL_FLAG_CREATE, nullptr, 0, strPath);

		if (SUCCEEDED(hr))
		{
			strDirectory = strPath;
		}
	}

	return strDirectory;
}
#endif

#ifdef _WIN32
_tstring CUtil::getMyMusicDirectory()
{
	// For backward compatibility, check specific registry location for the path
	_tstring strDirectory = getRegistryInfo(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"), _T("My Music"));

	if (strDirectory.compare(_T("")) == 0)
	{
		TCHAR strPath[MAX_PATH];

		// Since there was no path found in the registry, use the API
		HRESULT hr = SHGetFolderPath(nullptr, CSIDL_MYMUSIC | CSIDL_FLAG_CREATE, nullptr, 0, strPath);

		if (SUCCEEDED(hr))
		{
			strDirectory = strPath;
		}
	}

	return strDirectory;
}
#endif

#ifdef _WIN32
_tstring CUtil::getMyPicturesDirectory()
{
	// For backward compatibility, check specific registry location for the path
	_tstring strDirectory = getRegistryInfo(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"), _T("My Pictures"));

	if (strDirectory.compare(_T("")) == 0)
	{
		TCHAR strPath[MAX_PATH];

		// Since there was no path found in the registry, use the API
		HRESULT hr = SHGetFolderPath(nullptr, CSIDL_MYPICTURES | CSIDL_FLAG_CREATE, nullptr, 0, strPath);

		if (SUCCEEDED(hr))
		{
			strDirectory = strPath;
		}
	}

	return strDirectory;
}
#endif

#ifdef _WIN32
_tstring CUtil::getMyVideosDirectory()
{
	// For backward compatibility, check specific registry location for the path
	_tstring strDirectory = getRegistryInfo(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"), _T("My Video"));

	if (strDirectory.compare(_T("")) == 0)
	{
		TCHAR strPath[MAX_PATH];

		// Since there was no path found in the registry, use the API
		HRESULT hr = SHGetFolderPath(nullptr, CSIDL_MYVIDEO | CSIDL_FLAG_CREATE, nullptr, 0, strPath);

		if (SUCCEEDED(hr))
		{
			strDirectory = strPath;
		}
	}

	return strDirectory;
}
#endif

#ifdef _WIN32
_tstring CUtil::getAppDataDirectory()
{
	// For backward compatibility, check specific registry location for the path
	_tstring strDirectory = getRegistryInfo(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"), _T("AppData"));

	if (strDirectory.compare(_T("")) == 0)
	{
		TCHAR strPath[MAX_PATH];

		// Since there was no path found in the registry, use the API
		HRESULT hr = SHGetFolderPath(nullptr, CSIDL_APPDATA | CSIDL_FLAG_CREATE, nullptr, 0, strPath);

		if (SUCCEEDED(hr))
		{
			strDirectory = strPath;
		}
	}

	return strDirectory;
}
#endif

#ifdef _WIN32
_tstring CUtil::getLocalAppDataDirectory()
{
	// For backward compatibility, check specific registry location for the path
	_tstring strDirectory = getRegistryInfo(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"), _T("Local AppData"));

	if (strDirectory.compare(_T("")) == 0)
	{
		TCHAR strPath[MAX_PATH];

		// Since there was no path found in the registry, use the API
		HRESULT hr = SHGetFolderPath(nullptr, CSIDL_LOCAL_APPDATA | CSIDL_FLAG_CREATE, nullptr, 0, strPath);

		if (SUCCEEDED(hr))
		{
			strDirectory = strPath;
		}
	}

	return strDirectory;
}
#endif

#ifdef _WIN32
_tstring CUtil::getDesktopDirectory()
{
	// For backward compatibility, check specific registry location for the path
	_tstring strDirectory = getRegistryInfo(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"), _T("Desktop"));

	if (strDirectory.compare(_T("")) == 0)
	{
		TCHAR strPath[MAX_PATH];

		// Since there was no path found in the registry, use the API
		HRESULT hr = SHGetFolderPath(nullptr, CSIDL_DESKTOPDIRECTORY | CSIDL_FLAG_CREATE, nullptr, 0, strPath);

		if (SUCCEEDED(hr))
		{
			strDirectory = strPath;
		}
	}

	return strDirectory;
}
#endif

#ifdef _WIN32
_tstring CUtil::getStartupDirectory()
{
	// For backward compatibility, check specific registry location for the path
	_tstring strDirectory = getRegistryInfo(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"), _T("Startup"));

	if (strDirectory.compare(_T("")) == 0)
	{
		TCHAR strPath[MAX_PATH];

		// Since there was no path found in the registry, use the API
		HRESULT hr = SHGetFolderPath(nullptr, CSIDL_STARTUP | CSIDL_FLAG_CREATE, nullptr, 0, strPath);

		if (SUCCEEDED(hr))
		{
			strDirectory = strPath;
		}
	}

	return strDirectory;
}
#endif
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Copy or cut a file into another directory, creating the directory path if necessary
//////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
bool CUtil::copyFileToDirectory(const _tstring& _strSourceFilePath, const _tstring& _strDestinationDirectory, bool _bDeleteSource /* = false*/)
{
	if (_strSourceFilePath.compare(_T("")) == 0)
	{
		return false;
	}

	if (_strDestinationDirectory.compare(_T("")) == 0)
	{
		return false;
	}

	// Create the entire destination directory path, if not present
	makeDirectory(_strDestinationDirectory);

	_tstring strSourceFileName = getFileName(_strSourceFilePath, true);

	// Build the destination file path
	_tstring strDestinationFilePath = _strDestinationDirectory;
	strDestinationFilePath = CUtil::addDirectoryEnding(strDestinationFilePath);
	strDestinationFilePath += strSourceFileName;

	// Copy from source to destination
	if (!CopyFile(_strSourceFilePath.c_str(), strDestinationFilePath.c_str(), FALSE))
	{
		return false;
	}
	else
	{
		if (_bDeleteSource)
		{
			_tunlink(_strSourceFilePath.c_str());
		}

		return true;
	}
}
#endif
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Directory/File manipulation functions
//////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
void CUtil::getFileList(const _tstring& _strTargetDirectoryPath,
	const _tstring& _strWildcard,
	bool _bLookInSubdirectories,
	vector<_tstring>& _vstrFileList,
	int iPause /* = 0*/)
{
	// Check whether target directory string is empty
	if (_strTargetDirectoryPath.compare(_T("")) == 0)
	{
		return;
	}

	// Remove "\\" if present at the end of the target directory
	// Then make a copy of it and use as the current search directory
	_tstring strCurrentDirectory = removeDirectoryEnding(_strTargetDirectoryPath);

	// This data structure stores information about the file/folder that is found by any of these Win32 API functions:
	// FindFirstFile, FindFirstFileEx, or FindNextFile function
	WIN32_FIND_DATA fdDesktop = { 0 };

	// Format and copy the current directory
	// Note the addition of the wildcard *.*, which represents all files
	// 
	// Below is a list of wildcards that you can use
	// * (asterisk)         - represents zero or more characters at the current character position
	// ? (question mark)    - represents a single character
	//
	// Modify this function so that the function can take in a search pattern with wildcards and use it in the line below to find for e.g. only *.mpg files
	//
	// "\\?\" prefix to the file path means that the file system supports large paths/filenames
	_tstring strDesktopPath = _T("");
	strDesktopPath += _T("\\\\?\\");
	strDesktopPath += strCurrentDirectory;
	strDesktopPath = addDirectoryEnding(strDesktopPath);

	strDesktopPath += _T("*.*");

	// Finds the first file and populates the WIN32_FIND_DATA data structure with its information
	// The return value is a search handle used in subsequent calls to FindNextFile or FindClose functions
	HANDLE hDesktop = ::FindFirstFile(strDesktopPath.c_str(), &fdDesktop);

	// If an invalid handle is returned by FindFirstFile function, then the directory is empty, so nothing to do but quit
	if (hDesktop == INVALID_HANDLE_VALUE)
	{
		return;
	}

	// Do this on the first file found and repeat for every next file found until all the required files that match the search pattern are found
	do
	{
		// Reconstruct the path
		_tstring strPath = _T("");
		strPath += strCurrentDirectory;
		strPath = addDirectoryEnding(strPath);
		strPath += fdDesktop.cFileName;

		// Check if a directory was found
		if (fdDesktop.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			// Get the name of the directory
			_tstring strCurrentDirectoryName = getDirectoryName(strPath);

			// If its a current (.) or previous (..) directory indicator, just skip it
			if ((strCurrentDirectoryName.compare(_T(".")) == 0) || (strCurrentDirectoryName.compare(_T("..")) == 0))
			{
				continue;
			}
			// Other wise this is a sub-directory
			else
			{
				// Check whether function was called to include sub-directories in the search
				if (_bLookInSubdirectories)
				{
					// If sub-directories are to be searched as well, recursively call the function again, with the target directory as the sub-directory
					getFileList(strPath, _strWildcard, _bLookInSubdirectories, _vstrFileList);
				}
			}
		}
		// A file was found
		else
			// if(fdDesktop.dwFileAttributes & FILE_ATTRIBUTE_NORMAL)
		{
			_tstring strFileName = getFileName(strPath, true);
			if (compareWildcard(strFileName.c_str(), _strWildcard.c_str()))
			{
				// Add the string to the vector
				_vstrFileList.push_back(_tstring(strPath));
			}
		}

		if (iPause > 0)
		{
			Sleep(iPause);
		}
	}
	// Search for the next file that matches the search pattern
	while (::FindNextFile(hDesktop, &fdDesktop) == TRUE);

	// Close the search handle
	::FindClose(hDesktop);
}
#endif

#ifdef _WIN32
_tstring CUtil::renameFiles(const _tstring& _strTargetDirectoryPath,
	const _tstring& _strWildcard,
	bool _bLookInSubdirectories,
	const _tstring& _strFind,
	const _tstring& _strReplace,
	bool _bAppendNumbers)
{
	// Check whether target directory string is empty
	if (_strTargetDirectoryPath.compare(_T("")) == 0)
	{
		return _T("");
	}

	// Check whether substring to find is empty
	if (_strFind.compare(_T("")) == 0)
	{
		return _T("");
	}

	// Vector of file paths
	vector<_tstring> vstrFileList;
	vstrFileList.clear();

	// Get the list of files
	getFileList(_strTargetDirectoryPath, _strWildcard, _bLookInSubdirectories, vstrFileList);

	_tstring strLog = _T("");
	strLog += _T("Total number of files: ");
	strLog += toString((int)vstrFileList.size());
	strLog += _T("\r\n\r\n");

	for (size_t i = 0; i < vstrFileList.size(); i++)
	{
		_tstring strOldFilePath = vstrFileList[i];
		_tstring strFileName = getFileName(strOldFilePath, true);

		if (_strFind == _T("*."))
		{
			strFileName = _strReplace + getFileExtension(strFileName, true);
		}
		else
		{
			findReplace(strFileName, _strFind, _strReplace);
		}

		if (_bAppendNumbers)
		{
			_tstring strNumberedFileName = getFileName(strFileName);

			trim(strNumberedFileName, _T("0123456789_- \t\r\n"), false, true);

			strNumberedFileName += _T("_");
			strNumberedFileName += toString((int)i, 4);
			strNumberedFileName += getFileExtension(strFileName, true);

			strFileName = strNumberedFileName;
		}

		_tstring strNewFilePath = getFileDirectory(strOldFilePath);
		strNewFilePath = addDirectoryEnding(strNewFilePath);
		strNewFilePath += strFileName;

		_trename(strOldFilePath.c_str(), strNewFilePath.c_str());

		strLog += strOldFilePath;
		strLog += _T("\r\n");
		strLog += strNewFilePath;
		strLog += _T("\r\n\r\n");
	}

	return strLog;
}
#endif

void CUtil::makeDirectory(const wstring& _strDirectoryPath)
{
	makeDirectory(getStringA(_strDirectoryPath));
}

void CUtil::makeDirectory(const string& _strDirectoryPath)
{
	if (_strDirectoryPath.compare("") == 0)
	{
		return;
	}

	string strDirectoryPathCopy = removeDirectoryEnding(_strDirectoryPath);

	int iIndex = 0;
	int iCount = (int)getTokenCount(strDirectoryPathCopy, DIRECTORY_SEPARATOR_A);

	string strDirectory2Make = getToken(strDirectoryPathCopy, DIRECTORY_SEPARATOR_A, 0);

	for (iIndex = 1; iIndex < iCount; iIndex++)
	{
		strDirectory2Make += DIRECTORY_SEPARATOR_A;
		strDirectory2Make += getToken(strDirectoryPathCopy, DIRECTORY_SEPARATOR_A, iIndex);

		// check if directory does not exist
		if (access(strDirectory2Make.c_str(), O_RDWR))
		{
			// try to create the directory
			// 0 = file created, -1 = error, EEXIST, ENOENT
			int iError = mkdir(strDirectory2Make.c_str(), 0777);
		}
	}
}

#ifdef _WIN32
void CUtil::deleteDirectory(const _tstring& _strTargetDirectoryPath)
{
	// Check whether target directory string is empty
	if (_strTargetDirectoryPath.compare(_T("")) == 0)
	{
		return;
	}

	// Remove "\\" if present at the end of the target directory
	// Then make a copy of it and use as the current search directory
	_tstring strCurrentDirectory = removeDirectoryEnding(_strTargetDirectoryPath);

	// This data structure stores information about the file/folder that is found by any of these Win32 API functions:
	// FindFirstFile, FindFirstFileEx, or FindNextFile function
	WIN32_FIND_DATA fdDesktop = { 0 };

	// Format and copy the current directory
	// Note the addition of the wildcard *.*, which represents all files
	// 
	// Below is a list of wildcards that you can use
	// * (asterisk)         - represents zero or more characters at the current character position
	// ? (question mark)    - represents a single character
	//
	// Modify this function so that the function can take in a search pattern with wildcards and use it in the line below to find for e.g. only *.mpg files
	//
	// "\\?\" prefix to the file path means that the file system supports large paths/filenames
	_tstring strDesktopPath = _T("");
	strDesktopPath += _T("\\\\?\\");
	strDesktopPath += strCurrentDirectory;
	strDesktopPath = addDirectoryEnding(strDesktopPath);
	strDesktopPath += _T("*.*");

	_tstring strDirectory2Delete = _T("");
	strDirectory2Delete += _T("\\\\?\\");
	strDirectory2Delete += strCurrentDirectory;

	// Finds the first file and populates the WIN32_FIND_DATA data structure with its information
	// The return value is a search handle used in subsequent calls to FindNextFile or FindClose functions
	HANDLE hDesktop = ::FindFirstFile(strDesktopPath.c_str(), &fdDesktop);

	// If an invalid handle is returned by FindFirstFile function, then the directory is empty, so delete directory and quit
	if (hDesktop == INVALID_HANDLE_VALUE)
	{
		// Remove the directory
		RemoveDirectory(strDirectory2Delete.c_str());

		return;
	}

	// Do this on the first file found and repeat for every next file found until all the required files that match the search pattern are found
	do
	{
		// Reconstruct the path
		_tstring strPath = _T("");
		strPath += strCurrentDirectory;
		strPath = addDirectoryEnding(strPath);
		strPath += fdDesktop.cFileName;

		// Check if a directory was found
		if (fdDesktop.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			// Get the name of the directory
			_tstring strCurrentDirectoryName = getDirectoryName(strPath);

			// If its a current (.) or previous (..) directory indicator, just skip it
			if ((strCurrentDirectoryName.compare(_T(".")) == 0) || (strCurrentDirectoryName.compare(_T("..")) == 0))
			{
				continue;
			}
			// Other wise this is a sub-directory
			else
			{
				// Recursively call the function again, with the target directory as the sub-directory
				deleteDirectory(strPath);
			}
		}
		// A file was found
		else
			// if(fdDesktop.dwFileAttributes & FILE_ATTRIBUTE_NORMAL)
		{
			// Delete the file
			_tunlink(strPath.c_str());
		}
	}
	// Search for the next file that matches the search pattern
	while (::FindNextFile(hDesktop, &fdDesktop) == TRUE);

	// Close the search handle
	::FindClose(hDesktop);

	// Remove the directory
	RemoveDirectory(strDirectory2Delete.c_str());
}
#endif

#ifdef _WIN32
void CUtil::deleteFiles(const _tstring& _strTargetDirectoryPath, const _tstring& _strWildcard, bool _bLookInSubdirectories)
{
	vector<_tstring> vstrFileList;

	getFileList(_strTargetDirectoryPath, _strWildcard, _bLookInSubdirectories, vstrFileList);

	for (size_t i = 0; i < vstrFileList.size(); i++)
	{
		_tunlink(vstrFileList[i].c_str());
	}
}
#endif

#ifdef _WIN32
bool CUtil::isDirectory(const wstring& _strDirectoryPath)
{
	if (_strDirectoryPath.compare(L"") == 0)
	{
		return false;
	}

	CreateDirectoryW(_strDirectoryPath.c_str(), nullptr);

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		return true;
	}

	return false;
}

bool CUtil::isDirectory(const string& _strDirectoryPath)
{
	if (_strDirectoryPath.compare("") == 0)
	{
		return false;
	}

	CreateDirectoryA(_strDirectoryPath.c_str(), nullptr);

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		return true;
	}

	return false;
}
#endif
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Directory/File path string manipulation functions
//////////////////////////////////////////////////////////////////////////
wstring CUtil::homogenizePath(wstring _strPath)
{
	if (_strPath.empty())
	{
		return L"";
	}

#ifdef _WIN32
	std::replace(_strPath.begin(), _strPath.end(), L'/', DIRECTORY_SEPARATOR_W_C);
#else
	std::replace(_strPath.begin(), _strPath.end(), L'\\', DIRECTORY_SEPARATOR_W_C);
#endif

	if (isDirectory(_strPath))
	{
		if ((_strPath[_strPath.length() - 1] != DIRECTORY_SEPARATOR_W_C))
		{
			_strPath += DIRECTORY_SEPARATOR_W_C;
		}
	}
	else
	{
		if ((_strPath[_strPath.length() - 1] == DIRECTORY_SEPARATOR_W_C))
		{
			_strPath[_strPath.length() - 1] = L'\0';
		}
	}

	return _strPath;
}

string CUtil::homogenizePath(string _strPath)
{
	if (_strPath.empty())
	{
		return "";
	}

#ifdef _WIN32
	std::replace(_strPath.begin(), _strPath.end(), '/', DIRECTORY_SEPARATOR_A_C);
#else
	std::replace(_strPath.begin(), _strPath.end(), '\\', DIRECTORY_SEPARATOR_A_C);
#endif

	if (isDirectory(_strPath))
	{
		if ((_strPath[_strPath.length() - 1] != DIRECTORY_SEPARATOR_A_C))
		{
			_strPath += DIRECTORY_SEPARATOR_A_C;
		}
	}
	else
	{
		if ((_strPath[_strPath.length() - 1] == DIRECTORY_SEPARATOR_A_C))
		{
			_strPath[_strPath.length() - 1] = '\0';
		}
	}

	return _strPath;
}

wstring CUtil::addDirectoryEnding(const wstring& _strDirectoryPath)
{
	if (_strDirectoryPath.compare(L"") == 0)
	{
		return wstring(L"");
	}

	wstring strDirectory = _strDirectoryPath;

	if (strDirectory[strDirectory.length() - 1] != DIRECTORY_SEPARATOR_W_C)
	{
		strDirectory += DIRECTORY_SEPARATOR_W;
	}

	return strDirectory;
}

string CUtil::addDirectoryEnding(const string& _strDirectoryPath)
{
	if (_strDirectoryPath.compare("") == 0)
	{
		return string("");
	}

	string strDirectory = _strDirectoryPath;

	if (strDirectory[strDirectory.length() - 1] != DIRECTORY_SEPARATOR_A_C)
	{
		strDirectory += DIRECTORY_SEPARATOR_A;
	}

	return strDirectory;
}

wstring CUtil::removeDirectoryEnding(const wstring& _strDirectoryPath)
{
	if (_strDirectoryPath.compare(L"") == 0)
	{
		return wstring(L"");
	}

	wstring strDirectory = _strDirectoryPath;

	if (strDirectory[strDirectory.length() - 1] == DIRECTORY_SEPARATOR_W_C)
	{
		strDirectory.erase(strDirectory.length() - 1);
	}

	return strDirectory;
}

string CUtil::removeDirectoryEnding(const string& _strDirectoryPath)
{
	if (_strDirectoryPath.compare("") == 0)
	{
		return string("");
	}

	string strDirectory = _strDirectoryPath;

	if (strDirectory[strDirectory.length() - 1] == DIRECTORY_SEPARATOR_A_C)
	{
		strDirectory.erase(strDirectory.length() - 1);
	}

	return strDirectory;
}

wstring CUtil::getRootDirectory(const wstring& _strDirectoryPath)
{
	if (_strDirectoryPath.compare(L"") == 0)
	{
		return wstring(L"");
	}

	wstring strRootDirectory = removeDirectoryEnding(_strDirectoryPath);

	size_t nIndex = strRootDirectory.find_last_of(DIRECTORY_SEPARATOR_W);

	if (nIndex == wstring::npos)
	{
		return wstring(L"");
	}

	strRootDirectory.erase(nIndex);

	return strRootDirectory;
}

string CUtil::getRootDirectory(const string& _strDirectoryPath)
{
	if (_strDirectoryPath.compare("") == 0)
	{
		return string("");
	}

	string strRootDirectory = removeDirectoryEnding(_strDirectoryPath);

	size_t nIndex = strRootDirectory.find_last_of(DIRECTORY_SEPARATOR_A);

	if (nIndex == string::npos)
	{
		return string("");
	}

	strRootDirectory.erase(nIndex);

	return strRootDirectory;
}

wstring CUtil::getDirectoryName(const wstring& _strDirectoryPath)
{
	if (_strDirectoryPath.compare(L"") == 0)
	{
		return wstring(L"");
	}

	wstring strDirectoryName = removeDirectoryEnding(_strDirectoryPath);

	size_t nIndex = strDirectoryName.find_last_of(DIRECTORY_SEPARATOR_W);

	if (nIndex == wstring::npos)
	{
		return wstring(L"");
	}

	strDirectoryName.erase(0, nIndex + 1);

	return strDirectoryName;
}

string CUtil::getDirectoryName(const string& _strDirectoryPath)
{
	if (_strDirectoryPath.compare("") == 0)
	{
		return string("");
	}

	string strDirectoryName = removeDirectoryEnding(_strDirectoryPath);

	size_t nIndex = strDirectoryName.find_last_of(DIRECTORY_SEPARATOR_A);

	if (nIndex == string::npos)
	{
		return string("");
	}

	strDirectoryName.erase(0, nIndex + 1);

	return strDirectoryName;
}

wstring CUtil::getFileDirectory(const wstring& _strFilePath)
{
	if (_strFilePath.compare(L"") == 0)
	{
		return wstring(L"");
	}

	wstring strDirectory = removeDirectoryEnding(_strFilePath);

	size_t nIndex = strDirectory.find_last_of(DIRECTORY_SEPARATOR_W);

	if (nIndex == wstring::npos)
	{
		return wstring(L"");
	}

	strDirectory.erase(nIndex);

	return strDirectory;
}

string CUtil::getFileDirectory(const string& _strFilePath)
{
	if (_strFilePath.compare("") == 0)
	{
		return string("");
	}

	string strDirectory = removeDirectoryEnding(_strFilePath);

	size_t nIndex = strDirectory.find_last_of(DIRECTORY_SEPARATOR_A);

	if (nIndex == string::npos)
	{
		return string("");
	}

	strDirectory.erase(nIndex);

	return strDirectory;
}

wstring CUtil::getFileName(const wstring& _strFilePath, bool _bIncludeExtension /* = false*/)
{
	if (_strFilePath.compare(L"") == 0)
	{
		return wstring(L"");
	}

	wstring strFileName = _strFilePath;

	size_t nIndex = strFileName.find_last_of(DIRECTORY_SEPARATOR_W);

	if (nIndex != wstring::npos)
	{
		strFileName.erase(0, nIndex + 1);
	}

	if (!_bIncludeExtension)
	{
		nIndex = strFileName.find_last_of(L".");

		if (nIndex == wstring::npos)
		{
			return strFileName;
		}

		strFileName.erase(nIndex);
	}

	return strFileName;
}

string CUtil::getFileName(const string& _strFilePath, bool _bIncludeExtension /* = false*/)
{
	if (_strFilePath.compare("") == 0)
	{
		return string("");
	}

	string strFileName = _strFilePath;

	size_t nIndex = strFileName.find_last_of(DIRECTORY_SEPARATOR_A);

	if (nIndex != string::npos)
	{
		strFileName.erase(0, nIndex + 1);
	}

	if (!_bIncludeExtension)
	{
		nIndex = strFileName.find_last_of(".");

		if (nIndex == string::npos)
		{
			return strFileName;
		}

		strFileName.erase(nIndex);
	}

	return strFileName;
}

wstring CUtil::getFileExtension(const wstring& _strFilePath, bool _bIncludeDot /* = false*/)
{
	if (_strFilePath.compare(L"") == 0)
	{
		return wstring(L"");
	}

	wstring strFileExtension = _strFilePath;

	size_t nIndex = strFileExtension.find_last_of(L".");

	if (nIndex == wstring::npos)
	{
		return wstring(L"");
	}

	if (!_bIncludeDot)
	{
		strFileExtension.erase(0, nIndex + 1);
	}
	else
	{
		strFileExtension.erase(0, nIndex);
	}

	if (strFileExtension.compare(_strFilePath) == 0)
	{
		strFileExtension = L"";
	}

	return strFileExtension;
}

string CUtil::getFileExtension(const string& _strFilePath, bool _bIncludeDot /* = false*/)
{
	if (_strFilePath.compare("") == 0)
	{
		return string("");
	}

	string strFileExtension = _strFilePath;

	size_t nIndex = strFileExtension.find_last_of(".");

	if (nIndex == string::npos)
	{
		return string("");
	}

	if (!_bIncludeDot)
	{
		strFileExtension.erase(0, nIndex + 1);
	}
	else
	{
		strFileExtension.erase(0, nIndex);
	}

	if (strFileExtension.compare(_strFilePath) == 0)
	{
		strFileExtension = "";
	}

	return strFileExtension;
}

wstring CUtil::getFileNamePrefix(const wstring& _strFilePath, const wstring& _strDelimiter)
{
	wstring strFileName = getFileName(_strFilePath);

	wstring strFileNamePrefix = strFileName;

	size_t nIndex = strFileNamePrefix.find_last_of(_strDelimiter);

	if (nIndex != wstring::npos)
	{
		strFileNamePrefix.erase(nIndex);
	}

	if (nIndex <= 0 || nIndex >= strFileName.size() - 1)
	{
		strFileNamePrefix = strFileName;
	}

	return strFileNamePrefix;
}

string CUtil::getFileNamePrefix(const string& _strFilePath, const string& _strDelimiter)
{
	string strFileName = getFileName(_strFilePath);

	string strFileNamePrefix = strFileName;

	size_t nIndex = strFileNamePrefix.find_last_of(_strDelimiter);

	if (nIndex != string::npos)
	{
		strFileNamePrefix.erase(nIndex);
	}

	if (nIndex <= 0 || nIndex >= strFileName.size() - 1)
	{
		strFileNamePrefix = strFileName;
	}

	return strFileNamePrefix;
}

wstring CUtil::getFileNameSuffix(const wstring& _strFilePath, const wstring& _strDelimiter)
{
	wstring strFileName = getFileName(_strFilePath);

	wstring strFileNameSuffix = strFileName;

	size_t nIndex = strFileNameSuffix.find_last_of(_strDelimiter);

	if (nIndex != wstring::npos)
	{
		strFileNameSuffix.erase(0, nIndex + 1);
	}

	if (nIndex <= 0 || nIndex >= strFileName.size() - 1)
	{
		strFileNameSuffix = strFileName;
	}

	return strFileNameSuffix;
}

string CUtil::getFileNameSuffix(const string& _strFilePath, const string& _strDelimiter)
{
	string strFileName = getFileName(_strFilePath);

	string strFileNameSuffix = strFileName;

	size_t nIndex = strFileNameSuffix.find_last_of(_strDelimiter);

	if (nIndex != string::npos)
	{
		strFileNameSuffix.erase(0, nIndex + 1);
	}

	if (nIndex <= 0 || nIndex >= strFileName.size() - 1)
	{
		strFileNameSuffix = strFileName;
	}

	return strFileNameSuffix;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// String type converters, ANSI <-> UNICODE
//////////////////////////////////////////////////////////////////////////
char CUtil::wide2Narrow(wchar_t _wch)
{
	// Simple type cast works because UNICODE incorporates ASCII into itself
	char ch = (char)_wch;
	return ch;
}

wchar_t CUtil::narrow2Wide(char _ch)
{
	// Simple type cast works because UNICODE incorporates ASCII into itself
	wchar_t wch = (wchar_t)_ch;
	return wch;
}

wstring CUtil::getStringW(const string& _strA)
{
	wstring strW = L"";
	strW.resize(_strA.size(), L'\0');

	std::transform(_strA.begin(), _strA.end(), strW.begin(), narrow2Wide);

	return strW;
}

wstring CUtil::getString(const string& _strA)
{
	return getStringW(_strA);
}

string CUtil::getStringA(const wstring& _strW)
{
	string strA = "";
	strA.resize(_strW.size(), '\0');

	std::transform(_strW.begin(), _strW.end(), strA.begin(), wide2Narrow);

	return strA;
}

string CUtil::getString(const wstring& _strW)
{
	return getStringA(_strW);
}

wstring CUtil::utf8_to_utf16(const string& _strUTF8)
{
	// UTF8 to UTF16
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;
	wstring strUTF16 = convert.from_bytes(_strUTF8);

	return strUTF16;
}

string CUtil::utf16_to_utf8(const wstring& _strUTF16)
{
	// UTF16 to UTF8
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;
	string strUTF8 = convert.to_bytes(_strUTF16);

	return strUTF8;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Registry
//////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
_tstring CUtil::getRegistryInfo(HKEY _hkey, const _tstring& _strRegistryPath, const _tstring& _strValueName)
{
	CRegKey reg;
	_tstring strKey = _T("");

	if (reg.Open(_hkey, _strRegistryPath.c_str(), KEY_READ) == ERROR_SUCCESS)
	{
		TCHAR strKeyBuffer[MAX_PATH * 4];
		ULONG ulKeyBufferSize = MAX_PATH * 4;

		if (reg.QueryStringValue(_strValueName.c_str(), strKeyBuffer, &ulKeyBufferSize) == ERROR_SUCCESS)
		{
			strKey = strKeyBuffer;
		}

		reg.Close();
	}

	return strKey;
}

DWORD CUtil::getRegistryValue(HKEY _hkey, const _tstring& _strRegistryPath, const _tstring& _strValueName)
{
	CRegKey reg;
	DWORD dwKey = 0;

	if (reg.Open(_hkey, _strRegistryPath.c_str(), KEY_READ) == ERROR_SUCCESS)
	{
		if (reg.QueryDWORDValue(_strValueName.c_str(), dwKey) == ERROR_SUCCESS)
		{
			// do nothing!
		}

		reg.Close();
	}

	return dwKey;
}

bool CUtil::setRegistryInfo(HKEY _hkey, const _tstring& _strRegistryPath, const _tstring& _strValueName, const _tstring& _strValue)
{
	CRegKey reg;
	bool bSuccess = false;

	if (reg.Open(_hkey, _strRegistryPath.c_str(), KEY_WRITE) == ERROR_SUCCESS)
	{
		if (reg.SetStringValue(_strValueName.c_str(), _strValue.c_str(), REG_SZ) == ERROR_SUCCESS)
		{
			bSuccess = true;

			reg.Flush();
		}

		reg.Close();
	}

	return bSuccess;
}

bool CUtil::setRegistryValue(HKEY _hkey, const _tstring& _strRegistryPath, const _tstring& _strValueName, DWORD _dwValue)
{
	CRegKey reg;
	bool bSuccess = false;

	if (reg.Open(_hkey, _strRegistryPath.c_str(), KEY_WRITE) == ERROR_SUCCESS)
	{
		if (reg.SetDWORDValue(_strValueName.c_str(), _dwValue) == ERROR_SUCCESS)
		{
			bSuccess = true;

			reg.Flush();
		}

		reg.Close();
	}

	return bSuccess;
}
#endif
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Internet
//////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
LONG CUtil::getRegKey(HKEY _hkey, LPCTSTR _subkey, LPTSTR _regdata)
{
	HKEY hkey;
	int iResult = 0;

	iResult = RegOpenKeyEx(_hkey, _subkey, 0, KEY_QUERY_VALUE, &hkey);

	if (iResult == ERROR_SUCCESS)
	{
		TCHAR strDataBuffer[MAX_PATH * 4];
		LONG lDataSize = MAX_PATH * 4;

		RegQueryValue(hkey, nullptr, strDataBuffer, &lDataSize);

		lstrcpy(_regdata, strDataBuffer);

		RegCloseKey(hkey);
	}

	return iResult;
}
#endif

#ifdef _WIN32
HINSTANCE CUtil::openURL(LPCTSTR _strURL)
{
	TCHAR strkey[MAX_PATH * 2];

	// First try ShellExecute()
	HINSTANCE hinst = ShellExecute(nullptr, _T("open"), _strURL, nullptr, nullptr, SW_SHOW);

	// If it failed, get the .htm registry key and lookup the program
	if ((INT_PTR)hinst <= HINSTANCE_ERROR)
	{
		if (getRegKey(HKEY_CLASSES_ROOT, _T(".htm"), strkey) == ERROR_SUCCESS)
		{
			lstrcat(strkey, _T("\\shell\\open\\command"));

			if (getRegKey(HKEY_CLASSES_ROOT, strkey, strkey) == ERROR_SUCCESS)
			{
				TCHAR *pos;

				pos = _tcsstr(strkey, _T("\"%1\""));

				if (pos == nullptr)     // No quotes found
				{
					pos = _tcsstr(strkey, _T("%1"));        // Check for %1, without quotes 

					if (pos == nullptr)                     // No parameter at all...
					{
						pos = strkey + lstrlen(strkey) - 1;
					}
					else
					{
						*pos = _T('\0');    // Remove the parameter
					}
				}
				else
				{
					*pos = _T('\0');        // Remove the parameter
				}

				lstrcat(pos, _T(" "));
				lstrcat(pos, _strURL);

				USES_CONVERSION;
				hinst = (HINSTANCE)(INT_PTR)WinExec(T2A(strkey), SW_SHOW);
			}
		}
	}

	return hinst;
}
#endif

#ifdef _WIN32
_tstring CUtil::getMyPublicIP(const _tstring& _strURL /* = _T("http://bot.whatismyipaddress.com")*/)
{
	static const unsigned int BUFFER_SIZE = 1024;

	// get the public ip from the url
	HINTERNET hInternet = nullptr;
	HINTERNET hFile = nullptr;
	DWORD dwSize = 0;
	char strBuffer[BUFFER_SIZE];

	hInternet = InternetOpen(nullptr, INTERNET_OPEN_TYPE_PRECONFIG, nullptr, nullptr, 0);
	hFile = InternetOpenUrl(hInternet, _strURL.c_str(), nullptr, 0, INTERNET_FLAG_RELOAD, 0);
	InternetReadFile(hFile, &strBuffer, BUFFER_SIZE, &dwSize);

	if (dwSize < BUFFER_SIZE)
	{
		strBuffer[dwSize] = '\0';
	}
	else
	{
		strBuffer[BUFFER_SIZE - 1] = '\0';
	}

	InternetCloseHandle(hFile);
	InternetCloseHandle(hInternet);

	_tstring strIP = _T("");

#ifdef UNICODE
	strIP = getStringW(strBuffer);
#else
	strIP = strBuffer;
#endif

	if (compareNoCase(strIP, _T("")) == 0)
	{
		strIP = _T("0.0.0.0");
	}

	return strIP;
}
#endif
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Execution
//////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
bool CUtil::execute(const _tstring& _strCommandLine, bool _bShow /* = true*/, bool _bWait /* = false*/)
{
	STARTUPINFO SI;
	memset(&SI, 0, sizeof(SI));
	SI.cb = sizeof(STARTUPINFO);

	PROCESS_INFORMATION PI;
	memset(&PI, 0, sizeof(PI));

	DWORD dwCreationFlags = 0;

	if (_bShow)
	{
		dwCreationFlags = CREATE_NEW_CONSOLE;
	}
	else
	{
		dwCreationFlags = CREATE_NO_WINDOW;
	}

	if (!::CreateProcess(nullptr, (LPTSTR)_strCommandLine.c_str(), nullptr, nullptr, TRUE, dwCreationFlags, nullptr, nullptr, &SI, &PI))
	{
		return false;
	}
	else
	{
		if (_bWait)
		{
			::WaitForSingleObject(PI.hProcess, INFINITE);
		}
	}

	// Release handles
	CloseHandle(PI.hProcess);
	CloseHandle(PI.hThread);

	return true;
}
#endif
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// System
//////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
int CUtil::getProcessorCount()
{
	SYSTEM_INFO SystemInfo;

	::GetSystemInfo(&SystemInfo);

	return SystemInfo.dwNumberOfProcessors;
}
#endif

#ifdef _WIN32
bool CUtil::is_x64()
{
	BOOL bIs_x64 = FALSE;

	typedef BOOL(APIENTRY *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

	LPFN_ISWOW64PROCESS fnIsWow64Process;

	HMODULE module = GetModuleHandle(_T("kernel32"));

	const char strfnName[] = "IsWow64Process";
	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(module, strfnName);

	if (fnIsWow64Process != nullptr)
	{
		if (!fnIsWow64Process(GetCurrentProcess(), &bIs_x64))
		{
			throw std::exception("error");
		}
	}

	return (bIs_x64 != FALSE);
}
#endif

void CUtil::pause(unsigned int _uiMilliSeconds)
{
	if (_uiMilliSeconds == 0)
	{
		return;
	}

	std::chrono::milliseconds timespan(_uiMilliSeconds);
	std::this_thread::sleep_for(timespan);
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Random generators
//////////////////////////////////////////////////////////////////////////
wstring CUtil::generateRandomStringW(int _iMaxLength, bool _bIncludeAlpha /* = true*/, bool _bIncludeNumbers /* = true*/)
{
	return getStringW(generateRandomStringA(_iMaxLength, _bIncludeAlpha, _bIncludeNumbers));
}

string CUtil::generateRandomStringA(int _iMaxLength, bool _bIncludeAlpha /* = true*/, bool _bIncludeNumbers /* = true*/)
{
	string strRamdom = "";

	int iLength = _iMaxLength + 1;

	int iRandFactor = (RAND_MAX / _iMaxLength) - 1;

	while (iLength > _iMaxLength)
	{
		iLength = (rand() / iRandFactor) + 1;
	}

	for (int i = 0; i < iLength; i++)
	{
		while (true)
		{
			unsigned char cRandom = (unsigned char)abs((rand() / 128));

			if (_bIncludeAlpha && _bIncludeNumbers)
			{
				if (isalnum(cRandom))
				{
					strRamdom += cRandom;

					break;
				}
			}
			else if (_bIncludeAlpha)
			{
				if (isalpha(cRandom))
				{
					strRamdom += cRandom;

					break;
				}
			}
			else if (_bIncludeNumbers)
			{
				if (isdigit(cRandom))
				{
					strRamdom += cRandom;

					break;
				}
			}
			else
			{
				strRamdom += cRandom;

				break;
			}
		}
	}

	return strRamdom;
}

// Generate random number between A & B
float CUtil::getRandomNumber(float _fA, float _fB, float _fGrain /* = 10.0f*/)
{
	if (_fA == _fB)
	{
		return 0.0;
	}

	int iRandomNumber = rand() * rand();

	float fRange = abs(_fA - _fB);
	int iDivisor = (int)((fRange * _fGrain) + 1.0f);

	if (_fA < _fB)
	{
		return (((float)(iRandomNumber % iDivisor) / _fGrain) + (abs(_fA)));
	}
	else
	{
		return (((float)(iRandomNumber % iDivisor) / _fGrain) + (abs(_fB)));
	}
}

double CUtil::getRandomNumber(double _dA, double _dB, double _dGrain /* = 10.0*/)
{
	if (_dA == _dB)
	{
		return 0.0;
	}

	int iRandomNumber = rand() * rand();

	double dRange = abs(_dA - _dB);
	int iDivisor = (int)((dRange * _dGrain) + 1.0);

	if (_dA < _dB)
	{
		return (((double)(iRandomNumber % iDivisor) / _dGrain) + (abs(_dA)));
	}
	else
	{
		return (((double)(iRandomNumber % iDivisor) / _dGrain) + (abs(_dB)));
	}
}

bool CUtil::rollDice(double _dProbability)
{
	double dDelta = _nano;                  // lowest detectable probability increment
	int iInverseDelta = (int)(1.0 / _nano); // (int) (1.0 / dDelta);

	if (_dProbability <= dDelta)
	{
		return false;
	}

	if (_dProbability >= (1.0 - dDelta))
	{
		return true;
	}

	int iRandomNumber = rand() * rand();
	int iRequiredRange = (int)(_dProbability * iInverseDelta);

	return ((iRandomNumber % iInverseDelta) <= iRequiredRange);
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Permutations & Combinations
//////////////////////////////////////////////////////////////////////////
bool CUtil::nextCombination(vector<int>& _viCombination, int _ik, int _in)
{
	int i = _ik - 1;

	++_viCombination[i];

	while ((i > 0) && (_viCombination[i] >= _in - _ik + 1 + i))
	{
		--i;
		++_viCombination[i];
	}

	if (_viCombination[0] > _in - _ik)  // Combination (n - k, n - k + 1, ... , n) reached
	{
		return false;                   // No more combinations can be generated
	}

	// Set now looks like (... , x, n, n, n, ... , n)
	// Turn it into (... , x, x + 1, x + 2, ...)
	for (i = i + 1; i < _ik; ++i)
	{
		_viCombination[i] = _viCombination[i - 1] + 1;
	}

	return true;
}

bool CUtil::combinations(vector<vector<int>>& _vviCombinations, vector<int>& _viSet, int _ik)
{
	int in = (int)_viSet.size();

	if (in <= 0)
	{
		return false;
	}

	if (_ik > in)
	{
		_ik = in;
	}

	if (_ik <= 0)
	{
		_ik = 1;
	}

	vector<int> viIndexes;

	for (int i = 0; i < _ik; ++i)
	{
		viIndexes.push_back(i);
	}

	vector<int> viSetCombination;
	viSetCombination.resize(_ik);

	do
	{
		for (int i = 0; i < _ik; ++i)
		{
			viSetCombination[i] = _viSet[viIndexes[i]];
		}
		_vviCombinations.push_back(viSetCombination);
	}
	while (nextCombination(viIndexes, _ik, in));

	return true;
}

double CUtil::get_nCk(int _in, int _ik)
{
	// checks
	if (_in < 0 || _ik < 0)
	{
		return 0;
	}

	// more checks
	if (_ik > _in)
	{
		return 0;
	}

	double nCk = 1.0;
	for (int i = 1; i <= _ik; i++)
	{
		nCk *= ((double)(_in - (i - 1)) / i);
	}
	return nCk;
}

bool CUtil::permutations(vector<vector<int>>& _vviPermutations, vector<int>& _viSet)
{
	int in = (int)_viSet.size();

	if (in <= 0)
	{
		return false;
	}

	vector<int> viIndexes;

	for (int i = 0; i < in; ++i)
	{
		viIndexes.push_back(i);
	}

	vector<int> viSetPermutation;
	viSetPermutation.resize(in);

	do
	{
		for (int i = 0; i < in; ++i)
		{
			viSetPermutation[i] = _viSet[viIndexes[i]];
		}

		_vviPermutations.push_back(viSetPermutation);
	}
	while (next_permutation(viIndexes.begin(), viIndexes.end()));

	return true;
}

bool CUtil::permutations(vector<vector<int>>& _vviPermutations, vector<int>& _viSet, int _ik)
{
	int in = (int)_viSet.size();

	if (in <= 0)
	{
		return false;
	}

	if (_ik > in)
	{
		_ik = in;
	}

	if (_ik <= 0)
	{
		_ik = 1;
	}

	vector<vector<int>> vviP;

	if (!permutations(vviP, _viSet))
	{
		return false;
	}

	map<vector<int>, int> mviP;

	int ipc = (int)vviP.size();

	for (int ipi = 0; ipi < ipc; ++ipi)
	{
		vector<int> viP;

		for (int ipj = 0; ipj < _ik; ++ipj)
		{
			int iValue = vviP[ipi][ipj];

			viP.push_back(iValue);
		}

		mviP[viP]++;
	}

	for (auto imapIndex = mviP.begin(); imapIndex != mviP.end(); ++imapIndex)
	{
		_vviPermutations.push_back((*imapIndex).first);
	}

	return true;
}

double CUtil::get_nPk(int _in, int _ik)
{
	// checks
	if (_in < 0 || _ik < 0)
	{
		return 0;
	}

	// more checks
	if (_ik > _in)
	{
		return 0;
	}

	double nPk = 1.0;
	for (int i = 0; i < _ik; i++)
	{
		nPk *= (double)(_in - i);
	}
	return nPk;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Self destruct
//////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
void CUtil::selfDestruct()
{
	// This is the name of the temporary BAT file
	const char strTemporaryBATFileName[] = "_SelfDestruct.bat";

	// Temporary .bat file data
	static char strTemporaryBATFileData[] =
		":Repeat\r\n"
		"del /a \"%s\"\r\n"
		"if exist \"%s\" goto Repeat\r\n"
		"del /a \"%s\"";

	char strModuleName[_MAX_PATH];              // absolute path of calling EXE file

	char strTemporaryBATFilePath[_MAX_PATH];    // absolute path of temporary BAT file

	GetTempPathA(_MAX_PATH, strTemporaryBATFilePath);
	strcat_s(strTemporaryBATFilePath, _MAX_PATH, strTemporaryBATFileName);

	GetModuleFileNameA(nullptr, strModuleName, MAX_PATH);

	int iBATFileDataSize = (int)(strlen(strTemporaryBATFileData) + strlen(strModuleName) * 2 + strlen(strTemporaryBATFilePath) + _MAX_PATH);
	char* pstrBATFileData = (char*)alloca(iBATFileDataSize);
	sprintf_s(pstrBATFileData, iBATFileDataSize, strTemporaryBATFileData, strModuleName, strModuleName, strTemporaryBATFilePath);

	writeFile(string(strTemporaryBATFilePath), string(pstrBATFileData));
	// free(pstrBATFileData);

	ShellExecuteA(nullptr, "open", strTemporaryBATFilePath, nullptr, nullptr, SW_HIDE);

	exit(1);
}
#endif
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Draw an animated rectangle
//////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
void WINAPI drawWireRects(LPRECT _lprcFrom, LPRECT _lprcTo, unsigned int _uiMilliSecond)
{
	const int iNumberOfSteps = 10;

	GdiFlush();
	Sleep(1);   // let the desktop window sort itself out

	// if hwnd is null - "you have the whole screen".
	HDC hDC = ::GetDC(nullptr);

	// pen size
	HPEN hPen = ::CreatePen(PS_SOLID, 2, RGB(0, 0, 0));

	int nMode = ::SetROP2(hDC, R2_NOT);
	HPEN hOldPen = (HPEN) ::SelectObject(hDC, hPen);

	for (int i = 0; i < iNumberOfSteps; i++)
	{
		double dFraction = (double)i / (double)iNumberOfSteps;

		RECT transition;
		transition.left = _lprcFrom->left + (int)((_lprcTo->left - _lprcFrom->left) * dFraction);
		transition.right = _lprcFrom->right + (int)((_lprcTo->right - _lprcFrom->right) * dFraction);
		transition.top = _lprcFrom->top + (int)((_lprcTo->top - _lprcFrom->top) * dFraction);
		transition.bottom = _lprcFrom->bottom + (int)((_lprcTo->bottom - _lprcFrom->bottom) * dFraction);

		POINT pt[5];
		pt[0] = POINT{ transition.left, transition.top };
		pt[1] = POINT{ transition.right, transition.top };
		pt[2] = POINT{ transition.right, transition.bottom };
		pt[3] = POINT{ transition.left, transition.bottom };
		pt[4] = POINT{ transition.left, transition.top };

		// we use polyline because we can determine our own pen size

		// draw sides
		::Polyline(hDC, pt, 5);

		GdiFlush();

		Sleep(_uiMilliSecond);

		// erase sides
		::Polyline(hDC, pt, 5);

		GdiFlush();
	}

	::SetROP2(hDC, nMode);
	::SelectObject(hDC, hOldPen);

	::DeleteObject(hPen);

	::ReleaseDC(nullptr, hDC);
}
#endif
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Test code
//////////////////////////////////////////////////////////////////////////
void CUtil::testTokenizer()
{
	wstring strSource = L"Ki bia korli re modon.\r\nGushti shudhdha khaili chodon.\r\nDure jae mor.\r\n.";

	wstring strDelimiter = L".\r\n.";

	// test high performance tokenizer
	{
		CTokenizer<wstring> tokenizer(strSource, strDelimiter);
		auto nTokenCount = tokenizer.getTokenCount();
		for (size_t i = 0; i < nTokenCount; ++i)
		{
			auto token = tokenizer.getToken(i);
		}
	}

	// test static tokenizer
	{
		auto nTokenCount = getTokenCount(strSource, strDelimiter);
		for (size_t i = 0; i < nTokenCount; ++i)
		{
			auto token = getToken(strSource, strDelimiter, i);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
