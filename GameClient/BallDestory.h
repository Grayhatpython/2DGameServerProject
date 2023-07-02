#pragma once
#include "Effect.h"

class BallDestory : public Effect
{
public:
	BallDestory();
	virtual ~BallDestory();

public:
	virtual void	Initialize() override;
	virtual void	SpecificActorUpdate(float deltaTime);
};
