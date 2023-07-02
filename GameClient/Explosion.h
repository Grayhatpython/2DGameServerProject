#pragma once
#include "Effect.h"

class Explosion : public Effect
{
public:
	Explosion();
	virtual ~Explosion();

public:
	virtual void	Initialize() override;
	virtual void	SpecificActorUpdate(float deltaTime);


};

