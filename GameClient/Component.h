#pragma once

#define COMPONENT_TYPE_DECLARATION( componet ) 									\
public:																			\
    static const std::size_t ComponentType;                                     \
    virtual bool IsSameType( const std::size_t componentType ) const override;	\

#define COMPONENT_TYPE_DEFINITION( BaseComponent, DerivedComponent )											\
const std::size_t DerivedComponent::ComponentType = std::hash< std::string >()( _STRING2( DerivedComponent ) );	\
bool DerivedComponent::IsSameType( const std::size_t componentType ) const										\
{																												\
        if ( componentType == DerivedComponent::ComponentType )													\
            return true;																						\
        return BaseComponent::IsSameType( componentType );														\
}     


class Actor;
class Component
{
public:
	Component(int32 updateOrder = 100);
	virtual ~Component();

public:
	virtual void Update(float deltaTime);
	virtual void ProcessInput(const struct InputState& state);

public:
	virtual bool                    IsSameType(const std::size_t componentType) const { return ComponentType == componentType; }

public:
	void							SetOwner(std::weak_ptr<Actor> owner) { _owner = owner; }
	std::shared_ptr<Actor>			GetOwner() { return _owner.lock(); }
	const std::shared_ptr<Actor>	GetOwner() const { return _owner.lock(); }
	int32							GetUpdateOrder() { return _updateOrder; }

private:
	//	TEMP
	std::weak_ptr<Actor>		_owner;
	int32						_updateOrder = 0;

	static const std::size_t    ComponentType;
};
