#pragma once
#include "Component.h"

class CircleComponent : public Component
{
public:
	CircleComponent();

public:
	void			SetRadius(float radius) { _radius = radius; }
	float			GetRadius() const;

	const Vector2&	GetCenter() const;

private:
	float			_radius = 0.f;
};

