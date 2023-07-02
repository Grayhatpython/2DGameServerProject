#pragma once
#include "Effect.h"

class Sweep : public Effect
{
public:
	Sweep();
	virtual ~Sweep();

public:
	virtual void	Initialize() override;
	virtual void	SpecificActorUpdate(float deltaTime);

public:
	void						SetOwner(std::weak_ptr<Actor> owner) { _owner = owner; }
	std::shared_ptr<Actor>		GetOwner() { return _owner.lock(); }

private:
	std::weak_ptr<Actor>		_owner;
};

