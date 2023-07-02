#include "pch.h"
#include "Hit.h"
#include "Texture.h"
#include "ActorManager.h"
#include "AssetManager.h"
#include "TransformComponent.h"
#include "AnimSpriteComponent.h"

Hit::Hit()
{

}

Hit::~Hit()
{
	
}

void Hit::Initialize()
{
	Effect::Initialize();

	SetName(L"Hit");

	auto transform = AddComponent<TransformComponent>();
	auto sprite = AddComponent<AnimSpriteComponent>();
	sprite->SetDrawOrder(90);
}


void Hit::SpecificActorUpdate(float deltaTime)
{
	Effect::SpecificActorUpdate(deltaTime);
}
