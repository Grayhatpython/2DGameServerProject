#pragma once
#include "Creature.h"

class Monster : public Creature
{
public:
	Monster();
	virtual ~Monster();

public:
	virtual void	Initialize() override;
	virtual void	SpecificActorUpdate(float deltaTime);

public:
	virtual void	OnDamaged() override;
};

