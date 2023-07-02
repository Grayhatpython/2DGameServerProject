#include "pch.h"
#include "Ball.h"
#include "AssetManager.h"
#include "AIMove.h"
#include "AIDeath.h"

#include "FSMComponent.h"
#include "BoxComponent.h"
#include "MoveComponent.h"
#include "StatComponent.h"
#include "TransformComponent.h"
#include "AnimSpriteComponent.h"

Ball::Ball()
{

}

Ball::~Ball()
{

}

void Ball::Initialize()
{
	SetName(L"Ball");

	auto transform = AddComponent<TransformComponent>();
	auto move = AddComponent<MoveComponent>();
	auto stat = AddComponent<StatComponent>();
	auto sprite = AddComponent<AnimSpriteComponent>();

	auto box = AddComponent<BoxComponent>();
	float range = 12.f;
	box->SetRange(range);

	auto fsm = AddComponent<FSMComponent>();

	auto moveState = fsm->AddState<AIProjectileMove>();
	moveState->Initialize();
}

void Ball::SpecificActorUpdate(float deltaTime)
{
	Projectile::SpecificActorUpdate(deltaTime);
}
