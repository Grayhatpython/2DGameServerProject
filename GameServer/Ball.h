#pragma once
#include "Projectile.h"

class Ball : public Projectile
{
public:
	Ball();
	virtual ~Ball();

public:
	virtual void Update() override;

private:
};

