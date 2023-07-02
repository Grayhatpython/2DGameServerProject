#pragma once
#include "GUIScreen.h"

class GUILable;
class GUIInventoryItem;
class GUIInventoryEquippedItem;
class GUIInventory : public GUIScreen
{
public:
	GUIInventory();
	virtual ~GUIInventory();

public:
	virtual void			Initialize();
	virtual void			Render(const Vector2Int& cameraOffset);
	virtual void			Update(float deltaTime);
	virtual bool			IsInRange(const Vector2Int& position) override;

private:
	void					InitializeInventoryItem();
	void					InitializeStats();

public:
	virtual void			OnMousePressed(const Vector2Int& position) override;
	virtual void			OnMouseHeld(const Vector2Int& position) override;
	virtual void			OnMouseReleased(const Vector2Int& position) override;
	virtual void			OnKeyBoardPressed(SDL_Scancode keyCode) override;
	virtual void			OnKeyBoardReleased(SDL_Scancode keyCode) override;

public:
	void					Refresh();
	void					RefreshHp();

private:
	//	풀링이 필요없을 거 같음 .. 어차피 한번 생성되면 게임 꺼야 없어짐
	std::vector<GUIInventoryItem*>	_inventoryItems;
	std::unordered_map<std::wstring, GUIInventoryEquippedItem*>		_equippedItems;

	GUIScreen*			_selectedGui = nullptr;
	Vector2Int			_prevMousePressedPosition{};

	//	TEMP
	int32				_itemColumnCount = 9;
	int32				_itemRowCount = 5;

	GUILable*			_titleText = nullptr;
	std::unordered_map<std::wstring, GUILable*>	_stats;
	
};



