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

#pragma once

#ifdef _WIN32
#include <windows.h>
#include "atlbase.h"
#endif

#include <sys\types.h> 
#include <sys\stat.h> 

#include <vector>
#include <string>
#include <map>

using std::vector;
using std::map;

using std::string;
using std::wstring;

#ifdef UNICODE
#define _tstring        wstring
#else
#define _tstring        string
#endif

#ifdef _WIN32
#define DIRECTORY_SEPARATOR_W       L"\\"
#define DIRECTORY_SEPARATOR_W_C     L'\\'

#define DIRECTORY_SEPARATOR_A       "\\"
#define DIRECTORY_SEPARATOR_A_C     '\\'
#else
#define DIRECTORY_SEPARATOR_W       L"/"
#define DIRECTORY_SEPARATOR_W_C     L'/'

#define DIRECTORY_SEPARATOR_A       "/"
#define DIRECTORY_SEPARATOR_A_C     '/'
#endif

#ifdef UNICODE
#define DIRECTORY_SEPARATOR     DIRECTORY_SEPARATOR_W
#define DIRECTORY_SEPARATOR_C   DIRECTORY_SEPARATOR_W_C
#else
#define DIRECTORY_SEPARATOR     DIRECTORY_SEPARATOR_A
#define DIRECTORY_SEPARATOR_C   DIRECTORY_SEPARATOR_A_C
#endif

#define _milli      1.0e-3
#define _micro      1.0e-6
#define _nano       1.0e-9
#define _pico       1.0e-12
#define _femto      1.0e-15
#define _atto       1.0e-18

#define pi_d        3.141592653589793238
#define pi_f        3.1415926f

#define PERIOD_15_FPS   0.066666667f
#define PERIOD_20_FPS   0.050000000f
#define PERIOD_30_FPS   0.033333333f
#define PERIOD_60_FPS   0.016666667f
#define PERIOD_120_FPS  0.008333333f

#define DELIMITER_NEW_LINE      _T("\n")
#define DELIMITER_NEW_LINE_A    "\n"
#define DELIMITER_NEW_LINE_W    L"\n"
#define DELIMITER_COLON         _T(":")
#define DELIMITER_COLON_A       ":"
#define DELIMITER_COLON_W       L":"
#define DELIMITER_SEMICOLON     _T(";")
#define DELIMITER_SEMICOLON_A   ";"
#define DELIMITER_SEMICOLON_W   L";"
#define DELIMITER_COMMA         _T(",")
#define DELIMITER_COMMA_A       ","
#define DELIMITER_COMMA_W       L","
#define DELIMITER_SPACE         _T(" ")
#define DELIMITER_SPACE_A       " "
#define DELIMITER_SPACE_W       L" "

class CUtil
{
public:
	CUtil(void);
	~CUtil(void);

public:
	//////////////////////////////////////////////////////////////////////////
	// File IO functions
	//////////////////////////////////////////////////////////////////////////
	// Read/Write data to/from file
	static bool readFile(const wstring& _strFilePath, wstring& _strFileData);
	static bool readFile(const wstring& _strFilePath, string& _strFileData);
	static bool readFile(const string& _strFilePath, wstring& _strFileData);
	static bool readFile(const string& _strFilePath, string& _strFileData);

	static bool writeFile(const wstring& _strFilePath, const wstring& _strFileData);
	static bool writeFile(const wstring& _strFilePath, const string& _strFileData);
	static bool writeFile(const string& _strFilePath, const wstring& _strFileData);
	static bool writeFile(const string& _strFilePath, const string& _strFileData);

	static bool appendFile(const wstring& _strFilePath, const wstring& _strFileData);
	static bool appendFile(const wstring& _strFilePath, const string& _strFileData);
	static bool appendFile(const string& _strFilePath, const wstring& _strFileData);
	static bool appendFile(const string& _strFilePath, const string& _strFileData);

	// Get the file size in bytes
	static int getFileSize(const wstring& _strFilePath);
	static int getFileSize(const string& _strFilePath);
	static __int64 getFileSize64(const wstring& _strFilePath);
	static __int64 getFileSize64(const string& _strFilePath);

	// Check to see if its a valid file
	static bool isFile(const wstring& _strFilePath);
	static bool isFile(const string& _strFilePath);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// String manipulation functions
	//////////////////////////////////////////////////////////////////////////
	// Returns the number of successful substrings replaced
	static int findReplace(wstring& _strSource, const wstring& _strFind, const wstring& _strReplace);
	static int findReplace(string& _strSource, const string& _strFind, const string& _strReplace);

	// Removes/Replaces all white spaces
	static void removeWhiteSpaces(wstring& _strSource, const wstring& _strReplace = L"");
	static void removeWhiteSpaces(string& _strSource, const string& _strReplace = "");

	// Create padding
	static wstring createPadding(unsigned int _uiLength, const wstring& _strChars2Pad = L" ");
	static string createPadding(unsigned int _uiLength, const string& _strChars2Pad = " ");

	// Remove/Replace all characters that are not alpha numeric with specified characters
	static wstring removeGarbage(wstring& _strSource, wchar_t _wch = L'\0');
	static string removeGarbage(string& _strSource, char _ch = '\0');

	// A function to trim strings from both ends
	static wstring trim(wstring& _strSource, const wstring& _strChars2Trim = L" \t\r\n", bool _bTrimLeft = true, bool _bTrimRight = true);
	static string trim(string& _strSource, const string& _strChars2Trim = " \t\r\n", bool _bTrimLeft = true, bool _bTrimRight = true);

	// Format a string
	static string format(const string _str, ...);
	static wstring format(const wstring _str, ...);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Find URL
	//////////////////////////////////////////////////////////////////////////
	static wstring findURL(const wstring& _strSource);
	static string findURL(const string& _strSource);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// String tokenizing functions
	//////////////////////////////////////////////////////////////////////////
	static size_t getTokenCount(const wstring& _strSource, const wstring& _strDelimiter);
	static size_t getTokenCount(const string& _strSource, const string& _strDelimiter);

	static wstring getToken(const wstring& _strSource, const wstring& _strDelimiter, size_t _nTokenIndex);
	static string getToken(const string& _strSource, const string& _strDelimiter, size_t _nTokenIndex);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Array operations
	//////////////////////////////////////////////////////////////////////////
	static int findInArray(const wstring* _parrstr, int _iparrstrSize, const wstring& _strFind);
	static int findInArray(const string* _parrstr, int _iparrstrSize, const string& _strFind);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Clipboard
	//////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
	static bool copyToClipboard(const _tstring& _strText);
	static _tstring pasteFromClipboard();
#endif
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Numeric functions
	//////////////////////////////////////////////////////////////////////////
#ifdef UNICODE
#define toString toStringW
#else
#define toString toStringA
#endif

	static bool isNear(double _d1, double _d2, double _dThreshold = 0.1);

	static unsigned int numberOfDigits(int _iNumber);

	static double round(double _dNumber2Round, unsigned int _uiPlaces);
	static int round(double _dNumber2Round);

	static bool isPowerOf2(unsigned int _uiNumber);
	static unsigned int getNextPowerOf2(unsigned int _uiNumber);

	static wstring toStringW(int _iNumber, unsigned int _uiPrecision);
	static string toStringA(int _iNumber, unsigned int _uiPrecision);

	static wstring toStringW(int _iNumber);
	static string toStringA(int _iNumber);

	static wstring toStringW(__int64 _i64Number);
	static string toStringA(__int64 _i64Number);

	static wstring toStringW(double _dNumber, unsigned int _uiPrecision = 0);
	static string toStringA(double _dNumber, unsigned int _uiPrecision = 0);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// String case converters, upper <-> _iower
	//////////////////////////////////////////////////////////////////////////
	static wstring toUpper(const wstring& _str);
	static string toUpper(const string& _str);

	static wstring toLower(const wstring& _str);
	static string toLower(const string& _str);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Compare without case sensitivity
	//////////////////////////////////////////////////////////////////////////
	static int compareNoCase(const wstring& _str1, const wstring& _str2);
	static int compareNoCase(const string& _str1, const string& _str2);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Compare with wildcard
	//////////////////////////////////////////////////////////////////////////
	static bool compareWildcard(const wchar_t* _pstr, const wchar_t* _pstrWildcard);
	static bool compareWildcard(const char* _pstr, const char* _pstrWildcard);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// System time retrieval
	//////////////////////////////////////////////////////////////////////////
#ifdef UNICODE
#define getSystemTime getSystemTimeW
#else
#define getSystemTime getSystemTimeA
#endif

	static wstring getSystemTimeW(const wstring& _strDateFormat = L"%A, %d/%m/%Y, %H:%M:%S - %Z");
	static string getSystemTimeA(const string& _strDateFormat = "%A, %d/%m/%Y, %H:%M:%S - %Z");

	static wstring getSystemTimeW(const time_t& _ttTime, const wstring& _strDateFormat = L"%A, %d/%m/%Y, %H:%M:%S - %Z");
	static string getSystemTimeA(const time_t& _ttTime, const string& _strDateFormat = "%A, %d/%m/%Y, %H:%M:%S - %Z");
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Convert given number of seconds to hh:mm:ss and vice versa
	//////////////////////////////////////////////////////////////////////////
#ifdef UNICODE
#define getTime getTimeW
#else
#define getTime getTimeA
#endif

	static wstring getTimeW(int _iSeconds);
	static string getTimeA(int _iSeconds);

	static int getTimeW(const wstring& _strTime = L"00:00:00");
	static int getTimeA(const string& _strTime = "00:00:00");

	//////////////////////////////////////////////////////////////////////////
	// Retrieve time_t from string: dd/mm/yyyy, hh:mm:ss
	//////////////////////////////////////////////////////////////////////////
#ifdef UNICODE
#define makeTime makeTimeW
#else
#define makeTime makeTimeA
#endif

	static time_t makeTimeW(const wstring& _strDateTime = L"00/00/0000, 00:00:00");
	static time_t makeTimeA(const string& _strDateTime = "00/00/0000, 00:00:00");
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Get commonly used directory paths
	//////////////////////////////////////////////////////////////////////////
#ifdef UNICODE
#define getWorkingDirectory getWorkingDirectoryW

#ifdef _WIN32
#define getProgramDirectory getProgramDirectoryW
#define getProgramPath      getProgramPathW
#define getProgramName      getProgramNameW
#endif
#else
#define getWorkingDirectory getWorkingDirectoryA

#ifdef _WIN32
#define getProgramDirectory getProgramDirectoryA
#define getProgramPath      getProgramPathA
#define getProgramName      getProgramNameA
#endif
#endif

	static wstring getWorkingDirectoryW();
	static string getWorkingDirectoryA();

#ifdef _WIN32
	static wstring getProgramPathW();
	static string getProgramPathA();

	static wstring getProgramDirectoryW();
	static string getProgramDirectoryA();

	static wstring getProgramNameW(bool _bIncludeExtension = false);
	static string getProgramNameA(bool _bIncludeExtension = false);
#endif

#ifdef _WIN32
	static _tstring getProgramFilesDirectory();
	static _tstring getWindowsDirectory();
	static _tstring getSystemDirectory();
	static _tstring getMyDocumentsDirectory();
	static _tstring getMyMusicDirectory();
	static _tstring getMyPicturesDirectory();
	static _tstring getMyVideosDirectory();
	static _tstring getAppDataDirectory();
	static _tstring getLocalAppDataDirectory();
	static _tstring getDesktopDirectory();
	static _tstring getStartupDirectory();
#endif
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Copy or cut a file into another directory, creating the directory path if necessary
	//////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
	static bool copyFileToDirectory(const _tstring& _strSourceFilePath, const _tstring& _strDestinationDirectory, bool _bDeleteSource = false);
#endif
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Directory/File manipulation functions
	//////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
	// Get list of all files in the target directory
	static void getFileList(const _tstring& _strTargetDirectoryPath,
		const _tstring& _strWildcard,
		bool _bLookInSubdirectories,
		vector<_tstring>& _vstrFileList,
		int _iPause = 0);

	// Rename all files in the target directory
	static _tstring renameFiles(const _tstring& _strTargetDirectoryPath,
		const _tstring& _strWildcard,
		bool _bLookInSubdirectories,
		const _tstring& _strFind,
		const _tstring& _strReplace,
		bool _bAppendNumbers);
#endif

	// Create the entire directory path
	static void makeDirectory(const wstring& _strDirectoryPath);
	static void makeDirectory(const string& _strDirectoryPath);

#ifdef _WIN32
	// Delete the entire directory path, including all files and folders within
	static void deleteDirectory(const _tstring& _strTargetDirectoryPath);

	// Delete files from directory
	static void deleteFiles(const _tstring& _strTargetDirectoryPath, const _tstring& _strWildcard, bool _bLookInSubdirectories);

	// Check whether the given path is a directory
	static bool isDirectory(const wstring& _strDirectoryPath);
	static bool isDirectory(const string& _strDirectoryPath);
#endif
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Directory/File path string manipulation functions
	//////////////////////////////////////////////////////////////////////////
	// homogenize path, i.e. ensure there are either "\" or "/", but not both
	static wstring homogenizePath(wstring _strPath);
	static string homogenizePath(string _strPath);

	// Add "\" to the end of a directory path, if not present
	static wstring addDirectoryEnding(const wstring& _strDirectoryPath);
	static string addDirectoryEnding(const string& _strDirectoryPath);

	// Remove "\" from the end of a directory path, if present
	static wstring removeDirectoryEnding(const wstring& _strDirectoryPath);
	static string removeDirectoryEnding(const string& _strDirectoryPath);

	// Get the previous directory from a given directory path
	static wstring getRootDirectory(const wstring& _strDirectoryPath);
	static string getRootDirectory(const string& _strDirectoryPath);

	// Get the name of the directory form a given directory path: e.g. C:\Program Files\XYZ, will return XYZ
	static wstring getDirectoryName(const wstring& _strDirectoryPath);
	static string getDirectoryName(const string& _strDirectoryPath);

	// Get the directory from a file path
	static wstring getFileDirectory(const wstring& _strFilePath);
	static string getFileDirectory(const string& _strFilePath);

	// Get the file name including/excluding the extension from a given file path
	static wstring getFileName(const wstring& _strFilePath, bool _bIncludeExtension = false);
	static string getFileName(const string& _strFilePath, bool _bIncludeExtension = false);

	// Get the file extension including/excluding the "." from a given file path
	static wstring getFileExtension(const wstring& _strFilePath, bool _bIncludeDot = false);
	static string getFileExtension(const string& _strFilePath, bool _bIncludeDot = false);

	// Get the file prefix / suffix
	static wstring getFileNamePrefix(const wstring& _strFilePath, const wstring& _strDelimiter);
	static string getFileNamePrefix(const string& _strFilePath, const string& _strDelimiter);

	static wstring getFileNameSuffix(const wstring& _strFilePath, const wstring& _strDelimiter);
	static string getFileNameSuffix(const string& _strFilePath, const string& _strDelimiter);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// String type converters, ANSI <-> UNICODE
	//////////////////////////////////////////////////////////////////////////
	static char wide2Narrow(wchar_t _wch);
	static wchar_t narrow2Wide(char _ch);

	static wstring getStringW(const string& _strA);
	static wstring getStringW(const wstring& _strW) { return _strW; };
	static wstring getString(const string& _strA);

	static string getStringA(const wstring& _strW);
	static string getStringA(const string& _strA) { return _strA; };
	static string getString(const wstring& _strW);

	static wstring utf8_to_utf16(const string& _strUTF8);
	static string utf16_to_utf8(const wstring& _strUTF16);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Registry
	//////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
	static _tstring getRegistryInfo(HKEY _hKey, const _tstring& _strRegistryPath, const _tstring& _strValueName);
	static DWORD getRegistryValue(HKEY _hKey, const _tstring& _strRegistryPath, const _tstring& _strValueName);

	static bool setRegistryInfo(HKEY _hkey, const _tstring& _strRegistryPath, const _tstring& _strValueName, const _tstring& _strValue);
	static bool setRegistryValue(HKEY _hkey, const _tstring& _strRegistryPath, const _tstring& _strValueName, DWORD dwValue);
#endif
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Internet
	//////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
private:
	static LONG getRegKey(HKEY _hkey, LPCTSTR _subkey, LPTSTR _regdata);    // Only used by OpenURL()

public:
	static HINSTANCE openURL(LPCTSTR _strURL);

	static _tstring getMyPublicIP(const _tstring& _strURL = _T("http://bot.whatismyipaddress.com"));
#endif
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Execution
	//////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
	static bool execute(const _tstring& _strCommandLine, bool _bShow = true, bool _bWait = false);
#endif
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// System
	//////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
	static int getProcessorCount();

	static bool is_x64();
#endif

	static void pause(unsigned int _uiMilliSeconds);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Random generators
	//////////////////////////////////////////////////////////////////////////
#ifdef UNICODE
#define generateRandomString generateRandomStringW
#else
#define generateRandomString generateRandomStringA
#endif

	// Generate random string
	static wstring generateRandomStringW(int _iMaxLength, bool _bIncludeAlpha = true, bool _bIncludeNumbers = true);
	static string generateRandomStringA(int _iMaxLength, bool _bIncludeAlpha = true, bool _bIncludeNumbers = true);

	// Generate random number between A & B
	static float getRandomNumber(float _fA, float _fB, float _fGrain = 10.0f);
	static double getRandomNumber(double _dA, double _dB, double _dGrain = 10.0);

	// Simulate die roll in a Bernoulli trial
	static bool rollDice(double _dProbability);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Permutations & Combinations
private:
	static bool nextCombination(vector<int>& _viCombination, int _ik, int _in);

public:
	static bool combinations(vector<vector<int>>& _vviCombinations, vector<int>& _viSet, int _ik);
	static double get_nCk(int _in, int _ik);

	static bool permutations(vector<vector<int>>& _vviPermutations, vector<int>& _viSet);
	static bool permutations(vector<vector<int>>& _vviPermutations, vector<int>& _viSet, int _ik);
	static double get_nPk(int _in, int _ik);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Self destruct code
	//////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
	static void selfDestruct();
#endif
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Draw an animated rectangle
	//////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
	static void WINAPI drawWireRects(LPRECT _iprcFrom, LPRECT _iprcTo, unsigned int _uiMilliSecond);
#endif
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Test code
	//////////////////////////////////////////////////////////////////////////
	static void testTokenizer();
	//////////////////////////////////////////////////////////////////////////
};

//////////////////////////////////////////////////////////////////////////
// String tokenizer
//////////////////////////////////////////////////////////////////////////
template <class T>
class CTokenizer
{
public:
	CTokenizer(const T& _strSource, const T& _strDelimiter);
	~CTokenizer();

public:
	size_t getTokenCount();
	T getToken(size_t _nTokenIndex);

private:
	vector<T> m_vTokens;
};

template <class T>
CTokenizer<T>::CTokenizer(const T& _strSource, const T& _strDelimiter)
{
	if (_strSource.empty() || _strDelimiter.empty())
	{
		return;
	}

	size_t nSourceLength = _strSource.size();
	size_t nDelimiterLength = _strDelimiter.size();

	size_t nStartIndex = 0;

	while (nStartIndex < nSourceLength)
	{
		size_t nEndIndex = 0;
		T strTemp;

		nEndIndex = _strSource.find(_strDelimiter, nStartIndex);

		if (nEndIndex != T::npos)
		{
			strTemp = _strSource.substr(nStartIndex, nEndIndex - nStartIndex);
			nStartIndex = nEndIndex + nDelimiterLength;
		}
		else
		{
			strTemp = _strSource.substr(nStartIndex);
			nStartIndex = nEndIndex;
		}
		m_vTokens.push_back(strTemp);
	}
}

template <class T>
CTokenizer<T>::~CTokenizer()
{
	// do nothing
}

template <class T>
size_t CTokenizer<T>::getTokenCount()
{
	return m_vTokens.size();
}

template <class T>
T CTokenizer<T>::getToken(size_t _nTokenIndex)
{
	if (_nTokenIndex >= m_vTokens.size() || _nTokenIndex == T::npos)
	{
		return T();
	}
	return m_vTokens[_nTokenIndex];
}
//////////////////////////////////////////////////////////////////////////
