#include "pch.h"
#include "AIDeath.h"
#include "Actor.h"
#include "Explosion.h"
#include "FSMComponent.h"
#include "TransformComponent.h"
#include "AnimSpriteComponent.h"

#include "ActorManager.h"
#include "AnimationManager.h"

AIDeath::AIDeath()
	: AIState(L"Death")
{

}

AIDeath::~AIDeath()
{
	
}

void AIDeath::Initialize()
{

}

void AIDeath::Update(float deltaTime)
{

}

void AIDeath::UpdateAnimation()
{

}

void AIDeath::OnEnter()
{
	auto fsm = GetOwner();
	ASSERT(fsm);

	if (fsm->GetCurrentStateName().compare(L"Death") != 0)
		return;

	auto owner = fsm->GetOwner();
	ASSERT(owner);

	auto transform = owner->GetComponent<TransformComponent>();
	ASSERT(transform);

	auto explosion = GActorManager->AddEffect<Explosion>();

	auto explosionTransform = explosion->GetComponent<TransformComponent>();
	ASSERT(explosionTransform);

	auto position = transform->GetPosition();
	explosionTransform->SetPosition(position);

	auto explositionSprite = explosion->GetComponent<AnimSpriteComponent>();
	ASSERT(explositionSprite);

	explositionSprite->Play(L"ACTOR_EXPLOSION_EFFECT");
}

void AIDeath::OnExit()
{

}
