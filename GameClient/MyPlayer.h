#pragma once
#include "Creature.h"

class MyPlayer : public Creature
{
public:
	MyPlayer();
	virtual ~MyPlayer();

public:
	virtual void	Initialize() override;
	virtual void	SpecificActorUpdate(float deltaTime);

public:
	virtual void	OnDamaged() override;

};
