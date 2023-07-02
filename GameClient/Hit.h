#pragma once
#include "Effect.h"

class Hit : public Effect
{
public:
	Hit();
	virtual ~Hit();

public:
	virtual void	Initialize() override;
	virtual void	SpecificActorUpdate(float deltaTime);
};

