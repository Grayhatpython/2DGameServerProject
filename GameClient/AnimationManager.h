#pragma once

#include "AnimSpriteComponent.h"
class AnimationManager
{
public:
	AnimationManager();
	~AnimationManager();

public:
	void Clear();

public:
	void		AddAnimInfo(const std::wstring& name, const AnimSpriteInfo& animInfo);
	void		AddAnimInfo(const std::wstring& name, Texture* texture, int32 width, int32 height,
		int32 standardXIndex, int32 standardYIndex, int32 maxFrameCount, bool repeat = true, 
		AnimSpriteDirection animDir = AnimSpriteDirection::Row, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void		RemoveAnimInfo(const std::wstring& name);

public:
	AnimSpriteInfo*		FindAnimInfo(const std::wstring& name);

private:
	const AnimationManager& operator = (const AnimationManager&) = delete;
	AnimationManager(const AnimationManager&) = delete;

private:
	std::unordered_map<std::wstring, std::unique_ptr<AnimSpriteInfo>>		_animInfos;
};

extern AnimationManager* GAnimationManager;
