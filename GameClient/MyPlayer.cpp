#include "pch.h"
#include "MyPlayer.h"
#include "Shadow.h"
#include "ActorManager.h"
#include "AssetManager.h"

#include "FSMComponent.h"
#include "AnimSpriteComponent.h"
#include "PlayerInputComponent.h"

#include "AIIdle.h"
#include "AIMove.h"
#include "AISkill.h"
#include "AIDeath.h"

MyPlayer::MyPlayer()
{

}

MyPlayer::~MyPlayer()
{
	std::wcout << "~MyPlayer" << std::endl;
}

void MyPlayer::Initialize()
{
	Creature::Initialize();

	auto input = AddComponent<PlayerInputComponent>();
	auto fsm = GetComponent<FSMComponent>();

	auto idleState = fsm->AddState<AIIdle>();
	auto moveState = fsm->AddState<AIMove>();
	auto autoAttackSkillState = fsm->AddState<AIAutoAttackSkill>();
	auto ballAttackSkillState = fsm->AddState<AIBallAttackSkill>();
	auto deathState = fsm->AddState<AIDeath>();

	idleState->Initialize();
	moveState->Initialize();
	autoAttackSkillState->Initialize();
	ballAttackSkillState->Initialize();
	deathState->Initialize();

	auto shadow = GActorManager->AddEffect<Shadow>();
	shadow->SetOwner(shared_from_this());
	shadow->GetComponent<AnimSpriteComponent>()->Play(L"ACTOR_SHADOW_EFFECT");
}

void MyPlayer::SpecificActorUpdate(float deltaTime)
{

}

void MyPlayer::OnDamaged()
{
}

