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

	//	음.. GUI 없앨지는 아직..
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
	//	Pool -> 글쎄.. GUI가 어떻게 보면 잘 사라지고 생성될지도..
	unordered_map<std::wstring, GUIScreen*>	_guis;
};

