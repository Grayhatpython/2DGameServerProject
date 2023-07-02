#pragma once
#include "Actor.h"
#include "DataSheet.h"
class Player;
class Monster : public Actor
{
public:
	Monster();
	virtual ~Monster();

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void OnDamaged(std::shared_ptr<Actor> attacker, int32 damage) override;
	virtual void OnDead(std::shared_ptr<Actor> attacker) override;

protected:
	virtual void UpdateIdle();
	virtual void UpdateMove();
	virtual void UpdateSkill();
	virtual void UpdateDeath();

private:
	void					BroadCastChangeState(const Protocol::AIState& state);
	GameData::RewardData	GetRandomRewardData();

public:
	std::shared_ptr<Player>	GetTargetPlayer() { return _target.lock(); }

private:
	//	TEMP
	uint64					_nextSearchTick = 0;
	int32					_numberOfTilesToSearch = 8;

	uint64					_nextMoveTick = 0;
	int32					_numberOfTilesToChase = 8;

	int32					_numberOfTilesToSkill = 1;

	uint64					_nextUseSkillTick = 0;

	int32					_dataSheetId = 0;

	std::weak_ptr<Player>	_target;
};

