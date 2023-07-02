#pragma once
#include "Effect.h"

class Actor;
class Shadow : public Effect
{
public:
	Shadow();
	virtual ~Shadow();

public:
	virtual void				Initialize() override;
	virtual void				SpecificActorUpdate(float deltaTime);

public:
	void						SetOwner(std::weak_ptr<Actor> owner) { _owner = owner; }
	std::shared_ptr<Actor>		GetOwner() { return _owner.lock(); }

private:
	std::weak_ptr<Actor>		_owner;
};

