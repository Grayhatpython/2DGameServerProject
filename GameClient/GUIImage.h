#pragma once
#include "GUIScreen.h"

class Font;
class GUIImage : public GUIScreen
{
public:
	GUIImage();
	virtual ~GUIImage();

public:
	virtual void			Initialize();
	virtual void			Render(const Vector2Int& cameraOffset);
	virtual void			DrawTexture(Texture* texture, const Vector2Int& position, const Vector2Int& cameraOffset = Vector2Int{ 0, 0 }) override;
	virtual bool			IsInRange(const Vector2Int& position) override;

};

