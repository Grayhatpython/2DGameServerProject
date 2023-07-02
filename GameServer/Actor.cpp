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
		//	지금 플레이어 상황에서 자기 자신이 죽었을 때 방에서 Leave시키고 다시 Enter 시키는 구조로 되어 있다.
		//	근데... Room Leave에서 플레이어와 연결된 Session과의 연결을 끊어버린다. ( nullptr로 만듬 )
		//	그래서 Enter하는 과정에서 플레이어와 연결된 Session에게 패킷을 전달해야 하는데
		//	Leave 과정에서 Session과의 연결을 끊어버려서 Crash가 난다.
		 
		//	Room Leave에서 Session과의 연결을 끊어버리는 이유는 해당 클라이언트와 연결이 끊어졌을 경우다
		//	즉, Disconnected를 위해서 해놓은 로직인데 위의 상황과 충돌된다.
		//	Player와 Session은 지금 순환참조인 상황이다.. 
		//	이유는 Player가 사라지더라도 Session을 살려서 남은 패킷을 전달하고 Ref을 끊기 위해서다.
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
		//	방향 정보가 없는데 ... 이 함수를 호출한다??
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