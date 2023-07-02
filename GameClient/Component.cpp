#include "pch.h"
#include "Component.h"

const std::size_t Component::ComponentType = std::hash< std::string >()(_STRING2(DerivedComponent));

Component::Component(int32 updateOrder)
	: _updateOrder(updateOrder)
{

}

Component::~Component()
{

}

void Component::Update(float deltaTime)
{

}

void Component::ProcessInput(const struct InputState& state)
{

}
