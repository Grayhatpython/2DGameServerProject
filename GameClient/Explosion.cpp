#include "pch.h"
#include "Explosion.h"
#include "ActorManager.h"
#include "AssetManager.h"
#include "TransformComponent.h"
#include "AnimSpriteComponent.h"

Explosion::Explosion()
{

}

Explosion::~Explosion()
{
	std::wcout << "~Explosion" << std::endl;
}

void Explosion::Initialize()
{
	Effect::Initialize();

	SetName(L"Explosion");
	_liveTime = 0.7f;

	auto transform = AddComponent<TransformComponent>();
	auto sprite = AddComponent<AnimSpriteComponent>();
	
	sprite->SetAnimFPS(10.f);
	sprite->SetDrawOrder(120);
}


void Explosion::SpecificActorUpdate(float deltaTime)
{
	Effect::SpecificActorUpdate(deltaTime);
}
