#pragma once
#include "GUIScreen.h"

class Item;
class Texture;
class GUIInventoryItem : public GUIScreen
{
public:
	GUIInventoryItem(const std::wstring& name);
	virtual ~GUIInventoryItem();

public:
	virtual void			Initialize();
	virtual void			Render(const Vector2Int& cameraOffset);
	virtual void			Update(float deltaTime);

public:
	virtual void			OnMousePressed(const Vector2Int& position);
	virtual void			OnMouseReleased(const Vector2Int& position);

public:
	void					SetItem(Item* item);

private:
	Texture*				_equippedTexture = nullptr;
	Texture*				_itemTexture = nullptr;

	Item*					_item = nullptr;
};
