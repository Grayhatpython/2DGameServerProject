#include "pch.h"
#include "Shadow.h"
#include "Texture.h"
#include "ActorManager.h"
#include "AssetManager.h"
#include "BoxComponent.h"
#include "TransformComponent.h"
#include "AnimSpriteComponent.h"

Shadow::Shadow()
{

}

Shadow::~Shadow()
{

}

void Shadow::Initialize()
{
	Effect::Initialize();

	SetName(L"Shadow");

	auto transform = AddComponent<TransformComponent>();
	auto sprite = AddComponent<AnimSpriteComponent>();
	sprite->SetDrawOrder(90);
}

void Shadow::SpecificActorUpdate(float deltaTime)
{
	auto owner = GetOwner();
	if (owner)
	{
		auto ownerTransform = owner->GetComponent<TransformComponent>();
		auto box = owner->GetComponent<BoxComponent>();
		auto transform = GetComponent<TransformComponent>();

		Vector2 position{};
		auto ownerPosition = ownerTransform->GetPosition();
		position.x = ownerPosition.x;
		position.y = ownerPosition.y + box->GetRange();

		transform->SetPosition(position);
	}
	else
		SetState(ActorState::Dead);
}

