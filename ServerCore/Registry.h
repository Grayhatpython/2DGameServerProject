#pragma once

class Registry
{
public:
	Registry();
	~Registry();

public:
	bool OpenKey(HKEY rootKey,  LPCTSTR subKey);
	bool CloseKey();

	bool CreateKey(HKEY rootKey, LPCTSTR subKey);
	bool DeleteKey(HKEY rootKey, LPCTSTR subKey);

	bool SetValue(LPCTSTR valueName, DWORD value);
	bool SetValue(LPCTSTR valueName, LPCTSTR value);
	bool SetValue(LPCTSTR valueName, LPCTSTR value, DWORD length);

	bool GetValue(LPCTSTR valueName, DWORD* value);
	bool GetValue(LPCTSTR valueName, LPTSTR value, DWORD* length);

	bool DeleteValue(LPCTSTR valueName);

private:
	HKEY	_rootKey = nullptr;
	bool	_isOpened = false;
};

/*
template<typename T>
inline bool Registry::SetValue(LPCTSTR valueName, T value)
{
	static_assert(std::is_same<T, DWORD>::value || std::is_same<T, LPCTSTR>::value, L"Not a supported type");

	if (valueName == nullptr || _isOpened == false)
		return false;

	if (std::is_pointer<T>::value)
	{
		if (value == nullptr)
			return false;

		auto result = RegSetValueEx(_rootKey, valueName, 0, REG_SZ, reinterpret_cast<const BYTE*>(value), static_cast<DWORD>(_tcslen(value) * sizeof(TCHAR)));
		if (result != ERROR_SUCCESS)
			return false;
	}
	else
	{
		auto result = RegSetValueEx(_rootKey, valueName, 0, REG_DWORD, reinterpret_cast<const BYTE*>(value), sizeof(DWORD));
		if (result != ERROR_SUCCESS)
			return false;
	}


	return true;
}
*/