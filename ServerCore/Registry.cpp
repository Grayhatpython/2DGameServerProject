#include "pch.h"
#include "Registry.h"

Registry::Registry()
{

}

Registry::~Registry()
{

}

bool Registry::OpenKey(HKEY rootKey, LPCTSTR subKey)
{
	if (subKey == nullptr || _isOpened)
		return false;

	auto result = RegOpenKeyEx(rootKey, subKey, 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &_rootKey);
	if (result != ERROR_SUCCESS)
		return false;

	_isOpened = true;

	return true;
}

bool Registry::CloseKey()
{
	auto result = ::RegCloseKey(_rootKey);
	if (result != ERROR_SUCCESS)
		return false;

	_isOpened = false;

	return true;
}

bool Registry::CreateKey(HKEY rootKey, LPCTSTR subKey)
{
	if (subKey == nullptr || _isOpened)
		return false;

	DWORD dwDisposition = 0;
	auto result = RegCreateKeyEx(rootKey, subKey, 0, nullptr, REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS | KEY_WOW64_64KEY, nullptr, &_rootKey, &dwDisposition );
	if (result != ERROR_SUCCESS)
		return false;

	_isOpened = true;

	return true;
}

bool Registry::DeleteKey(HKEY rootKey, LPCTSTR subKey)
{
	//	TEMP
	if (subKey == nullptr)
		return false;

	auto result = RegDeleteKeyEx(rootKey, subKey, KEY_WOW64_64KEY, 0);
	if (result != ERROR_SUCCESS)
		return false;

	return true;
}

bool Registry::SetValue(LPCTSTR valueName, DWORD value)
{
	if (valueName == nullptr  || _isOpened == false)
		return false;

	auto result = RegSetValueEx(_rootKey, valueName, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&value), sizeof(DWORD));
	if (result != ERROR_SUCCESS)
		return false;

	return true;
}

bool Registry::SetValue(LPCTSTR valueName, LPCTSTR value)
{
	if (valueName == nullptr || value == nullptr || _isOpened == false)
		return false;

	auto result = RegSetValueEx(_rootKey, valueName, 0, REG_SZ, reinterpret_cast<const BYTE*>(value), static_cast<DWORD>(_tcslen(value) * sizeof(TCHAR)));
	if (result != ERROR_SUCCESS)
		return false;

	return false;
}

bool Registry::SetValue(LPCTSTR valueName, LPCTSTR value, DWORD length)
{
	if (valueName == nullptr || value == nullptr || _isOpened == false)
		return false;
	
	//	TEMP
	//	REG_MULTI_SZ => TWO NULL STRING
	auto result = RegSetValueEx(_rootKey, valueName, 0, REG_MULTI_SZ, reinterpret_cast<const BYTE*>(value), length);
	if (result != ERROR_SUCCESS)
		return false;
	return true;
}

bool Registry::GetValue(LPCTSTR valueName, DWORD* value)
{
	DWORD valueType = 0;
	DWORD valueLength = sizeof(DWORD);

	if (valueName == nullptr || value == nullptr || _isOpened == false)
		return false;

	auto result = RegQueryValueEx(_rootKey, valueName, 0, &valueType, reinterpret_cast<BYTE*>(value), &valueLength);
	if (result != ERROR_SUCCESS)
		return false;

	return true;
}

bool Registry::GetValue(LPCTSTR valueName, LPTSTR value, DWORD* length)
{
	DWORD valueType = 0;

	if (valueName == nullptr || value == nullptr || length == nullptr || _isOpened == false)
		return false;

	auto result = RegQueryValueEx(_rootKey, valueName, 0, &valueType, reinterpret_cast<BYTE*>(value), length);
	if (result != ERROR_SUCCESS)
		return false;

	return true;
}

bool Registry::DeleteValue(LPCTSTR valueName)
{
	if (valueName == nullptr ||  _isOpened == false)
		return false;
	
	auto result = RegDeleteValue(_rootKey, valueName);
	if (result != ERROR_SUCCESS)
		return false;

	return true;
}
