#include "pch.h"
#include "Projectile.h"


Projectile::Projectile(const GameData::SkillData& skillData)
	: Actor(Protocol::ActorType::PROJECTILE), _skillData(skillData)
{

}

Projectile::~Projectile()
{
	std::wcout << "~Projectile" << std::endl;
}

void Projectile::Update()
{
	
}
