#pragma once

class IniFileParser
{
public:
	IniFileParser();
	~IniFileParser();
	
public:
	void SetFileName(const std::wstring& fileName);
	

	void SetSectionValue(const std::wstring& sectionName, const std::wstring& keyName, int value);
	void SetSectionValue(const std::wstring& sectionName, const std::wstring& keyName, float value);
	void SetSectionValue(const std::wstring& sectionName, const std::wstring& keyName, const WCHAR* value);
	void SetSectionValue(const std::wstring& sectionName, const std::wstring& keyName, const std::wstring& value);

	void GetSectionValue(const std::wstring& sectionName, const std::wstring& keyName, OUT int& value);
	void GetSectionValue(const std::wstring& sectionName, const std::wstring& keyName, OUT float& value);
	void GetSectionValue(const std::wstring& sectionName, const std::wstring& keyName, OUT WCHAR* value, int32 length);
	void GetSectionValue(const std::wstring& sectionName, const std::wstring& keyName, OUT std::wstring& value);

private:
	std::wstring _fileName;
};
