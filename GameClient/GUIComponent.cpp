#include "pch.h"
#include "GUIComponent.h"

COMPONENT_TYPE_DEFINITION(Component, GUIComponent);

GUIComponent::GUIComponent()
{

}

GUIComponent::~GUIComponent()
{
	for (auto& gui : _guis)
		gui.second->SetState(GUIState::Remove);

	_guis.clear();
}

