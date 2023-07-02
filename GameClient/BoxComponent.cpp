#include "pch.h"
#include "BoxComponent.h"
#include "Actor.h"
#include "TransformComponent.h"


COMPONENT_TYPE_DEFINITION(Component, BoxComponent);

BoxComponent::BoxComponent(int32 updateOrder)
	: Component(updateOrder)
{

}

BoxComponent::~BoxComponent()
{

}

void BoxComponent::Update(float deltaTime)
{
	auto owner = GetOwner();

	ASSERT(owner);

	Vector2 range{ _range, _range };

	if (owner->GetName().compare(L"Ball") == 0)
		int a = 3;

	auto transform = owner->GetComponent<TransformComponent>();
	//	TEMP
	_min = transform->GetPosition() * transform->GetScale() - range;
	_max = transform->GetPosition() * transform->GetScale() + range;
}
