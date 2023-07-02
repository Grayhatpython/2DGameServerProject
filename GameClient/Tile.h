#pragma once
#include "Actor.h"

class Tile : public Actor
{
	friend class Grid;
	friend class MapManager;

public:
	Tile();
	virtual ~Tile();

public:
	virtual void	Initialize() override;

public:
	void			SetIsBlocked(bool isBlocked) { _isBlocked = isBlocked; }
	bool			GetIsBlocked() const { return _isBlocked; }

	float			GetCost() const { return _f; }

private:
	std::vector<Tile*>	_adjacent;
	Tile*				_parent = nullptr;
	
	float	_f = 0.f;
	float	_g = 0.f;	

	bool	_isVisited = false;
	bool	_isBlocked = false;

};

