#pragma once
#include "Creature.h"

class Player : public Creature
{
public:
	Player();
	virtual ~Player();

public:
	virtual void	Initialize() override;
	virtual void	SpecificActorUpdate(float deltaTime);

public:
	virtual void	OnDamaged() override;

};

