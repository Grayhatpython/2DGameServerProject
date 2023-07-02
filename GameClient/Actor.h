#pragma once
#include "Component.h"

class Actor : public std::enable_shared_from_this<Actor>
{
public:
	Actor();
	virtual ~Actor();

public:
	virtual void	Initialize();

	void			Update(float deltaTime);
	void			UpdateComponents(float deltaTime);
	virtual void	SpecificActorUpdate(float deltaTime);

	void			ProcessInput(const struct InputState& state);
	virtual	void	SpecificProcessInput(const struct InputState& state);

public:
	template<typename T>
	T*	AddComponent()
	{
		Component* component;
		if (_type == Protocol::ActorType::NONE)
			component = new T();
		else
			component = cnew<T>(); //	StaticMemoryPool
		
		component->SetOwner(shared_from_this());

		int updateOrder = component->GetUpdateOrder();

		if (_type == Protocol::ActorType::NONE)
		{
			auto iter = _staticComponents.begin();
			for (; iter != _staticComponents.end(); ++iter)
			{
				if (updateOrder < (*iter)->GetUpdateOrder())
					break;
			}

			_staticComponents.insert(iter, component);
		}
		else
		{
			auto iter = _components.begin();
			for (; iter != _components.end(); ++iter)
			{
				if (updateOrder < (*iter)->GetUpdateOrder())
					break;
			}

			_components.insert(iter, component);
		}

		return static_cast<T*>(component);
	}

	template<typename T>
	bool RemoveComponent()
	{
		if (_components.empty())
			return false;

		auto& iter = std::find_if(_components.begin(),_components.end(),
			[Type = T::ComponentType](auto& component) {
				return component->IsSameType(Type);
			});

		if (iter != _components.end())
		{
			cdelete(*iter);
			_components.erase(iter);
		}

		return true;
	}

	template< class T >
	T*	 GetComponent() 
	{
		if (_type == Protocol::ActorType::NONE)
		{
			for (auto&& component : _staticComponents) {
				if (component->IsSameType(T::ComponentType))
					return static_cast<T*>(component);
			}
		}
		else
		{
			for (auto&& component : _components) {
				if (component->IsSameType(T::ComponentType))
					return static_cast<T*>(component);
			}
		}
		//	TEMP
		return nullptr;
	}

public:
	ActorState			GetState() const { return _state; }
	void				SetState(ActorState state);
	
	Protocol::ActorType	GetType() const { return _type; }
	void				SetType(Protocol::ActorType type) { _type = type; }

	const std::wstring& GetName() const { return _name; }
	void				SetName(const std::wstring& name) { _name = name; }

	int32				GetId() const { return _id; }
	void				SetId(int32 id) { _id = id; }

protected:
	ActorState			_state = ActorState::Active; 
	Protocol::ActorType	_type = Protocol::ActorType::NONE;
	std::wstring		_name;
	int32				_id;

	//	TEMP
	Vector<Component*>			_components;
	std::vector<Component*>		_staticComponents;
};

