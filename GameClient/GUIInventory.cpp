#include "pch.h"
#include "GUIInventory.h"
#include "GUIInventoryItem.h"
#include "GUIInventoryEquippedItem.h"

#include "Texture.h"
#include "GUILable.h"
#include "AssetManager.h"
#include "RenderManager.h"

#include "Item.h"
#include "DataManager.h"
#include "InventoryManager.h"

#include "MyPlayer.h"
#include "StatComponent.h"
#include "ActorManager.h"

GUIInventory::GUIInventory()
	:	 GUIScreen(L"Inventory")
{
	_background = GAssetManager->GetTexture("../Assets/GUI/Inventory.png");

	auto screenWidth = GRenderManager->GetScreenWidth();
	_position = Vector2Int{ screenWidth - _background->GetWidth() , _background->GetHeight()};
}

GUIInventory::~GUIInventory()
{
	for (auto item : _inventoryItems)
		delete item;
	_inventoryItems.clear();
	
	for (auto& equippedItem : _equippedItems)
		delete equippedItem.second;
	_equippedItems.clear();

	for (auto& stat : _stats)
		delete stat.second;
	_stats.clear();

	if (_titleText)
	{
		delete _titleText;
		_titleText = nullptr;
	}
}

void GUIInventory::Initialize()
{
	InitializeInventoryItem();
	InitializeStats();
}

void GUIInventory::Render(const Vector2Int& cameraOffset)
{
	if(_background)
		GUIScreen::DrawTexture(_background, _position);

	if (_titleText)
		_titleText->Render(cameraOffset);

	for (auto& item : _inventoryItems)
		item->Render(cameraOffset);

	for (auto& equippedItem : _equippedItems)
		equippedItem.second->Render(cameraOffset);

	for (auto& stat : _stats)
		stat.second->Render(cameraOffset);
}

void GUIInventory::Update(float deltaTime)
{
}

bool GUIInventory::IsInRange(const Vector2Int& position)
{
	bool isinRange = GUIScreen::IsInRange(position);

	for (auto item : _inventoryItems)
	{
		if (item->IsInRange(position))
			_selectedGui = item;
	}
	
	return isinRange;
}

void GUIInventory::InitializeInventoryItem()
{
	{
		auto equippedHelmetItem = new GUIInventoryEquippedItem(L"Helmet");
		equippedHelmetItem->SetPosition(_position + Vector2Int{ 57, -186 });

		auto equippedArmorItem = new GUIInventoryEquippedItem(L"Armor");
		equippedArmorItem->SetPosition(_position + Vector2Int{ 57, -128 });

		auto equippedBootsItem = new GUIInventoryEquippedItem(L"Boots");
		equippedBootsItem->SetPosition(_position + Vector2Int{ 57, 13 });

		auto equippedWeaponItem = new GUIInventoryEquippedItem(L"Weapon");
		equippedWeaponItem->SetPosition(_position + Vector2Int{ -7, -2 });

		_equippedItems.insert(std::make_pair(equippedHelmetItem->GetName(), equippedHelmetItem));
		_equippedItems.insert(std::make_pair(equippedArmorItem->GetName(), equippedArmorItem));
		_equippedItems.insert(std::make_pair(equippedBootsItem->GetName(), equippedBootsItem));
		_equippedItems.insert(std::make_pair(equippedWeaponItem->GetName(), equippedWeaponItem));
	}

	{
		std::wstring inventoryItemName = L"InventoryItem_";
		Vector2Int standardOffsetPosition = Vector2Int{ -128, 75 };
		Vector2Int offsetPosition{};

		for (int32 i = 0; i < _itemRowCount; i++)
		{
			for (int32 j = 0; j < _itemColumnCount; j++)
			{
				auto slot = j + (i * _itemColumnCount);
				inventoryItemName = L"InventoryItem_";
				inventoryItemName += std::to_wstring(slot);

				_inventoryItems.push_back(new GUIInventoryItem(inventoryItemName));
				_inventoryItems[slot]->Initialize();

				auto backgroud = _inventoryItems[slot]->GetBackgroundTexture();
				offsetPosition.x = standardOffsetPosition.x + (j * backgroud->GetWidth());
				offsetPosition.y = standardOffsetPosition.y + (i * backgroud->GetHeight());
				_inventoryItems[slot]->SetPosition(_position + offsetPosition);
			}
		}
	}
}

void GUIInventory::InitializeStats()
{
	auto halfWidth = static_cast<int32>(_background->GetWidth() * _scale) / 2;
	auto halfHeight = static_cast<int32>(_background->GetHeight() * _scale) / 2;

	{
		_titleText = new GUILable(L"Inventory Title");
		//_titleText->Initialize();
		_titleText->SetFont(GAssetManager->GetFont("../Assets/Font/font_Bold.ttf"));
		_titleText->SetText(L"인벤토리");
		Texture* fontTexture = _titleText->GetFontTexture();

		if (fontTexture)
			_titleText->SetPosition(_position - Vector2Int{ halfWidth - fontTexture->GetWidth() / 2 - 5, halfHeight - fontTexture->GetHeight() / 2 - 2 });

		auto levelText = new GUILable(L"Level");
		levelText->SetText(L"Level", 14);
		fontTexture = levelText->GetFontTexture();
		if(fontTexture)
			levelText->SetPosition(_position - Vector2Int{ halfWidth - fontTexture->GetWidth() / 2 - 37, halfHeight - fontTexture->GetHeight() / 2 - 45 });

		auto levelValueText = new GUILable(L"LevelValue");
		levelValueText->SetText(L"0", 14);
		fontTexture = levelValueText->GetFontTexture();
		if (fontTexture)
			levelValueText->SetPosition(_position - Vector2Int{ halfWidth - fontTexture->GetWidth() / 2 - 77, halfHeight - fontTexture->GetHeight() / 2 - 45 });

		auto strengthText = new GUILable(L"Strength");
		strengthText->SetText(L"Strength:", 10);
		fontTexture = strengthText->GetFontTexture();
		if (fontTexture)
			strengthText->SetPosition(_position - Vector2Int{ halfWidth - fontTexture->GetWidth() / 2 - 15, halfHeight - fontTexture->GetHeight() / 2 - 70 });

		auto strengthValueText = new GUILable(L"StrengthValue");
		strengthValueText->SetText(L"0", 10);
		fontTexture = strengthValueText->GetFontTexture();
		if (fontTexture)
			strengthValueText->SetPosition(_position - Vector2Int{ halfWidth - fontTexture->GetWidth() / 2 - 100, halfHeight - fontTexture->GetHeight() / 2 - 70 });

		auto DefenceText = new GUILable(L"Defence");
		DefenceText->SetText(L"Defence:", 10);
		fontTexture = DefenceText->GetFontTexture();
		if (fontTexture)
			DefenceText->SetPosition(_position - Vector2Int{ halfWidth - fontTexture->GetWidth() / 2 - 15, halfHeight - fontTexture->GetHeight() / 2 - 83 });

		auto DefenceValueText = new GUILable(L"DefenceValue");
		DefenceValueText->SetText(L"0", 10);
		fontTexture = DefenceValueText->GetFontTexture();
		if (fontTexture)
			DefenceValueText->SetPosition(_position - Vector2Int{ halfWidth - fontTexture->GetWidth() / 2 - 100, halfHeight - fontTexture->GetHeight() / 2 - 83 });


		auto HpText = new GUILable(L"Hp");
		HpText->SetText(L"Hp:", 10);
		fontTexture = HpText->GetFontTexture();
		if (fontTexture)
			HpText->SetPosition(_position - Vector2Int{ halfWidth - fontTexture->GetWidth() / 2 - 15, halfHeight - fontTexture->GetHeight() / 2 - 96 });

		auto HpValueText = new GUILable(L"HpValue");
		HpValueText->SetText(L"0", 10);
		fontTexture = HpValueText->GetFontTexture();
		if (fontTexture)
			HpValueText->SetPosition(_position - Vector2Int{ halfWidth - fontTexture->GetWidth() / 2 - 100, halfHeight - fontTexture->GetHeight() / 2 - 96 });


		_stats.insert(std::make_pair(levelText->GetName(), levelText));
		_stats.insert(std::make_pair(levelValueText->GetName(), levelValueText));
		_stats.insert(std::make_pair(strengthText->GetName(), strengthText));
		_stats.insert(std::make_pair(strengthValueText->GetName(), strengthValueText));
		_stats.insert(std::make_pair(DefenceText->GetName(), DefenceText));
		_stats.insert(std::make_pair(DefenceValueText->GetName(), DefenceValueText));
		_stats.insert(std::make_pair(HpText->GetName(), HpText));
		_stats.insert(std::make_pair(HpValueText->GetName(), HpValueText));
	}
}

void GUIInventory::OnMousePressed(const Vector2Int& position)
{
	_prevMousePressedPosition = position;

}

void GUIInventory::OnMouseHeld(const Vector2Int& position)
{
	if (_titleText->IsInRange(_prevMousePressedPosition))
	{
		if (_prevMousePressedPosition.x != position.x || _prevMousePressedPosition.y != position.y)
		{
			auto diff = position - _prevMousePressedPosition;

			_position += diff;

			const auto& titlePosition = _titleText->GetPosition();
			Vector2Int titleMovePosition = titlePosition + diff;
			_titleText->SetPosition(titleMovePosition);

			for (auto item : _inventoryItems)
			{
				const auto& itemPosition = item->GetPosition();
				Vector2Int itemMovePosition = itemPosition + diff;
				item->SetPosition(itemMovePosition);
			}

			for (auto& equippedItem : _equippedItems)
			{
				const auto& itemPosition = equippedItem.second->GetPosition();
				Vector2Int itemMovePosition = itemPosition + diff;
				equippedItem.second->SetPosition(itemMovePosition);
			}

			for (auto& stat : _stats)
			{
				const auto& statPosition = stat.second->GetPosition();
				Vector2Int statMovePosition = statPosition + diff;
				stat.second->SetPosition(statMovePosition);
			}

			_prevMousePressedPosition = position;
		}
	}
}

void GUIInventory::OnMouseReleased(const Vector2Int& position)
{
	if (_selectedGui && _selectedGui->IsInRange(position))
		_selectedGui->OnMouseReleased(position);

	_selectedGui = nullptr;

	_prevMousePressedPosition = Vector2Int{};
}

void GUIInventory::OnKeyBoardPressed(SDL_Scancode keyCode)
{
}

void GUIInventory::OnKeyBoardReleased(SDL_Scancode keyCode)
{
	if (keyCode == SDL_SCANCODE_I)
	{
		if (_state == GUIState::Active)
			_state = GUIState::DeActive;
		else
		{
			_state = GUIState::Active;
			Refresh();
		}
	}
}


void GUIInventory::Refresh()
{
	const auto& items = GInventoryManager->GetItems();
	//	TEMP
	Map<int32, Item*> slotSortItems;
	for (auto& item : items)
		slotSortItems.insert(std::make_pair(item.second->GetSlot(), item.second));

	//	소유 아이템
	for (auto& item : slotSortItems)
	{
		auto slot = item.second->GetSlot();
		ASSERT(slot >= 0 || slot < _itemColumnCount * _itemRowCount);

		_inventoryItems[slot]->SetItem(item.second);
	}

	//	장착 아이템
	for (auto& item : items)
	{
		if (item.second->GetEquipped() == false)
			continue;

		if (item.second->GetItemType() == Protocol::ItemType::ITEM_TYPE_WEAPON)
		{
			auto weapon = _equippedItems.find(L"Weapon");
			if (weapon != _equippedItems.end())
				weapon->second->SetItemTexture(item.second);
		}
		else if (item.second->GetItemType() == Protocol::ItemType::ITEM_TYPE_ARMOR)
		{
			auto armor = static_cast<Armor*>(item.second);
			switch (armor->GetArmorType())
			{
			case Protocol::ArmorType::ARMOR_TYPE_HELMET:
				{
					auto helmet = _equippedItems.find(L"Helmet");
					if (helmet != _equippedItems.end())
						helmet->second->SetItemTexture(item.second);
				}
				break;
			case Protocol::ArmorType::ARMOR_TYPE_ARMOR:
				{
					auto armor = _equippedItems.find(L"Armor");
					if (armor != _equippedItems.end())
						armor->second->SetItemTexture(item.second);
				}
				break;
			case Protocol::ArmorType::ARMOR_TYPE_BOOTS:
				{
					auto boots = _equippedItems.find(L"Boots");
					if (boots != _equippedItems.end())
						boots->second->SetItemTexture(item.second);
				}
				break;
			}
		}
	}

	auto myPlayer = GActorManager->GetMyPlayer();
	ASSERT(myPlayer);
	auto stat = myPlayer->GetComponent<StatComponent>();
	ASSERT(stat);
	stat->ReCalculationAdditionalStat();

	auto levelValueText = _stats.find(L"LevelValue");
	if (levelValueText != _stats.end())
		levelValueText->second->SetText(std::to_wstring(stat->GetLevel()), 14);

	auto strengthValueText = _stats.find(L"StrengthValue");
	if (strengthValueText != _stats.end())
		strengthValueText->second->SetText(std::to_wstring(stat->GetAttack() + stat->GetWeaponDamage()), 10);

	auto defenceValueText = _stats.find(L"DefenceValue");
	if (defenceValueText != _stats.end())
		defenceValueText->second->SetText(std::to_wstring(stat->GetArmorDefence()), 10);

	auto hpValueText =_stats.find(L"HpValue");
	if (hpValueText != _stats.end())
		hpValueText->second->SetText(std::to_wstring(stat->GetHp()), 10);
}

void GUIInventory::RefreshHp()
{
	auto myPlayer = GActorManager->GetMyPlayer();
	ASSERT(myPlayer);
	auto stat = myPlayer->GetComponent<StatComponent>();
	ASSERT(stat);
	stat->ReCalculationAdditionalStat();

	auto hpValueText = _stats.find(L"HpValue");
	if (hpValueText != _stats.end())
		hpValueText->second->SetText(std::to_wstring(stat->GetHp()), 10);
}
