#pragma once
#include "Actor.h"

class Projectile : public Actor
{
public:
	Projectile();
	virtual ~Projectile();

public:
	void					SetOwner(std::shared_ptr<Actor> owner) { _owner = owner; }
	std::shared_ptr<Actor>	GetOwner() { return _owner; }

private:
	std::shared_ptr<Actor>	_owner;
};

