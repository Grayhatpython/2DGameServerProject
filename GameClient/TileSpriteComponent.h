#pragma once
#include "SpriteComponent.h"

struct TileSpriteInfo
{
	int32 offsetX = 0;
	int32 offsetY = 0;
};

class TileSpriteComponent : public SpriteComponent
{
	COMPONENT_TYPE_DECLARATION(TileSpriteComponent);

public:
	TileSpriteComponent(int32 drawOrder = 10);
	virtual ~TileSpriteComponent();

public:
	virtual void	Render(const Vector2Int& cameraOffset);

public:
	void	SetWidth(int32 width) { _width = width; }
	void	SetHeight(int32 height) { _height = height; }
	void	AddTileSpriteInfo(int32 offsetX, int32 offsetY)
	{
		TileSpriteInfo tileSpriteInfo;
		tileSpriteInfo.offsetX = offsetX;
		tileSpriteInfo.offsetY = offsetY;
		_tileSpriteInfos.push_back(tileSpriteInfo);
	}

private:
	int32						_width = 0;
	int32						_height = 0;
	std::vector<TileSpriteInfo>	_tileSpriteInfos;
};

