#include "pch.h"
#include "Sweep.h"
#include "ActorManager.h"
#include "AssetManager.h"
#include "TransformComponent.h"
#include "AnimSpriteComponent.h"

Sweep::Sweep()
{

}

Sweep::~Sweep()
{

}

void Sweep::Initialize()
{
	Effect::Initialize();
	SetName(L"Sweep");

	AddComponent<TransformComponent>();
	auto sweepSprite = AddComponent<AnimSpriteComponent>();
	sweepSprite->SetDrawOrder(110);
}

void Sweep::SpecificActorUpdate(float deltaTime)
{
	Effect::SpecificActorUpdate(deltaTime);
}
