#include "pch.h"
#include "Monster.h"

#include "FSMComponent.h"
#include "AIIdle.h"
#include "AIMove.h"
#include "AIDeath.h"
#include "AISkill.h"

Monster::Monster()
{

}

Monster::~Monster()
{

}

void Monster::Initialize()
{
	Creature::Initialize();

	auto fsm = GetComponent<FSMComponent>();

	auto idleState = fsm->AddState<AIMonsterIdle>();
	auto moveState = fsm->AddState<AIMonsterMove>();
	auto autoAttackSkillState = fsm->AddState<AIMonsterAutoAttackSkill>();
	auto deathState = fsm->AddState<AIDeath>();

	idleState->Initialize();
	moveState->Initialize();
	autoAttackSkillState->Initialize();
	deathState->Initialize();

}

void Monster::SpecificActorUpdate(float deltaTime)
{
	Actor::SpecificActorUpdate(deltaTime);
}

void Monster::OnDamaged()
{

}

