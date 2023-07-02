#pragma once
#include "Actor.h"
#include "DataSheet.h"
class Projectile : public Actor
{
public:
	Projectile(const GameData::SkillData& skillData);
	virtual ~Projectile();

public:
	virtual void			Update();

public:
	void							SetOwner(std::weak_ptr<Actor> owner) { _owner = owner; }
	virtual	std::shared_ptr<Actor>	GetOwner() override { return _owner.lock(); }

protected:
	uint64						_nextMoveTick = 0;

	std::weak_ptr<Actor>		_owner;
	const GameData::SkillData&	_skillData;
};

