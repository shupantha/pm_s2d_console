#include "stdafx.h"
#include "Settings.h"

#define SETTINGS_FILE_NAME			_T("settings.txt")

CSettings::CSettings()
{
	wallSize = 0;
	textFont = "";

	Load();
}

CSettings::~CSettings()
{
}

void CSettings::Load()
{
	Load(SETTINGS_FILE_NAME);
}

void CSettings::Load(_tstring strSettingsFileName)
{
	std::string strFilePath = "./msc/";
	strFilePath += CUtil::getStringA(strSettingsFileName);

	std::string strFileData = "";
	if (!CUtil::readFile(strFilePath, strFileData))
	{
		return;
	}

	CTokenizer<std::string> tFileData(strFileData, DELIMITER_NEW_LINE_A);
	for (size_t i = 0; i < tFileData.getTokenCount(); i++)
	{
		std::string strLineData = tFileData.getToken(i);
		CUtil::trim(strLineData);

		CTokenizer<std::string> tLineData(strLineData, DELIMITER_SPACE_A);
		if (tLineData.getTokenCount() <= 1)
		{
			continue;
		}

		// parse each line
		std::string strAttribute = tLineData.getToken(0);
		CUtil::trim(strAttribute);

		if (CUtil::compareNoCase(strAttribute, "WALL_SIZE") == 0)
		{
			wallSize = atoi(tLineData.getToken(1).c_str());
			continue;
		}

		if (CUtil::compareNoCase(strAttribute, "WALL_COLOR") == 0)
		{
			byte r = (byte)atoi(tLineData.getToken(1).c_str());
			byte g = (byte)atoi(tLineData.getToken(2).c_str());
			byte b = (byte)atoi(tLineData.getToken(3).c_str());
			byte a = (byte)atoi(tLineData.getToken(4).c_str());
			wallColor.Set(r, g, b, a);
			continue;
		}

		if (CUtil::compareNoCase(strAttribute, "PATH_COLOR") == 0)
		{
			byte r = (byte)atoi(tLineData.getToken(1).c_str());
			byte g = (byte)atoi(tLineData.getToken(2).c_str());
			byte b = (byte)atoi(tLineData.getToken(3).c_str());
			byte a = (byte)atoi(tLineData.getToken(4).c_str());
			pathColor.Set(r, g, b, a);
			continue;
		}

		if (CUtil::compareNoCase(strAttribute, "PATH_CLICK_COLOR") == 0)
		{
			byte r = (byte)atoi(tLineData.getToken(1).c_str());
			byte g = (byte)atoi(tLineData.getToken(2).c_str());
			byte b = (byte)atoi(tLineData.getToken(3).c_str());
			byte a = (byte)atoi(tLineData.getToken(4).c_str());
			pathClickColor.Set(r, g, b, a);
			continue;
		}

		if (CUtil::compareNoCase(strAttribute, "PILL_COLOR") == 0)
		{
			byte r = (byte)atoi(tLineData.getToken(1).c_str());
			byte g = (byte)atoi(tLineData.getToken(2).c_str());
			byte b = (byte)atoi(tLineData.getToken(3).c_str());
			byte a = (byte)atoi(tLineData.getToken(4).c_str());
			pillColor.Set(r, g, b, a);
			continue;
		}

		if (CUtil::compareNoCase(strAttribute, "TEXT_FONT") == 0)
		{
			textFont = tLineData.getToken(1);
			CUtil::trim(textFont);
			continue;
		}

		if (CUtil::compareNoCase(strAttribute, "TEXT_COLOR") == 0)
		{
			byte r = (byte)atoi(tLineData.getToken(1).c_str());
			byte g = (byte)atoi(tLineData.getToken(2).c_str());
			byte b = (byte)atoi(tLineData.getToken(3).c_str());
			byte a = (byte)atoi(tLineData.getToken(4).c_str());
			textColor.Set(r, g, b, a);
			continue;
		}
	}
}

void CSettings::LoadNext()
{
	static unsigned int count = 0;

	_tstring strProgramDirectory = CUtil::getProgramDirectory();
	strProgramDirectory = CUtil::addDirectoryEnding(strProgramDirectory);
	strProgramDirectory += _T("msc");

	std::vector<_tstring> vFileList;
	CUtil::getFileList(strProgramDirectory, _T("*.txt"), false, vFileList);

	if (vFileList.size() >= 0)
	{
		int index = count % vFileList.size();
		_tstring strFileName = CUtil::getFileName(vFileList[index], true);
		Load(strFileName);
	}

	count++;
}
