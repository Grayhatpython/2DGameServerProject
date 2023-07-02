#include "pch.h"
#include "IniFileParser.h"

IniFileParser::IniFileParser()
{

}

IniFileParser::~IniFileParser()
{

}

void IniFileParser::SetFileName(const std::wstring& fileName)
{
	_fileName = fileName;
}

void IniFileParser::SetSectionValue(const std::wstring& sectionName, const std::wstring& keyName, int value)
{
	std::wstring str = std::to_wstring(value);
	WritePrivateProfileString(sectionName.c_str(), keyName.c_str(), str.c_str(), _fileName.c_str());
}

void IniFileParser::SetSectionValue(const std::wstring& sectionName, const std::wstring& keyName, float value)
{
	std::wstring str = std::to_wstring(value);
	WritePrivateProfileString(sectionName.c_str(), keyName.c_str(), str.c_str(), _fileName.c_str());
}

void IniFileParser::SetSectionValue(const std::wstring& sectionName, const std::wstring& keyName, const WCHAR* value)
{
	WritePrivateProfileString(sectionName.c_str(), keyName.c_str(), value, _fileName.c_str());
}

void IniFileParser::SetSectionValue(const std::wstring& sectionName, const std::wstring& keyName, const std::wstring& value)
{
	WritePrivateProfileString(sectionName.c_str(), keyName.c_str(), value.c_str(), _fileName.c_str());
}

void IniFileParser::GetSectionValue(const std::wstring& sectionName, const std::wstring& keyName, OUT int& value)
{
	value = GetPrivateProfileInt(sectionName.c_str(), keyName.c_str(), 0, _fileName.c_str());
}

void IniFileParser::GetSectionValue(const std::wstring& sectionName, const std::wstring& keyName, OUT float& value)
{
	WCHAR str[32] = { 0, };

	GetPrivateProfileString(sectionName.c_str(), keyName.c_str(), L"", str, 32, _fileName.c_str());
	value = std::stof(str);
}

void IniFileParser::GetSectionValue(const std::wstring& sectionName, const std::wstring& keyName, OUT WCHAR* value, int32 length)
{
	GetPrivateProfileString(sectionName.c_str(), keyName.c_str(), L"", value, length, _fileName.c_str());
}

void IniFileParser::GetSectionValue(const std::wstring& sectionName, const std::wstring& keyName, OUT std::wstring& value)
{
	WCHAR str[MAX_PATH];
	GetPrivateProfileString(sectionName.c_str(), keyName.c_str(), L"", str, MAX_PATH, _fileName.c_str());
	value = str;
}

