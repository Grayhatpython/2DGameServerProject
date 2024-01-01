#pragma once

enum class ActorState
{
	Active,
	DeActive,
	Dead,
};

class Region;
class Actor : public std::enable_shared_from_this<Actor>
{
public:
	Actor(Protocol::ActorType type);
	virtual ~Actor();

public:
	virtual void Initialize();
	virtual void Update(float deltaTime);
	virtual void OnDamaged(std::shared_ptr<Actor> attacker, int32 damage);
	virtual void OnDead(std::shared_ptr<Actor> attacker);

public:
	void							SetRegion(std::shared_ptr<Region> region) { _region = region; }
	std::shared_ptr<Region>			GetRegion() { return _region; }

	void							SetType(Protocol::ActorType type) { _type = type; }
	Protocol::ActorType				GetType() const { return _type; }

	void							SetInfo(const Protocol::ActorInfo& info) { _info.CopyFrom(info); }
	Protocol::ActorInfo&			GetInfo() { return _info; }
	const Protocol::ActorInfo&		GetInfo() const { return _info; }

	void							SetPositionInfo(const Protocol::PositionInfo& info) { _info.mutable_positioninfo()->CopyFrom(info); }
	const Protocol::PositionInfo&	GetPositionInfo() const { return _info.positioninfo(); }

	void							SetStatInfo(const Protocol::StatInfo& info) { _info.mutable_statinfo()->CopyFrom(info); }
	const Protocol::StatInfo&		GetStatInfo() const { return _info.statinfo(); }

	void							SetState(Protocol::AIState state) { _info.mutable_positioninfo()->set_state(state); }
	Protocol::AIState				GetState() const { return GetPositionInfo().state(); }

	void							SetPosition(const Vector2& position) { _info.mutable_positioninfo()->set_positionx(position.x); _info.mutable_positioninfo()->set_positiony(position.y); }
	Vector2							GetPosition() const { return Vector2{ GetPositionInfo().positionx(), GetPositionInfo().positiony() }; }

	void							SetId(int32 id) { _info.set_id(id); }
	int32							GetId() const { return static_cast<int32>(_info.id()); }

	void							SetMoveDir(Protocol::MoveDir moveDir) { _info.mutable_positioninfo()->set_movedir(moveDir); }
	Protocol::MoveDir				GetMoveDir() const { return GetPositionInfo().movedir(); }

	//	TEMP
	void							SetUsedSkillId(int32 id) { _info.mutable_positioninfo()->set_usedskillid(id); }
	int32							GetUsedSkillId() const { return GetPositionInfo().usedskillid(); }

	void							SetSpeed(float speed) { _info.mutable_statinfo()->set_speed(speed); }
	float							GetSpeed() const { return GetStatInfo().speed(); }

	void							SetHp(int32 hp) { _info.mutable_statinfo()->set_hp(Math::Clamp(hp, 0, GetMaxHp())); }
	int32							GetHp() const { return GetStatInfo().hp(); }

	void							SetMapHp(int32 maxHp) { _info.mutable_statinfo()->set_maxhp(maxHp); }
	int32							GetMaxHp() const { return GetStatInfo().maxhp(); }

	void							SetAttack(int32 damage) { _info.mutable_statinfo()->set_attack(damage); }
	int32							GetAttack() const { return GetStatInfo().attack(); }

	void							SetTotalExp(int32 exp) { _info.mutable_statinfo()->set_totalexp(exp); }
	int32							GetTotalExp() const { return GetStatInfo().totalexp(); }

	void							SetLevel(int32 level) { _info.mutable_statinfo()->set_level(level); }
	int32							GetLevel() const { return GetStatInfo().level(); }

	void							SetName(const std::string& name) { _info.set_name(name); }
	const std::string&				GetName() const { return _info.name(); }

	virtual std::shared_ptr<Actor>	GetOwner() { return shared_from_this(); }
	virtual int32					GetTotalAttack() { return GetStatInfo().attack(); }
	virtual int32					GetTotalDefence() { return 0; }

public:
	Vector2							GetForwardPosition();
	Protocol::MoveDir				GetMoveDirFromDirectionVector(const Vector2& vector);

protected:
	std::shared_ptr<Region>			_region;

	Protocol::ActorType				_type = Protocol::ActorType::NONE;
	Protocol::ActorInfo				_info;

};

