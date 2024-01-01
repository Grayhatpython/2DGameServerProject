#include "pch.h"
#include "Player.h"
#include "ClientPacketHandler.h"
#include "Item.h"
#include "Inventory.h"
#include "DBProcess.h"
#include "DataManager.h"
#include "ClientSession.h"

Player::Player()
	:	Actor(Protocol::ActorType::PLAYER)
{

}

Player::~Player()
{
	std::cout << "~Player" << std::endl;
}

void Player::Initialize()
{
	auto player = std::static_pointer_cast<Player>(shared_from_this());
	_fieldOfView = std::make_unique<FieldOfView>();
	_fieldOfView->SetOwner(player);
}

void Player::OnDamaged(std::shared_ptr<Actor> attacker, int32 damage)
{
	Actor::OnDamaged(attacker, damage);
}

void Player::OnDead(std::shared_ptr<Actor> attacker)
{
	Actor::OnDead(attacker);
}

void Player::Update(float deltaTime)
{
	ASSERT(_fieldOfView);
	_fieldOfView->Update();

	//	TEMP
	/*auto state = GetState();
	if (state == Protocol::AIState::MOVE)
	{
		auto forwardSpeed = MOVE_DISTANCE * GetSpeed();
		Vector2 diff = _movePosition - GetPosition();

		if (Math::NearZero(diff.Length(), forwardSpeed * deltaTime))
		{
			SetPosition(_movePosition);
			_movePosition = {};
		}
		else
		{
			OutputDebugString(std::to_wstring(diff.Length()).c_str());
			OutputDebugString(L",");
			OutputDebugString(std::to_wstring(forwardSpeed * deltaTime).c_str());
			OutputDebugString(L"\n");
			auto forwardPosition = Vector2::Normalize(diff) * forwardSpeed * deltaTime;
			SetPosition(GetPosition() + forwardPosition);
		}
	}*/
}

void Player::Clear()
{
	SetRegion(nullptr);
	_fieldOfView->Clear();
}

void Player::OnLeaveGame()
{
	GDBProcess->UpdatePlayerHp(std::static_pointer_cast<Player>(shared_from_this()));
}

void Player::EquipItem(Protocol::C_EQUIP_ITEM equipItemPacket)
{
	auto player = std::static_pointer_cast<Player>(shared_from_this());
	ASSERT(player);
	ASSERT(_inventory);

	//	���� or ��ü ��û�� �������� ���� �����ϰ� ���� �ʴ�..? ����..
	auto hasItem = _inventory->GetItem(equipItemPacket.itemdbid());
	ASSERT(hasItem);

	//	������ ������ ��� ��Ŷ���� �ٲ���
	if (hasItem->GetItemType() == Protocol::ItemType::ITEM_TYPE_CONSUMABLE)
		return;


	//	�ߺ��� ������ ���� �˻� -> �̹� ���� �� ��� ����� ��û�� ��� ����
	if (equipItemPacket.equipped())
	{
		std::shared_ptr<Item> unEquippedItem = nullptr;

		if (hasItem->GetItemType() == Protocol::ItemType::ITEM_TYPE_WEAPON)
			unEquippedItem = _inventory->FindItem([](const std::shared_ptr<Item>& item) {
			return item->GetItemType() == Protocol::ItemType::ITEM_TYPE_WEAPON && item->GetEquipped();
				});
		else if (hasItem->GetItemType() == Protocol::ItemType::ITEM_TYPE_ARMOR)
		{
			auto armorType = std::static_pointer_cast<Armor>(hasItem)->GetArmorType();
			unEquippedItem = _inventory->FindItem([armorType](const std::shared_ptr<Item>& item) {
				return item->GetItemType() == Protocol::ItemType::ITEM_TYPE_ARMOR &&
				std::static_pointer_cast<Armor>(item)->GetArmorType() == armorType &&
				item->GetEquipped();
				});
		}

		if (unEquippedItem)
		{
			//	�� �޸� ���� -> �߿����� �ʴ� ���
			unEquippedItem->SetEquipped(false);

			GDBProcess->UpdatePlayerItemEquipped(player, unEquippedItem);

			Protocol::S_EQUIP_ITEM equipItemSendPacket;
			equipItemSendPacket.set_equipped(unEquippedItem->GetEquipped());
			equipItemSendPacket.set_itemdbid(unEquippedItem->GetItemDbId());
			auto sendBuffer = ClientPacketHandler::MakeSendBuffer(equipItemSendPacket);
			ASSERT(_session);
			_session->Send(sendBuffer);
		}
	}

	{
		//	�� �޸� ���� -> �߿����� �ʴ� ���
		hasItem->SetEquipped(equipItemPacket.equipped());

		GDBProcess->UpdatePlayerItemEquipped(player, hasItem);

		Protocol::S_EQUIP_ITEM equipItemSendPacket;
		equipItemSendPacket.set_equipped(equipItemPacket.equipped());
		equipItemSendPacket.set_itemdbid(equipItemPacket.itemdbid());
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(equipItemSendPacket);
		auto session = player->GetSession();
		ASSERT(session);

		session->Send(sendBuffer);
	}

	ReCalculationAdditionalStat();
}

void Player::ReCalculationAdditionalStat()
{
	_weaponDamage = 0;
	_armorDefence = 0;

	const auto& items = _inventory->GetItems();
	for (const auto& item : items)
	{
		if (item.second->GetEquipped() == false)
			continue;
		
		switch (item.second->GetItemType())
		{
		case Protocol::ItemType::ITEM_TYPE_WEAPON:
			{
				auto weapon = std::static_pointer_cast<Weapon>(item.second);
				_weaponDamage += weapon->GetDamage();
			}
			break;
		case Protocol::ItemType::ITEM_TYPE_ARMOR:
			{
				auto armor = std::static_pointer_cast<Armor>(item.second);
				_armorDefence += armor->GetDefence();
			}
			break;
		}
	}
}

