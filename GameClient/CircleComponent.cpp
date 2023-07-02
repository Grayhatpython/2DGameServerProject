#include "pch.h"
#include "CircleComponent.h"
#include "Actor.h"
#include "TransformComponent.h"

CircleComponent::CircleComponent()
{

}

float CircleComponent::GetRadius() const
{
	auto owner = GetOwner();
	assert(owner);
	auto transform = owner->GetComponent<TransformComponent>();
	return transform->GetScale() * _radius;
}

const Vector2& CircleComponent::GetCenter() const
{
	auto owner = GetOwner();
	//	TEMP
	assert(owner);
	auto transform = owner->GetComponent<TransformComponent>();
	return transform->GetPosition();
}
