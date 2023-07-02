#include "pch.h"
#include "Creature.h"
#include "ActorManager.h"
#include "InputManager.h"
#include "AssetManager.h"

#include "BoxComponent.h"
#include "FSMComponent.h"
#include "GUIComponent.h"
#include "MoveComponent.h"
#include "StatComponent.h"
#include "TransformComponent.h"
#include "AnimSpriteComponent.h"

#include "HealthBar.h"

Creature::Creature()
{

}

Creature::~Creature()
{

}

void Creature::Initialize()
{
	auto transform = AddComponent<TransformComponent>();
	auto move = AddComponent<MoveComponent>();
	auto stat = AddComponent<StatComponent>();
	auto box = AddComponent<BoxComponent>();
	auto gui = AddComponent<GUIComponent>();
	auto sprite = AddComponent<AnimSpriteComponent>();
	auto fsm = AddComponent<FSMComponent>();

	float range = 16.f;
	box->SetRange(range);

	gui->AddGui<HealthBar>();

}

void Creature::SpecificActorUpdate(float deltaTime)
{

}

void Creature::OnDamaged()
{

}
