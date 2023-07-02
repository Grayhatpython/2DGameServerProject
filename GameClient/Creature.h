#pragma once
#include "Actor.h"

class Creature : public Actor
{
public:
	Creature();
	virtual ~Creature();

public:
	virtual void	Initialize() override;
	virtual void	SpecificActorUpdate(float deltaTime);

public:
	virtual void	OnDamaged();
};