#include "pch.h"
#include "CollisionManager.h"
#include "CircleComponent.h"
#include "BoxComponent.h"
#include "Actor.h"
#include "Projectile.h"
#include "ActorManager.h"

CollisionManager* GCollisionManager = nullptr;

CollisionManager::CollisionManager()
{

}

CollisionManager::~CollisionManager()
{

}

void CollisionManager::HasCollsion(BoxComponent* box, OUT BoxComponent& outBox)
{
	auto& actors = GActorManager->GetActors();

	for (auto actor : actors)
	{
		auto box1 = actor.second->GetComponent<BoxComponent>();
		
		if (box1 == nullptr || box == box1)
			continue;
			
		auto boxOwner = box->GetOwner();
		auto box1Owner = box1->GetOwner();

		/*if (boxOwner->GetType() == ActorType::Projectile && std::static_pointer_cast<Projectile>(boxOwner)->GetOwner() == box1Owner)
			continue;*/

		if (Intersect(box, box1))
		{
			outBox = *box1;
			return;
		}
	}
}

bool CollisionManager::HasCollsion(Vector2 position)
{
	auto& actors = GActorManager->GetActors();

	for (auto actor : actors)
	{
		auto box = actor.second->GetComponent<BoxComponent>();

		if (box == nullptr)
			continue;

		auto boxOwner = box->GetOwner();
		/*if (boxOwner == nullptr || boxOwner->GetType() == ActorType::Projectile)
			continue;*/

		if (Intersect(box, position))
			return true;
	}

	return false;
}


//bool :Intersect(CircleComponent* circle, CircleComponent* circle1)
//{
//		auto diff = circle->GetCenter() - circle1->GetCenter();
//		float distanceSq = diff.LengthSq();
//
//		float radiusSum = circle->GetRadius() + circle1->GetRadius();
//		radiusSum *= radiusSum;
//
//		if (distanceSq <= radiusSum)
//			return true;
// 
//	return false;
//}


bool Intersect(BoxComponent* box, BoxComponent* box1)
{
	if (box->GetMin().x < box1->GetMax().x && box->GetMax().x > box1->GetMin().x &&
		box->GetMin().y < box1->GetMax().y && box->GetMax().y > box1->GetMin().y)
		return true;

	return false;
}

inline bool Intersect(BoxComponent* box, const Vector2& position)
{
	Vector2 posMax{};
	Vector2 posMin{};
	posMax.x = position.x + box->GetRange();
	posMax.y = position.y + box->GetRange();
	posMin.x = position.x - box->GetRange();
	posMin.y = position.y - box->GetRange();

	if (box->GetMin().x < posMax.x && box->GetMax().x > posMin.x &&
		box->GetMin().y < posMax.y && box->GetMax().y > posMin.y)
		return true;

	return false;
}

