#include "pch.h"
#include "Ball.h"
#include "Region.h"
#include "Field.h"
#include "ClientPacketHandler.h"

#include "DataManager.h"

Ball::Ball()
	:	Projectile(DataManager::FindSkillData(Protocol::SkillType::SKILL_PROJECTILE_ATTACK))
{

}

Ball::~Ball()
{
	std::wcout << "~Ball" << std::endl;
}

void Ball::Update()
{
	ASSERT(GetOwner());
	ASSERT(_region);

	if (_nextMoveTick > ::GetTickCount64())
		return;

	//	음.. 서버랑 클라 같은 속도로 돌고 싶은데 고민이네..

	auto waitTick = static_cast<uint64>(1000 / _skillData.projectileInfo.speed);
	_nextMoveTick = ::GetTickCount64() + waitTick;

	auto movePosition = GetForwardPosition();

	auto field = _region->GetField();
	ASSERT(field);

	//	틱 계산하면서 시물레이션 하자...
	if (field->ModifyActorsMappingTableByMove(shared_from_this(), movePosition))
	{
		Protocol::S_MOVE sendMovePacket;
		sendMovePacket.set_actorid(GetId());
		const auto& positionInfo = _info.positioninfo();
		sendMovePacket.mutable_positioninfo()->CopyFrom(positionInfo);
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(sendMovePacket);
		_region->BroadCast(GetPosition(), sendBuffer);

		/*_room->PushJobTimer(waitTick, [ball = shared_from_this()]() {
			ball->Update();
			});*/
	}
	else
	{
		auto target = field->FindActorFromPosition(movePosition);
		if (target)
		{
			//	Hit 
			target->OnDamaged(shared_from_this(), _skillData.damage + GetOwner()->GetTotalAttack());
		}

		//	어차피 Update가 한 스레드에서만 처리하기 때문에 지금 상황에서는 여기서 바로 해도 상관없다
		_region->Leave(GetId());
	}
}
