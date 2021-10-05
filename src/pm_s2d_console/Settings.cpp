#include "stdafx.h"
#include "Settings.h"

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
	std::string strFileData = "";
	if (!CUtil::readFile("./msc/settings.txt", strFileData))
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
