#include "pch.h"
#include "Player.h"
#include "AssetManager.h"

#include "FSMComponent.h"
#include "AnimSpriteComponent.h"

#include "AIIdle.h"
#include "AIMove.h"
#include "AISkill.h"
#include "AIDeath.h"

Player::Player()
{

}

Player::~Player()
{
	std::wcout << "~Player" << std::endl;
}

void Player::Initialize()
{
	Creature::Initialize();

	{
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
	}
}

void Player::SpecificActorUpdate(float deltaTime)
{

}

void Player::OnDamaged()
{
}

