#pragma once
#include "Component.h"

class Texture;
class SpriteComponent : public Component
{
	COMPONENT_TYPE_DECLARATION(SpriteComponent);	

public:
	SpriteComponent(int32 drawOrder = 150);
	virtual ~SpriteComponent();

public:
	virtual void	Render(const Vector2Int& cameraOffset);
	virtual void	SetTexture(Texture* texture);

	bool			HasIntersection(const SDL_Rect& rect);

public:
	int32			GetDrawOrder() const { return _drawOrder; }
	void			SetDrawOrder(int32 drawOrder) { _drawOrder = drawOrder; }
	int32			GetTextureWidth() const { return _textureWidth; }
	int32			GetTextureHeight() const { return _textureHeight; }

protected:
	Texture*	_texture = nullptr;
	int32		_drawOrder = 0;
	int32		_textureWidth = 0;
	int32		_textureHeight = 0;
};

