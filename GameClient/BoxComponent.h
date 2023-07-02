#pragma once
#include "Component.h"

class BoxComponent : public Component
{
	COMPONENT_TYPE_DECLARATION(BoxComponent);

public:
	BoxComponent(int32 updateOrder = 200);
	virtual ~BoxComponent();

public:
	virtual void Update(float deltaTime) override;

public:
	void	SetRange(float range) { _range = range; }
	float	GetRange() const { return _range; }

	Vector2 GetMin() const { return _min; }
	Vector2 GetMax() const { return _max; }

private:
	float	_range = 0.f;
	Vector2 _min{};
	Vector2 _max{};
};

