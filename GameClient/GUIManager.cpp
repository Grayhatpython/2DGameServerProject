#include "pch.h"
#include "GUIManager.h"
#include "GUIScreen.h"
#include "Button.h"
#include "InputManager.h"

GUIManager* GGUIManager = nullptr;

GUIManager::GUIManager()
{
	_connectedEventHandlers.push_back(GInputManager->RegisterMouseReleasedEvent(this, &GUIManager::OnMouseReleased));
	_connectedEventHandlers.push_back(GInputManager->RegisterMousePressedEvent(this, &GUIManager::OnMousePressed));
	_connectedEventHandlers.push_back(GInputManager->RegisterMouseHeldEvent(this, &GUIManager::OnMouseHeld));
	_connectedEventHandlers.push_back(GInputManager->RegisterMouseOverEvent(this, &GUIManager::OnMouseOver));
	_connectedEventHandlers.push_back(GInputManager->RegisterMouseWheelEvent(this, &GUIManager::OnMouseWheel));
	_connectedEventHandlers.push_back(GInputManager->RegisterKeyBoardReleasedEvent(this, &GUIManager::OnKeyBoardReleased));
	_connectedEventHandlers.push_back(GInputManager->RegisterKeyBoardHeldEvent(this, &GUIManager::OnKeyBoardHeld));
	_connectedEventHandlers.push_back(GInputManager->RegisterTextInputEvent(this, &GUIManager::OnTextInput));
}

GUIManager::~GUIManager()
{
	//	등록된 이벤트 연결 해제
	for (auto& eventHandler : _connectedEventHandlers)
		eventHandler.Disconnect();

	Clear();
}

void GUIManager::Update(float deltaTime)
{
	for (auto& gui : _guis)
	{
		if(gui.second->GetState() == GUIState::Active)
			gui.second->Update(deltaTime);
	}
}

void GUIManager::LateUpdate()
{
	for (auto id : _removeGuisId)
	{
		auto gui = _guis.find(id);
		if (gui != _guis.end())
		{
			auto nameGui = _nameGuisTable.find(gui->second->GetName());
			if (nameGui != _nameGuisTable.end())
				_nameGuisTable.erase(nameGui);

			cdelete(gui->second);
			_guis.erase(gui);
		}
	}
	
	_removeGuisId.clear();
}

void GUIManager::Render(const Vector2Int& cameraOffset)
{
	for (auto& gui : _guis)
	{
		if (gui.second->GetState() == GUIState::DeActive)
			continue;

		gui.second->Render(cameraOffset);
	}
}

void GUIManager::Clear()
{
	for (auto& gui : _guis)
		cdelete(gui.second);

	for (auto& gui : _nameGuisTable)
		gui.second = nullptr;

	_guis.clear();
	_nameGuisTable.clear();
}


void GUIManager::OnMousePressed(const Vector2Int& position)
{
	if (_focusedGui && _focusedGui->GetState() == GUIState::Active)
	{
		if (_focusedGui->IsInRange(position))
			_focusedGui->OnMousePressed(position);
		else
		{
			_focusedGui = FindFocusedGui(position);

			if (_focusedGui)
			{
				RenderingSortGuis();
				_focusedGui->OnMousePressed(position);
			}
		}
	}
	else
	{
		_focusedGui = FindFocusedGui(position);

		if (_focusedGui)
		{
			RenderingSortGuis();
			_focusedGui->OnMousePressed(position);
		}
	}
}

void GUIManager::OnMouseHeld(const Vector2Int& position)
{
	if (_focusedGui && _focusedGui->GetState() == GUIState::Active)
			_focusedGui->OnMouseHeld(position);
}

void GUIManager::OnMouseReleased(const Vector2Int& position)
{
	if (_focusedGui && _focusedGui->GetState() == GUIState::Active)
	{
		if (_focusedGui->IsInRange(position))
			_focusedGui->OnMouseReleased(position);
	}

	auto loginForm = _nameGuisTable.find(L"LoginForm");
	if (loginForm != _nameGuisTable.end())
		loginForm->second->OnMouseReleased(position);

	auto chatBox = _nameGuisTable.find(L"ChatBox");
	if (chatBox != _nameGuisTable.end())
		chatBox->second->OnMouseReleased(position);
}

void GUIManager::OnMouseOver(const Vector2Int& position)
{
	for (auto& gui : _guis)
		gui.second->OnMouseOver(position);
}

void GUIManager::OnMouseWheel(const Vector2Int& position)
{
	if (_focusedGui && _focusedGui->GetState() == GUIState::Active)
		_focusedGui->OnMouseWheel(position);
}

void GUIManager::OnKeyBoardReleased(SDL_Scancode keyCode)
{
	for (auto& gui : _guis)
		gui.second->OnKeyBoardReleased(keyCode);
}

void GUIManager::OnTextInput(const std::wstring& text)
{
	if (_focusedGui && _focusedGui->GetState() == GUIState::Active)
		_focusedGui->OnTextInput(text);
}

void GUIManager::OnKeyBoardPressed(SDL_Scancode keyCode)
{
	/*for (auto& gui : _guis)
	{
		if (gui.second->GetName().compare(L"ChatBox") == 0)
			gui.second->OnKeyBoardPressed(keyCode);
	}*/
}

void GUIManager::OnKeyBoardHeld(SDL_Scancode keyCode)
{
	for (auto& gui : _guis)
	{
		if (gui.second->GetName().compare(L"ChatBox") == 0)
			gui.second->OnKeyBoardHeld(keyCode);

		if (gui.second->GetName().compare(L"LoginForm") == 0)
			gui.second->OnKeyBoardHeld(keyCode);
	}
}

GUIScreen* GUIManager::FindFocusedGui(const Vector2Int& position)
{
	GUIScreen* focusedGui = nullptr;
	for (auto& gui : _guis)
	{
		if (gui.second->GetState() == GUIState::DeActive)
			continue;

		if (gui.second->IsInRange(position))
			focusedGui = gui.second;
	}

	return focusedGui;
}

void GUIManager::RenderingSortGuis()
{
	if (_guis.size() == 0)
		return;

	if (_focusedGui)
	{
		_guis.erase(_focusedGui->GetId());
		_guis.insert(std::make_pair(_focusedGui->GetId(), _focusedGui));
	}

}

GUIScreen* GUIManager::GetGui(int32 id)
{
	auto iter = _guis.find(id);
	if (iter != _guis.end())
		return iter->second;

	return nullptr;
}

GUIScreen* GUIManager::GetGui(const std::wstring& name)
{
	auto iter = _nameGuisTable.find(name);
	if (iter != _nameGuisTable.end())
		return iter->second;

	return nullptr;
}
