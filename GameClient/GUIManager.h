#pragma once

#include "ConnectedEventHandler.h"

#include "GUIScreen.h"
class GUIManager
{
public:
	GUIManager();
	~GUIManager();

public:
	void Update(float deltaTime);
	void LateUpdate();
	void Render(const Vector2Int& cameraOffset);
	void Clear();

public:
	void OnMousePressed(const Vector2Int& position);
	void OnMouseHeld(const Vector2Int& position);
	void OnMouseReleased(const Vector2Int& position);
	void OnMouseOver(const Vector2Int& position);
	void OnMouseWheel(const Vector2Int& position);
	void OnKeyBoardPressed(SDL_Scancode keyCode);
	void OnKeyBoardHeld(SDL_Scancode keyCode);
	void OnKeyBoardReleased(SDL_Scancode keyCode);
	void OnTextInput(const std::wstring& text);

private:
	GUIScreen*	FindFocusedGui(const Vector2Int& position);
	void		RenderingSortGuis();

public:
	template<typename T>
	T*	AddGui() 
	{
		GUIScreen* gui = cnew<T>();
		gui->Initialize();
		_guis[gui->GetId()] = gui;
		_nameGuisTable[gui->GetName()] = gui;
		return static_cast<T*>(gui);
	}

	template<typename T>
	T* AddGui(const std::wstring& name)
	{
		GUIScreen* gui = cnew<T>();
		gui->Initialize();
		gui->SetName(name);
		_guis[gui->GetId()] = gui;
		_nameGuisTable[name] = gui;
		return static_cast<T*>(gui);
	}

public:
	void	AddRemoveGuiId(int32 id) { _removeGuisId.push_back(id); }

public:
	const HashMap<int32, GUIScreen*>&	GetGuis() { return _guis; }
	GUIScreen*							GetGui(int32 id);
	GUIScreen*							GetGui(const std::wstring& name);

private:
	const GUIManager& operator = (const GUIManager&) = delete;
	GUIManager(const GUIManager&) = delete;

private:
	HashMap<int32, GUIScreen*>				_guis;
	HashMap<std::wstring, GUIScreen*>		_nameGuisTable;

	std::vector<ConnectedEventHandler>		_connectedEventHandlers;
	std::vector<int32>						_removeGuisId;
	GUIScreen*								_focusedGui = nullptr;
	//bool								_isAlwaysActive = false;
};

extern GUIManager* GGUIManager;