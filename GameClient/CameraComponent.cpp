#include "pch.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "Actor.h"
#include "MapManager.h"
#include "FollowCamera.h"
#include "RenderManager.h"

COMPONENT_TYPE_DEFINITION(Component, CameraComponent);

CameraComponent::CameraComponent(int32 updateOrder)
	: Component(updateOrder)
{
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::Update(float deltaTime)
{
	//	TEMP
	auto owner = GetOwner();
	ASSERT(owner);

	auto screenSize = Vector2Int{ GRenderManager->GetScreenWidth(), GRenderManager->GetScreenHeight() };
	auto screenCenter = screenSize / 2;
	Vector2Int lookAtPosition{};
	auto target = GetTarget();
	if (target)
	{
		auto transform = target->GetComponent<TransformComponent>();
		lookAtPosition = Vector2ToVector2Int(transform->GetPosition());
	}

	else
		lookAtPosition = screenCenter;

	auto diff = lookAtPosition - screenCenter;

	if (diff.x < 0)
		diff.x = 0;
	if (diff.y < 0)
		diff.y = 0;
	if (diff.x > GMapManager->GetMapMax().x - screenSize.x)
		diff.x = GMapManager->GetMapMax().x - screenSize.x;
	if (diff.y > GMapManager->GetMapMax().y - screenSize.y)
		diff.y = GMapManager->GetMapMax().y - screenSize.y;

	GRenderManager->SetCameraOffset(diff);
}
