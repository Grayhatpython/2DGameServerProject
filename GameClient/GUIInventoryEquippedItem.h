#pragma once
#include "GUIScreen.h"

class Item;
class Texture;
class GUIInventoryEquippedItem : public GUIScreen
{
public:
	GUIInventoryEquippedItem(const std::wstring& name);
	virtual ~GUIInventoryEquippedItem();

public:
	virtual void			Initialize();
	virtual void			Render(const Vector2Int& cameraOffset);
	virtual void			Update(float deltaTime);

public:
	void					SetItemTexture(Item* item);

private:
	Item*					_item = nullptr;
	Texture*				_itemTexture = nullptr;
};
