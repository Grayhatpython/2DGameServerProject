#pragma once

class BoxComponent;
class CircleComponent;

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();

public:
	void HasCollsion(BoxComponent* box, OUT BoxComponent& outBox);
	bool HasCollsion(Vector2 position);

private:
	const CollisionManager& operator = (const CollisionManager&) = delete;
	CollisionManager(const CollisionManager&) = delete;

private:
};

extern CollisionManager* GCollisionManager;

//bool Intersect(CircleComponent* circle, CircleComponent* circle1);
inline bool Intersect(BoxComponent* box, BoxComponent* box1);
inline bool Intersect(BoxComponent* box, const Vector2& position);
