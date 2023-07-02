#include "pch.h"
#include "AnimationManager.h"
#include "Texture.h"

AnimationManager* GAnimationManager = nullptr;

AnimationManager::AnimationManager()
{

}

AnimationManager::~AnimationManager()
{

}

void AnimationManager::Clear()
{
	_animInfos.clear();
}

void AnimationManager::AddAnimInfo(const std::wstring& name, const AnimSpriteInfo& animInfo)
{
	auto anim = std::make_unique<AnimSpriteInfo>(animInfo);
	_animInfos.insert(std::make_pair(name, std::move(anim)));
}

void AnimationManager::AddAnimInfo(const std::wstring& name, Texture* texture, int32 width, int32 height,
	int32 standardXIndex, int32 standardYIndex, int32 maxFrameCount, bool repeat, AnimSpriteDirection animDir, SDL_RendererFlip flip)
{
	AnimSpriteInfo animInfo;
	animInfo.texture = texture;
	animInfo.width = width;
	animInfo.height = height;
	animInfo.standardXIndex = standardXIndex;
	animInfo.standardYIndex = standardYIndex;
	animInfo.maxFrameCount = maxFrameCount;
	animInfo.repeat = repeat;
	animInfo.animDir = animDir;
	animInfo.flip = flip;

	auto anim = std::make_unique<AnimSpriteInfo>(animInfo);
	_animInfos.insert(std::make_pair(name, std::move(anim)));
}

void AnimationManager::RemoveAnimInfo(const std::wstring& name)
{
	_animInfos.erase(name);
}

AnimSpriteInfo* AnimationManager::FindAnimInfo(const std::wstring& name)
{
	auto iter = _animInfos.find(name);
	if (iter != _animInfos.end())
		return iter->second.get();

	return nullptr;
}
