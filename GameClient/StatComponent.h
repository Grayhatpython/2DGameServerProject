#pragma once
#include "Component.h"

class StatComponent : public Component
{
	COMPONENT_TYPE_DECLARATION(StatComponent);

public:
	StatComponent();
	virtual ~StatComponent();

public:
	void						SetStatInfo(const Protocol::StatInfo& info) { _statInfo.CopyFrom(info); }
	const Protocol::StatInfo&	GetStatInfo() const { return _statInfo; }

	void						SetSpeed(float speed) { _statInfo.set_speed(speed); }
	float						GetSpeed() const { return _statInfo.speed(); }

	void						SetHp(int32 hp) { _statInfo.set_hp(hp); }
	int32						GetHp() const { return _statInfo.hp(); }

	void						SetMapHp(int32 maxHp) { _statInfo.set_maxhp(maxHp); }
	int32						GetMaxHp() const { return _statInfo.maxhp(); }

	void						SetAttack(int32 damage) { _statInfo.set_attack(damage); }
	int32						GetAttack() const { return _statInfo.attack(); }

	void						SetTotalExp(int32 exp) { _statInfo.set_totalexp(exp); }
	int32						GetTotalExp() const { return _statInfo.totalexp(); }

	void						SetLevel(int32 level) { _statInfo.set_level(level); }
	int32						GetLevel() const { return _statInfo.level(); }

	void						SetWeaponDamage(int32 damage) { _weaponDamage = damage; }
	int32						GetWeaponDamage() { return _weaponDamage; }

	void						SetArmorDefence(int32 defence) { _armorDefence = defence; }
	int32						GetArmorDefence() { return _armorDefence; }

	void						ReCalculationAdditionalStat();

private:
	Protocol::StatInfo _statInfo;


	//	Buff or Equip 
	int32							_weaponDamage = 0;
	int32							_armorDefence = 0;
};

