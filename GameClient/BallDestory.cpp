#include "pch.h"
#include "BallDestory.h"
#include "ActorManager.h"
#include "AssetManager.h"
#include "TransformComponent.h"
#include "AnimSpriteComponent.h"

BallDestory::BallDestory()
{
}

BallDestory::~BallDestory()
{
}

void BallDestory::Initialize()
{
	Effect::Initialize();

	SetName(L"BallDestory");

	auto transform = AddComponent<TransformComponent>();
	auto sprite = AddComponent<AnimSpriteComponent>();
	sprite->SetDrawOrder(110);
}

void BallDestory::SpecificActorUpdate(float deltaTime)
{
	Effect::SpecificActorUpdate(deltaTime);
}
