#pragma once
#include "SpriteComponent.h"

enum class AnimSpriteDirection
{
	Row,
	Column
};

class Texture;
struct AnimSpriteInfo
{
	Texture* texture = nullptr;
	int32 width = 0;
	int32 height = 0;
	int32 standardXIndex = 0;
	int32 standardYIndex = 0;
	int32 maxFrameCount = 0;
	bool  repeat = false;
	AnimSpriteDirection animDir = AnimSpriteDirection::Row;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
};

class AnimSpriteComponent : public SpriteComponent
{
	COMPONENT_TYPE_DECLARATION(AnimSpriteComponent);	

public:
	AnimSpriteComponent(int32 drawOrder = 100);

public:
	virtual void		Update(float deltaTime);
	virtual void		Render(const Vector2Int& cameraOffset);

public:
	float				GetAnimFPS() const { return _animFPS; }
	void				SetAnimFPS(float fps) { _animFPS = fps; }

	void				Play(const std::wstring& name, bool reset = true);
	void				Reset();

private:
	float				_currentFrame = 0.f;
	int32				_currentAnimFrameCount = 0;
	
	float				_animFPS = 10.f;

	AnimSpriteInfo*		_currentAnimInfo = nullptr;
};

