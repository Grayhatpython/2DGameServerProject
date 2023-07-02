#include "Actor.h"
#include "pch.h"
#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include "ActorManager.h"

Actor::Actor()
{

}

Actor::~Actor()
{
	if (_type == Protocol::ActorType::NONE)
	{
		for (auto component : _staticComponents)
			delete component;
	}
	else
	{
		for (auto component : _components)
			cdelete(component);
	}

	_staticComponents.clear();
	_components.clear();
}

void Actor::Initialize()
{

}

void Actor::Update(float deltaTime)
{
	if (_state == ActorState::Active)
	{
		UpdateComponents(deltaTime);
		SpecificActorUpdate(deltaTime);
	}
}

void Actor::UpdateComponents(float deltaTime)
{
	if (_type == Protocol::ActorType::NONE)
	{
		for (auto& component : _staticComponents)
			component->Update(deltaTime);
	}
	else
	{
		for (auto& component : _components)
			component->Update(deltaTime);
	}
}

void Actor::SpecificActorUpdate(float deltaTime)
{

}

void Actor::ProcessInput(const struct InputState& state)
{
	if (_state == ActorState::Active)
	{
		if (_type == Protocol::ActorType::NONE)
		{
			for (auto& component : _staticComponents)
				component->ProcessInput(state);
		}
		else
		{
			for (auto& component : _components)
				component->ProcessInput(state);
		}

		SpecificProcessInput(state);
	}
}

void Actor::SpecificProcessInput(const struct InputState& state)
{

}

void Actor::SetState(ActorState state)
{
	_state = state;

	//	NONE -> 사라질일이 없는 Actor들... 아직은
	if (state == ActorState::Dead && _type != Protocol::ActorType::NONE)
	{
		if (_type == Protocol::ActorType::EFFECT)
			GActorManager->AddRemoveEffectId(_id);
		else
			GActorManager->AddDeatActorId(_id);
	}
}
