#include "pch.h"
#include "AIMove.h"
#include "FSMComponent.h"
#include "MoveComponent.h"
#include "TransformComponent.h"
#include "AnimSpriteComponent.h"
#include "PlayerInputComponent.h"
#include "Actor.h"
#include "Monster.h"
#include "Creature.h"
#include "BallDestory.h"
#include "MapManager.h"
#include "ActorManager.h"
#include "AssetManager.h"
#include "BoxComponent.h"
#include "CollisionManager.h"

#include "Protocol.pb.h"
#include "NetworkManager.h"
#include "ServerPacketHandler.h"
#include "NetworkManager.h"

AIMove::AIMove()
	: AIState(L"Move")
{
}

AIMove::~AIMove()
{
}

void AIMove::Initialize()
{

}

void AIMove::Update(float deltaTime)
{
	//	TEMP
	UpdatePosition(deltaTime);
}

void AIMove::UpdateAnimation()
{
	auto fsm = GetOwner();
	ASSERT(fsm);

	if (fsm->GetCurrentStateName().compare(L"Move") != 0)
		return;

	auto owner = fsm->GetOwner();
	ASSERT(owner);

	auto sprite = owner->GetComponent<AnimSpriteComponent>();
	ASSERT(sprite);
	auto move = owner->GetComponent<MoveComponent>();
	ASSERT(move);

	auto moveDir = move->GetMoveDir();

	std::wstring animName;
	switch (moveDir)
	{
	case Protocol::MoveDir::UP:
		animName = L"PLAYER_MOVE_UP";
		break;
	case Protocol::MoveDir::UP_RIGHT:
		animName = L"PLAYER_MOVE_UP_RIGHT";
		break;
	case Protocol::MoveDir::RIGHT:
		animName = L"PLAYER_MOVE_RIGHT";
		break;
	case Protocol::MoveDir::DOWN_RIGHT:
		animName = L"PLAYER_MOVE_DOWN_RIGHT";
		break;
	case Protocol::MoveDir::DOWN:
		animName = L"PLAYER_MOVE_DOWN";
		break;
	case Protocol::MoveDir::DOWN_LEFT:
		animName = L"PLAYER_MOVE_DOWN_LEFT";
		break;
	case Protocol::MoveDir::LEFT:
		animName = L"PLAYER_MOVE_LEFT";
		break;
	case Protocol::MoveDir::UP_LEFT:
		animName = L"PLAYER_MOVE_UP_LEFT";
		break;
	}

	sprite->Play(animName);
}

void AIMove::OnEnter()
{
	UpdateAnimation();
}

void AIMove::OnExit()
{

}

void AIMove::UpdatePosition(float deltaTime)
{
	auto fsm = GetOwner();
	ASSERT(fsm);

	if (fsm->GetCurrentStateName().compare(L"Move") != 0)
		return;

	auto owner = fsm->GetOwner();
	ASSERT(owner);

	auto move = owner->GetComponent<MoveComponent>();
	ASSERT(move);
	auto transform = owner->GetComponent<TransformComponent>();
	ASSERT(transform);
	auto input = owner->GetComponent<PlayerInputComponent>();

	auto movePosition = move->GetMovePosition();
	auto forwardSpeed = move->GetForwardSpeed();

	Vector2 currentPosition = transform->GetPosition();
	Vector2 diff = movePosition - currentPosition;

	if (Math::NearZero(diff.Length(), forwardSpeed * deltaTime))
	{
		transform->SetPosition(movePosition);
		UpdateMovePosition();
	}
	else
	{
		currentPosition += Vector2::Normalize(diff) * forwardSpeed * deltaTime;
		/*OutputDebugString(std::to_wstring(count).c_str());
		OutputDebugString(L"\n");*/
		transform->SetPosition(currentPosition);
	}
}

void AIMove::UpdateMovePosition()
{
	auto fsm = GetOwner();
	ASSERT(fsm);
	if (fsm->GetCurrentStateName().compare(L"Move") != 0)
		return;

	auto owner = fsm->GetOwner();
	ASSERT(owner);

	auto input = owner->GetComponent<PlayerInputComponent>();

	//	MyPlayer
	if (input)
	{
		auto move = owner->GetComponent<MoveComponent>();
		auto transform = owner->GetComponent<TransformComponent>();

		bool isMoveKeyPressed = input->GetIsMoveKeyPressed();

		if (isMoveKeyPressed == false)
		{
			Protocol::C_CHANGE_STATE changeStateSendPacket;
			changeStateSendPacket.set_state(Protocol::AIState::IDLE);
			auto sendBuffer = ServerPacketHandler::MakeSendBuffer(changeStateSendPacket);
			GNetworkManager->Send(sendBuffer);
		}
		else
		{
			Protocol::MoveDir movekeyInputDir;
			/*
			if (input->GetIsMoveUpLeftKeyPressed())
				movekeyInputDir = Protocol::MoveDir::UP_LEFT;
			else if (input->GetIsMoveUpRightKeyPressed())
				movekeyInputDir = Protocol::MoveDir::UP_RIGHT;
			else if (input->GetIsMoveDownLeftKeyPressed())
				movekeyInputDir = Protocol::MoveDir::DOWN_LEFT;
			else if (input->GetIsMoveDownRightKeyPressed())
				movekeyInputDir = Protocol::MoveDir::DOWN_RIGHT;
			*/
			if (input->GetIsMoveUpKeyPressed())
				movekeyInputDir = Protocol::MoveDir::UP;
			else if (input->GetIsMoveDownKeyPressed())
				movekeyInputDir = Protocol::MoveDir::DOWN;
			else if (input->GetIsMoveLeftKeyPressed())
				movekeyInputDir = Protocol::MoveDir::LEFT;
			else
				movekeyInputDir = Protocol::MoveDir::RIGHT;
			auto movePosition = transform->GetPosition();
			movePosition += move->GetForwardPositionFromMoveDir(movekeyInputDir);

			if (GMapManager->CanMovePosition(movePosition) && GActorManager->FindActor(movePosition) == nullptr)
			{
				move->SetMovePosition(movePosition);
				move->SetMoveDir(movekeyInputDir);
				Protocol::PositionInfo positionInfo;
				positionInfo.set_movedir(movekeyInputDir);
				positionInfo.set_positionx(movePosition.x);
				positionInfo.set_positiony(movePosition.y);
				positionInfo.set_state(Protocol::AIState::MOVE);
				positionInfo.set_usedskillid(0);	//	TODO

				Protocol::C_MOVE movePacket;
				auto posInfo = movePacket.mutable_positioninfo();
				posInfo->MergeFrom(positionInfo);
				auto sendBuffer = ServerPacketHandler::MakeSendBuffer(movePacket);
				GNetworkManager->Send(sendBuffer);
			}
		}
	}
}

AIMonsterMove::AIMonsterMove()
	: AIState(L"Move")
{
}

AIMonsterMove::~AIMonsterMove()
{
}

void AIMonsterMove::Initialize()
{

}

void AIMonsterMove::Update(float deltaTime)
{
	UpdatePosition(deltaTime);
}

void AIMonsterMove::UpdateAnimation()
{
	auto fsm = GetOwner();
	ASSERT(fsm);

	if (fsm->GetCurrentStateName().compare(L"Move") != 0)
		return;

	auto owner = fsm->GetOwner();
	ASSERT(owner);

	auto sprite = owner->GetComponent<AnimSpriteComponent>();
	ASSERT(sprite);
	auto move = owner->GetComponent<MoveComponent>();
	ASSERT(move);

	auto moveDir = move->GetMoveDir();

	std::wstring animName;
	switch (moveDir)
	{
	case Protocol::MoveDir::UP:
		animName = L"MONSTER_MOVE_UP";
		break;
	case Protocol::MoveDir::UP_RIGHT:
		animName = L"MONSTER_MOVE_UP_RIGHT";
		break;
	case Protocol::MoveDir::RIGHT:
		animName = L"MONSTER_MOVE_RIGHT";
		break;
	case Protocol::MoveDir::DOWN_RIGHT:
		animName = L"MONSTER_MOVE_DOWN_RIGHT";
		break;
	case Protocol::MoveDir::DOWN:
		animName = L"MONSTER_MOVE_DOWN";
		break;
	case Protocol::MoveDir::DOWN_LEFT:
		animName = L"MONSTER_MOVE_DOWN_LEFT";
		break;
	case Protocol::MoveDir::LEFT:
		animName = L"MONSTER_MOVE_LEFT";
		break;
	case Protocol::MoveDir::UP_LEFT:
		animName = L"MONSTER_MOVE_UP_LEFT";
		break;
	}

	sprite->Play(animName, false);
}

void AIMonsterMove::OnEnter()
{
	UpdateAnimation();
}

void AIMonsterMove::OnExit()
{
}

void AIMonsterMove::UpdatePosition(float deltaTime)
{
	auto fsm = GetOwner();
	ASSERT(fsm);

	if (fsm->GetCurrentStateName().compare(L"Move") != 0)
		return;

	auto owner = fsm->GetOwner();
	ASSERT(owner);

	auto move = owner->GetComponent<MoveComponent>();
	ASSERT(move);
	auto transform = owner->GetComponent<TransformComponent>();
	ASSERT(transform);

	float speed = 0.f;
	auto movePosition = move->GetMovePosition();
	Vector2 currentPosition = transform->GetPosition();
	Vector2 diff = movePosition - currentPosition;

	if (move->IsDiagonalDirectionFromDirectionVector(diff))
		speed = move->GetDiagonalSpeed();
	else
		speed = move->GetForwardSpeed();

	if (Math::NearZero(diff.Length(), speed * deltaTime))
	{
		transform->SetPosition(movePosition);
	}
	else
	{
		currentPosition += Vector2::Normalize(diff) * speed * deltaTime;
		transform->SetPosition(currentPosition);
	}
}


AIProjectileMove::AIProjectileMove()
	:	AIState(L"Move")
{

}

AIProjectileMove::~AIProjectileMove()
{

}

void AIProjectileMove::Initialize()
{

}

void AIProjectileMove::Update(float deltaTime)
{
	UpdatePosition(deltaTime);
}

void AIProjectileMove::UpdateAnimation()
{
	auto fsm = GetOwner();
	ASSERT(fsm);

	if (fsm->GetCurrentStateName().compare(L"Move") != 0)
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
	float angle = 0.f;
	switch (moveDir)
	{
	case Protocol::MoveDir::UP:
		break;
	case Protocol::MoveDir::UP_RIGHT:
		angle = -Math::PiOver2 / 2.f;
		break;
	case Protocol::MoveDir::RIGHT:
		angle = -Math::PiOver2;
		break;
	case Protocol::MoveDir::DOWN_RIGHT:
		angle = -(Math::PiOver2 + Math::PiOver2 / 2.f);
		break;
	case Protocol::MoveDir::DOWN:
		angle = -Math::Pi;
		break;
	case Protocol::MoveDir::DOWN_LEFT:
		angle = Math::PiOver2 + Math::PiOver2 / 2.f;
		break;
	case Protocol::MoveDir::LEFT:
		angle = Math::PiOver2;
		break;
	case Protocol::MoveDir::UP_LEFT:
		angle = Math::PiOver2 / 2.f;
		break;
	}

	transform->SetRotation(angle);
	sprite->Play(L"FIREBALL_MOVE");
}

void AIProjectileMove::OnEnter()
{
	UpdateAnimation();
}

void AIProjectileMove::OnExit()
{

}

void AIProjectileMove::UpdatePosition(float deltaTime)
{
	auto fsm = GetOwner();
	ASSERT(fsm);

	if (fsm->GetCurrentStateName().compare(L"Move") != 0)
		return;

	auto owner = fsm->GetOwner();
	ASSERT(owner);

	auto move = owner->GetComponent<MoveComponent>();
	ASSERT(move);
	auto transform = owner->GetComponent<TransformComponent>();
	ASSERT(transform);
	auto box = owner->GetComponent<BoxComponent>();
	ASSERT(box);

	float speed = 0.f;
	auto movePosition = move->GetMovePosition();
	Vector2 currentPosition = transform->GetPosition();
	Vector2 diff = movePosition - currentPosition;

	if (move->IsDiagonalDirectionFromDirectionVector(diff))
		speed = move->GetDiagonalSpeed();
	else
		speed = move->GetForwardSpeed();

	if (Math::NearZero(diff.Length(), speed * deltaTime))
	{
		transform->SetPosition(movePosition);
	}
	else
	{
		currentPosition += Vector2::Normalize(diff) * speed * deltaTime;
		transform->SetPosition(currentPosition);
	}
}

