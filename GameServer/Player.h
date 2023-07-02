#pragma once
#include "Actor.h"
#include "Protocol.pb.h"
#include "FieldOfView.h"

class Inventory;
class ClientSession;
class Player : public Actor
{
public:
	Player();
	virtual ~Player();

public:
	virtual void	Initialize() override;
	virtual void	OnDamaged(std::shared_ptr<Actor> attacker, int32 damage) override;
	virtual void	OnDead(std::shared_ptr<Actor> attacker) override;

	void			Update();
	void			Clear();

public:
	//	DB
	void			OnLeaveGame();
	void			EquipItem(Protocol::C_EQUIP_ITEM equipItemPacket);

public:
	void			ReCalculationAdditionalStat();

public:
	void							SetSession(std::shared_ptr<ClientSession> session) { _session = session; }
	std::shared_ptr<ClientSession>	GetSession() { return _session; }

	void							SetPlayerDbId(int32 id) { _playerDbId = id; }
	int32							GetPlayerDbId() { return _playerDbId; }

	void							SetInventory(std::shared_ptr<Inventory> inventory) { _inventory = inventory; }
	std::shared_ptr<Inventory>		GetInventory() { return _inventory; }

	void							SetWeaponDamage(int32 damage) { _weaponDamage = damage; }
	int32							GetWeaponDamage() { return _weaponDamage; }

	void							SetArmorDefence(int32 defence) { _armorDefence = defence; }
	int32							GetArmorDefence() { return _armorDefence; }

	virtual int32					GetTotalAttack() override { return GetStatInfo().attack() + _weaponDamage; }
	virtual int32					GetTotalDefence() override { return _armorDefence; }

	FieldOfView*					GetFieldOfView() { return _fieldOfView.get(); }

private:
	//	Ref 사이클 체크!
	std::shared_ptr<ClientSession>	_session;
	std::shared_ptr<Inventory>		_inventory;
	std::unique_ptr<FieldOfView>	_fieldOfView;

	//	DB Key
	int32							_playerDbId = 0;

	//	Buff or Equip 
	int32							_weaponDamage = 0;
	int32							_armorDefence = 0;
};

