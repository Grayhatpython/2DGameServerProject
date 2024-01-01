#include "pch.h"
#include "AIIdle.h"
#include "FSMComponent.h"
#include "MoveComponent.h"
#include "TransformComponent.h"
#include "PlayerInputComponent.h"
#include "AnimSpriteComponent.h"
#include "Actor.h"
#include "Ball.h"
#include "MapManager.h"
#include "AssetManager.h"
#include "ActorManager.h"
#include "AnimationManager.h"

#include "NetworkManager.h"
#include "Protocol.pb.h"

AIIdle::AIIdle()
	: AIState(L"Idle")
{
	
}

AIIdle::~AIIdle()
{

}

void AIIdle::Initialize()
{

}

void AIIdle::Update(float deltaTime)
{
	auto fsm = GetOwner();

	if (fsm->GetCurrentStateName().compare(L"Idle") != 0)
		return;

	ASSERT(fsm);
	auto owner = fsm->GetOwner();
	ASSERT(owner);

	//	MyPlayer
	auto input = owner->GetComponent<PlayerInputComponent>();
	if (input)
	{
		auto move = owner->GetComponent<MoveComponent>();
		ASSERT(move);
		auto transform = owner->GetComponent<TransformComponent>();
		ASSERT(transform);

		bool isMoveKeyPressed = input->GetIsMoveKeyPressed();

		if (isMoveKeyPressed == false)
		{
			if (_isSkillKeyDown == false && input->GetIsSkillKeyPressed())
			{
				int32 skillId = 0;
				if (input->GetIsAutoAttackSkillKeyPressed())
					skillId = 1;
				else
					skillId = 2;

				_isSkillKeyDown = true;
				std::wcout << L"스킬 사용!" << std::endl;
				Protocol::C_SKILL sendSkillPacket;
				sendSkillPacket.mutable_skillinfo()->set_skillid(skillId);
				GNetworkManager->Send(sendSkillPacket);
			}
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
				positionInfo.set_usedskillid(0);
				fsm->ChangeState(positionInfo);
			}
			//	Idle 상태에서 Move 상태로 변경하고 싶은데 앞길이 막혔을 경우..
			else
			{
				//	현재 바라보는 방향이랑 다른 방향으로 방향키를 눌렀을 경우..
				if (movekeyInputDir != move->GetMoveDir())
				{
					//	방향만 변경되었다고 패킷 전송...
					Protocol::C_CHANGE_MOVE_DIR changeMoveDirSendPacket;
					changeMoveDirSendPacket.set_movedir(movekeyInputDir);
					auto moveDirPacketSendBuffer = ServerPacketHandler::MakeSendBuffer(changeMoveDirSendPacket);
					GNetworkManager->Send(moveDirPacketSendBuffer);

					//	및 클라이언트에서 방향 변경
					move->SetMoveDir(movekeyInputDir);
				}

				//	현재 바라보는 방향이랑 같은 방향키를 눌렀을 경우...
				//	상태만 움직이는 상태로 변경되었다고 패킷 전송...
				Protocol::C_CHANGE_STATE changeStateSendPacket;
				changeStateSendPacket.set_state(Protocol::AIState::MOVE);
				auto changeStatePacketSendBuffer = ServerPacketHandler::MakeSendBuffer(changeStateSendPacket);
				GNetworkManager->Send(changeStatePacketSendBuffer);

				//	및 클라이언트에서 상태 변경
				fsm->ChangeState(Protocol::AIState::MOVE);
			}
		}
	}
}

void AIIdle::UpdateAnimation()
{
	auto fsm = GetOwner();
	ASSERT(fsm);
	if (fsm->GetCurrentStateName().compare(L"Idle") != 0)
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
		animName = L"PLAYER_IDLE_UP";
		break;
	case Protocol::MoveDir::UP_RIGHT:
		animName = L"PLAYER_IDLE_UP_RIGHT";
		break;
	case Protocol::MoveDir::RIGHT:
		animName = L"PLAYER_IDLE_RIGHT";
		break;
	case Protocol::MoveDir::DOWN_RIGHT:
		animName = L"PLAYER_IDLE_DOWN_RIGHT";
		break;
	case Protocol::MoveDir::DOWN:
		animName = L"PLAYER_IDLE_DOWN";
		break;
	case Protocol::MoveDir::DOWN_LEFT:
		animName = L"PLAYER_IDLE_DOWN_LEFT";
		break;
	case Protocol::MoveDir::LEFT:
		animName = L"PLAYER_IDLE_LEFT";
		break;
	case Protocol::MoveDir::UP_LEFT:
		animName = L"PLAYER_IDLE_UP_LEFT";
		break;
	}

	sprite->Play(animName);
}

void AIIdle::OnEnter()
{
	_isSkillKeyDown = false;
	UpdateAnimation();
}

void AIIdle::OnExit()
{

}

AIMonsterIdle::AIMonsterIdle()
	:	AIState(L"Idle")
{

}

AIMonsterIdle::~AIMonsterIdle()
{

}

void AIMonsterIdle::Initialize()
{

}

void AIMonsterIdle::Update(float deltaTime)
{

}

void AIMonsterIdle::UpdateAnimation()
{
	auto fsm = GetOwner();
	ASSERT(fsm);
	if (fsm->GetCurrentStateName().compare(L"Idle") != 0)
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
		animName = L"MONSTER_IDLE_UP";
		break;
	case Protocol::MoveDir::UP_RIGHT:
		animName = L"MONSTER_IDLE_UP_RIGHT";
		break;
	case Protocol::MoveDir::RIGHT:
		animName = L"MONSTER_IDLE_RIGHT";
		break;
	case Protocol::MoveDir::DOWN_RIGHT:
		animName = L"MONSTER_IDLE_DOWN_RIGHT";
		break;
	case Protocol::MoveDir::DOWN:
		animName = L"MONSTER_IDLE_DOWN";
		break;
	case Protocol::MoveDir::DOWN_LEFT:
		animName = L"MONSTER_IDLE_DOWN_LEFT";
		break;
	case Protocol::MoveDir::LEFT:
		animName = L"MONSTER_IDLE_LEFT";
		break;
	case Protocol::MoveDir::UP_LEFT:
		animName = L"MONSTER_IDLE_UP_LEFT";
		break;
	}

	sprite->Play(animName);
}

void AIMonsterIdle::OnEnter()
{
	UpdateAnimation();
}

void AIMonsterIdle::OnExit()
{

}
