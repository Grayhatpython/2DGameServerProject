#include "pch.h"
#include "Actor.h"
#include "Region.h"
#include "Player.h"
#include "Monster.h"
#include "ClientPacketHandler.h"
#include "Protocol.pb.h"

Actor::Actor(Protocol::ActorType type)
	: _type(type)
{

}

Actor::~Actor()
{

}

void Actor::Initialize()
{

}

void Actor::Update()
{

}

//	TODO : Ref Count
void Actor::OnDamaged(std::shared_ptr<Actor> attacker, int32 damage)
{
	auto region = GetRegion();
	ASSERT(region);

	auto calcDamage = Math::Max(damage - GetTotalDefence(), 0);
	SetHp(Math::Max(GetHp() - calcDamage, 0));

	{
		//	Send HP Change -> All Session
		Protocol::S_CHANGE_HP sendChangeHpPacket;
		sendChangeHpPacket.set_actorid(GetId());
		sendChangeHpPacket.set_hp(GetHp());
		auto region = GetRegion();
		ASSERT(region);
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(sendChangeHpPacket);
		region->BroadCast(GetPosition(), sendBuffer);
	}

	if (GetHp() <= 0)
	{
		OnDead(attacker);
	}

}

void Actor::OnDead(std::shared_ptr<Actor> attacker)
{
	auto region = GetRegion();
	ASSERT(region);

	{
		//	Send Death Actor -> All Session
		Protocol::S_DEATH sendDeathPacket;
		sendDeathPacket.set_actorid(GetId());
		sendDeathPacket.set_attackerid(attacker->GetId());
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(sendDeathPacket);
		region->BroadCast(GetPosition(), sendBuffer);
	}

	{
		//	���� �÷��̾� ��Ȳ���� �ڱ� �ڽ��� �׾��� �� �濡�� Leave��Ű�� �ٽ� Enter ��Ű�� ������ �Ǿ� �ִ�.
		//	�ٵ�... Room Leave���� �÷��̾�� ����� Session���� ������ ���������. ( nullptr�� ���� )
		//	�׷��� Enter�ϴ� �������� �÷��̾�� ����� Session���� ��Ŷ�� �����ؾ� �ϴµ�
		//	Leave �������� Session���� ������ ��������� Crash�� ����.
		 
		//	Room Leave���� Session���� ������ ��������� ������ �ش� Ŭ���̾�Ʈ�� ������ �������� ����
		//	��, Disconnected�� ���ؼ� �س��� �����ε� ���� ��Ȳ�� �浹�ȴ�.
		//	Player�� Session�� ���� ��ȯ������ ��Ȳ�̴�.. 
		//	������ Player�� ��������� Session�� ����� ���� ��Ŷ�� �����ϰ� Ref�� ���� ���ؼ���.
		std::shared_ptr<Player> player;
		std::shared_ptr<ClientSession> session;
		if (_type == Protocol::ActorType::PLAYER)
		{
			player = std::static_pointer_cast<Player>(shared_from_this());
			session = player->GetSession();
		}

		region->Leave(GetId());

		if (_type == Protocol::ActorType::PLAYER)
			player->SetSession(session);
	}

	{
		Protocol::PositionInfo positionInfo;
		positionInfo.set_movedir(Protocol::MoveDir::DOWN);
		positionInfo.set_state(Protocol::AIState::IDLE);
		positionInfo.set_usedskillid(0);
		positionInfo.set_positionx(16.f);
		positionInfo.set_positiony(16.f);

		SetPositionInfo(positionInfo);
		SetHp(GetMaxHp());

		region->Enter(shared_from_this(), true);
	}

	std::wcout << L"OnDead" << std::endl;
}


Vector2 Actor::GetForwardPosition()
{
	auto position = GetPosition();
	switch (GetMoveDir())
	{
	case Protocol::MoveDir::UP:
		return position + Vector2{ 0.f, -MOVE_DISTANCE };
	case Protocol::MoveDir::UP_RIGHT:
		return position + Vector2{ MOVE_DISTANCE, -MOVE_DISTANCE };
	case Protocol::MoveDir::RIGHT:
		return position + Vector2{ MOVE_DISTANCE, 0.f };
	case Protocol::MoveDir::DOWN_RIGHT:
		return position + Vector2{ MOVE_DISTANCE, MOVE_DISTANCE };
	case Protocol::MoveDir::DOWN:
		return position + Vector2{ 0.f, MOVE_DISTANCE };
	case Protocol::MoveDir::DOWN_LEFT:
		return position + Vector2{ -MOVE_DISTANCE, MOVE_DISTANCE };
	case Protocol::MoveDir::LEFT:
		return position + Vector2{ -MOVE_DISTANCE, 0.f };
	case Protocol::MoveDir::UP_LEFT:
		return position + Vector2{ -MOVE_DISTANCE, -MOVE_DISTANCE };
	default:
		//	���� ������ ���µ� ... �� �Լ��� ȣ���Ѵ�??
		ASSERT(false);
		return Vector2{};
	}
}

Protocol::MoveDir Actor::GetMoveDirFromDirectionVector(const Vector2& vector)
{
	if (vector.x > 0 && vector.y > 0)
		return Protocol::MoveDir::DOWN_RIGHT;
	else if (vector.x < 0 && vector.y > 0)
		return Protocol::MoveDir::DOWN_LEFT;
	else if (vector.x > 0 && vector.y < 0)
		return Protocol::MoveDir::UP_RIGHT;
	else if (vector.x < 0 && vector.y < 0)
		return Protocol::MoveDir::UP_LEFT;
	else if (vector.x > 0)
		return Protocol::MoveDir::RIGHT;
	else if (vector.x < 0)
		return Protocol::MoveDir::LEFT;
	else if (vector.y > 0)
		return Protocol::MoveDir::DOWN;
	else /*if (vector.y < 0)*/
		return Protocol::MoveDir::UP;
}