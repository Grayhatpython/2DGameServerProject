#pragma once
#include "Component.h"
#include "GUIManager.h"

class GUIComponent : public Component
{
	COMPONENT_TYPE_DECLARATION(GUIComponent);

public:
	GUIComponent();
	virtual ~GUIComponent();

public:
	template<typename T>
	T* AddGui()
	{
		GUIScreen* gui = GGUIManager->AddGui<T>();
		ASSERT(gui);

		_guis[gui->GetName()] = gui;
		return static_cast<T*>(gui);
	}

	//	��.. GUI �������� ����..
	void RemoveGui(const std::wstring& name)
	{
		auto iter = _guis.find(name);
		if (iter != _guis.end())
		{
			iter->second->SetState(GUIState::Remove);
			_guis.erase(iter);
		}
	}

	GUIScreen* FindGuiFromName(const std::wstring& name)
	{
		auto iter = _guis.find(name);
		if (iter != _guis.end())
			return iter->second;

		return nullptr;
	}

private:
	//	Pool -> �۽�.. GUI�� ��� ���� �� ������� ����������..
	unordered_map<std::wstring, GUIScreen*>	_guis;
};

