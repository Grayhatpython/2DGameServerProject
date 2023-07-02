#include "pch.h"
#include "AISkill.h"
#include "FSMComponent.h"
#include "MoveComponent.h"
#include "TransformComponent.h"
#include "AnimSpriteComponent.h"
#include "PlayerInputComponent.h"

//	TEMP
#include "Ball.h"
#include "Sweep.h"
#include "Creature.h"
#include "AssetManager.h"
#include "ActorManager.h"
#include "RenderManager.h"
#include "DataManager.h"

#include "Protocol.pb.h"
#include "NetworkManager.h"
#include "ServerPacketHandler.h"
#include "NetworkManager.h"

AIAutoAttackSkill::AIAutoAttackSkill()
	: AIState(DataManager::FindSkillData(Protocol::SkillType::SKILL_AUTO_ATTACK).name)
{

}

AIAutoAttackSkill::~AIAutoAttackSkill()
{

}

void AIAutoAttackSkill::Initialize()
{
	_coolDown = DataManager::FindSkillData(Protocol::SkillType::SKILL_AUTO_ATTACK).coolDown;
}

void AIAutoAttackSkill::Update(float deltaTime)
{
	_coolDown -= deltaTime;
	if (_coolDown <= 0.f)
	{

		auto fsm = GetOwner();
		ASSERT(fsm);
		auto owner = fsm->GetOwner();
		ASSERT(owner);
		auto sprite = owner->GetComponent<AnimSpriteComponent>();
		ASSERT(sprite);
		auto move = owner->GetComponent<MoveComponent>();
		ASSERT(move);
		auto transform = owner->GetComponent<TransformComponent>();
		ASSERT(transform);
		auto input = owner->GetComponent<PlayerInputComponent>();
		
		sprite->Reset();

		_coolDown = DataManager::FindSkillData(Protocol::SkillType::SKILL_AUTO_ATTACK).coolDown;
		//	TEMP
		{
			//	꼭 내가 움직이는 플레이어만 패킷 관련해서 컨트롤 한다.
			if (input)
			{
				//	클라에서도 상태 변경 패킷 보낸다
				//	서버에서도 로직 처리하고 상태 변경 시 패킷 보내는 데 서버 늦게오면 서버쪽 정보로 보정된다
				Protocol::C_CHANGE_STATE changeStateSendPacket;
				changeStateSendPacket.set_state(Protocol::AIState::IDLE);
				auto sendBuffer = ServerPacketHandler::MakeSendBuffer(changeStateSendPacket);
				GNetworkManager->Send(sendBuffer);
			}
			//	나머지 플레이어는 서버에서만 판별한다.
			//	미리 클라에서 상태 변경 후에 서버에서 패킷 오는 걸로 그 정보를 토대로 확인해서 보정된다.
			else
				//fsm->ChangeState(Protocol::AIState::IDLE);
				;
		}
	}
}

void AIAutoAttackSkill::UpdateAnimation()
{
	auto fsm = GetOwner();
	ASSERT(fsm);

	if (fsm->GetCurrentStateName().compare(DataManager::FindSkillData(Protocol::SkillType::SKILL_AUTO_ATTACK).name) != 0)
		return;

	auto owner = fsm->GetOwner();
	ASSERT(owner);

	auto sprite = owner->GetComponent<AnimSpriteComponent>();
	ASSERT(sprite);
	auto move = owner->GetComponent<MoveComponent>();
	ASSERT(move);
	auto transform = owner->GetComponent<TransformComponent>();
	ASSERT(transform);
	auto moveDir = move->GetMoveDir();

	auto sweep = GActorManager->AddEffect<Sweep>();

	auto sweepSprite = sweep->GetComponent<AnimSpriteComponent>();
	ASSERT(sweepSprite);
	auto sweepTransform = sweep->GetComponent<TransformComponent>();
	ASSERT(sweepTransform);

	Vector2 position = transform->GetPosition();
	float angle = 0.f;

	std::wstring animName;
	switch (moveDir)
	{
	case Protocol::MoveDir::UP:
		animName = L"PLAYER_AUTO_ATTACK_UP";
		position.y -= 8.f;
		break;
	case Protocol::MoveDir::UP_RIGHT:
		animName = L"PLAYER_AUTO_ATTACK_RIGHT";
		angle = -Math::PiOver2 / 2.f;
		position.x += 8.f;
		position.y -= 8.f;
		break;
	case Protocol::MoveDir::RIGHT:
		animName = L"PLAYER_AUTO_ATTACK_RIGHT";
		angle = -Math::PiOver2;
		position.x += 8.f;
		break;
	case Protocol::MoveDir::DOWN_RIGHT:
		animName = L"PLAYER_AUTO_ATTACK_DOWN_RIGHT";
		angle = -(Math::PiOver2 + Math::PiOver2 / 2.f);
		position.x += 8.f;
		position.y += 8.f;
		break;
	case Protocol::MoveDir::DOWN:
		animName = L"PLAYER_AUTO_ATTACK_DOWN";
		angle = -Math::Pi;
		position.y += 8.f;
		break;
	case Protocol::MoveDir::DOWN_LEFT:
		animName = L"PLAYER_AUTO_ATTACK_DOWN_LEFT";
		angle = Math::PiOver2 + Math::PiOver2 / 2.f;
		position.x -= 8.f;
		position.y += 8.f;
		break;
	case Protocol::MoveDir::LEFT:
		animName = L"PLAYER_AUTO_ATTACK_LEFT";
		angle = Math::PiOver2;
		position.x -= 8.f;
		break;
	case Protocol::MoveDir::UP_LEFT:
		animName = L"PLAYER_AUTO_ATTACK_LEFT";
		angle = Math::PiOver2 / 2.f;
		position.x -= 8.f;
		position.y -= 8.f;
		break;
	}

	sprite->Play(animName);

	sweepSprite->Play(L"PLAYER_AUTO_ATTACK_SWEEP_EFFECT");
	sweepTransform->SetPosition(position);
	sweepTransform->SetRotation(angle);
}

void AIAutoAttackSkill::OnEnter()
{
	UpdateAnimation();
}

void AIAutoAttackSkill::OnExit()
{
}

AIBallAttackSkill::AIBallAttackSkill()
	:	AIState(DataManager::FindSkillData(Protocol::SkillType::SKILL_PROJECTILE_ATTACK).name)
{

}

AIBallAttackSkill::~AIBallAttackSkill()
{

}

void AIBallAttackSkill::Initialize()
{
	_coolDown = DataManager::FindSkillData(Protocol::SkillType::SKILL_PROJECTILE_ATTACK).coolDown;
}

void AIBallAttackSkill::Update(float deltaTime)
{
	_coolDown -= deltaTime;
	if (_coolDown <= 0.f)
	{

		auto fsm = GetOwner();
		ASSERT(fsm);
		auto owner = fsm->GetOwner();
		ASSERT(owner);
		auto sprite = owner->GetComponent<AnimSpriteComponent>();
		ASSERT(sprite);
		auto move = owner->GetComponent<MoveComponent>();
		ASSERT(move);
		auto transform = owner->GetComponent<TransformComponent>();
		ASSERT(transform);
		auto input = owner->GetComponent<PlayerInputComponent>();

		sprite->Reset();

		_coolDown = DataManager::FindSkillData(Protocol::SkillType::SKILL_PROJECTILE_ATTACK).coolDown;
		//	TEMP
		{
			//	꼭 내가 움직이는 플레이어만 패킷 관련해서 컨트롤 한다.
			if (input)
			{
				//	클라에서도 상태 변경 패킷 보낸다
				//	서버에서도 로직 처리하고 상태 변경 시 패킷 보내는 데 서버 늦게오면 서버쪽 정보로 보정된다
				Protocol::C_CHANGE_STATE changeStateSendPacket;
				changeStateSendPacket.set_state(Protocol::AIState::IDLE);
				auto sendBuffer = ServerPacketHandler::MakeSendBuffer(changeStateSendPacket);
				GNetworkManager->Send(sendBuffer);
			}
			//	나머지 플레이어는 서버에서만 판별한다.
			//	미리 클라에서 상태 변경 후에 서버에서 패킷 오는 걸로 그 정보를 토대로 확인해서 보정된다.
			else
				//fsm->ChangeState(Protocol::AIState::IDLE);
				;
		}
	}

}

void AIBallAttackSkill::UpdateAnimation()
{
	auto fsm = GetOwner();
	ASSERT(fsm);

	if (fsm->GetCurrentStateName().compare(DataManager::FindSkillData(Protocol::SkillType::SKILL_PROJECTILE_ATTACK).name) != 0)
		return;

	auto owner = fsm->GetOwner();
	ASSERT(owner);

	auto sprite = owner->GetComponent<AnimSpriteComponent>();
	ASSERT(sprite);
	auto move = owner->GetComponent<MoveComponent>();
	ASSERT(move);
	auto transform = owner->GetComponent<TransformComponent>();
	ASSERT(transform);
	auto moveDir = move->GetMoveDir();

	std::wstring animName;
	switch (moveDir)
	{
	case Protocol::MoveDir::UP:
		animName = L"PLAYER_BALL_ATTACK_UP";
		break;
	case Protocol::MoveDir::UP_RIGHT:
		animName = L"PLAYER_BALL_ATTACK_UP_RIGHT";
		break;
	case Protocol::MoveDir::RIGHT:
		animName = L"PLAYER_BALL_ATTACK_RIGHT";
		break;
	case Protocol::MoveDir::DOWN_RIGHT:
		animName = L"PLAYER_BALL_ATTACK_DOWN_RIGHT";
		break;
	case Protocol::MoveDir::DOWN:
		animName = L"PLAYER_BALL_ATTACK_DOWN";
		break;
	case Protocol::MoveDir::DOWN_LEFT:
		animName = L"PLAYER_BALL_ATTACK_DOWN_LEFT";
		break;
	case Protocol::MoveDir::LEFT:
		animName = L"PLAYER_BALL_ATTACK_LEFT";
		break;
	case Protocol::MoveDir::UP_LEFT:
		animName = L"PLAYER_BALL_ATTACK_UP_LEFT";
		break;
	}

	sprite->Play(animName);
}

void AIBallAttackSkill::OnEnter()
{
	UpdateAnimation();
}

void AIBallAttackSkill::OnExit()
{

}

AIMonsterAutoAttackSkill::AIMonsterAutoAttackSkill()
	: AIState(DataManager::FindSkillData(Protocol::SkillType::SKILL_AUTO_ATTACK).name)
{
}

AIMonsterAutoAttackSkill::~AIMonsterAutoAttackSkill()
{

}

void AIMonsterAutoAttackSkill::Initialize()
{

}

void AIMonsterAutoAttackSkill::Update(float deltaTime)
{

}

void AIMonsterAutoAttackSkill::UpdateAnimation()
{
	auto fsm = GetOwner();
	ASSERT(fsm);

	if (fsm->GetCurrentStateName().compare(DataManager::FindSkillData(Protocol::SkillType::SKILL_AUTO_ATTACK).name) != 0)
		return;

	auto owner = fsm->GetOwner();
	ASSERT(owner);

	auto sprite = owner->GetComponent<AnimSpriteComponent>();
	ASSERT(sprite);
	auto move = owner->GetComponent<MoveComponent>();
	ASSERT(move);
	auto transform = owner->GetComponent<TransformComponent>();
	ASSERT(transform);
	auto moveDir = move->GetMoveDir();

	std::wstring animName;
	switch (moveDir)
	{
	case Protocol::MoveDir::UP:
		animName = L"MONSTER_AUTO_ATTACK_UP";
		break;
	case Protocol::MoveDir::UP_RIGHT:
		animName = L"MONSTER_AUTO_ATTACK_UP_RIGHT";
		break;
	case Protocol::MoveDir::RIGHT:
		animName = L"MONSTER_AUTO_ATTACK_RIGHT";
		break;
	case Protocol::MoveDir::DOWN_RIGHT:
		animName = L"MONSTER_AUTO_ATTACK_DOWN_RIGHT";
		break;
	case Protocol::MoveDir::DOWN:
		animName = L"MONSTER_AUTO_ATTACK_DOWN";
		break;
	case Protocol::MoveDir::DOWN_LEFT:
		animName = L"MONSTER_AUTO_ATTACK_DOWN_LEFT";
		break;
	case Protocol::MoveDir::LEFT:
		animName = L"MONSTER_AUTO_ATTACK_LEFT";
		break;
	case Protocol::MoveDir::UP_LEFT:
		animName = L"MONSTER_AUTO_ATTACK_UP_LEFT";
		break;
	}

	sprite->Play(animName);
}

void AIMonsterAutoAttackSkill::OnEnter()
{
	UpdateAnimation();
}

void AIMonsterAutoAttackSkill::OnExit()
{

}
