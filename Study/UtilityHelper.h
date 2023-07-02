#pragma once

class UtilityHelper
{
public:
	static std::vector<BYTE>		ReadFile(const WCHAR* path);
	static std::wstring			ConvertUTF8ToUnicode(std::string str);
	static std::string			ConvertUnicodeToUTF8(std::wstring wstr);
};

