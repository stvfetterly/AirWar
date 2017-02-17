#include "stdafx.h"
#include "Weapon.h"
#include "ServiceLocator.h"
#include "Game.h"

int Weapon::numWeapons = 0;

Weapon::Weapon(float xVel, float yVel, WeaponsManager::WeaponType type) : _damage(0.0), _type(type), _firedFrom(ENEMY)
{
	const float missileDamage = 70;
	const float bombDamage = 100;
	const float bulletDamage = 10;
	const float laserDamage = 5;

	_xVelocity = xVel;
	_yVelocity = yVel;

	//Load weapon image
	switch (_type)
	{
	case WeaponsManager::SM_MISSILE:
		Load("images/Ordnance/Missile.png");
		_damage = missileDamage;
		_name = "Missile";
		break;
	case WeaponsManager::MED_MISSILE:
		Load("images/Ordnance/Missile2.png");
		_damage = missileDamage*2;
		_name = "Missile";
		break;
	case WeaponsManager::LG_MISSILE:
		Load("images/Ordnance/Missile3.png");
		_damage = missileDamage * 4;
		_name = "Missile";
		break;
	case WeaponsManager::SM_BULLET:
		Load("images/Ordnance/Bullet.png");
		_damage = bulletDamage;
		_name = "Bullet";
		break;
	case WeaponsManager::MED_BULLET:
		Load("images/Ordnance/Bullet2.png");
		_damage = bulletDamage * 2;
		_name = "Bullet";
		break;
	case WeaponsManager::LG_BULLET:
		Load("images/Ordnance/Bullet3.png");
		_damage = bulletDamage * 4;
		_name = "Bullet";
		break;
	case WeaponsManager::SM_BOMB:
		Load("images/Ordnance/Bomb.png");
		_damage = bombDamage;
		_name = "Bomb";
		break;
	case WeaponsManager::MED_BOMB:
		Load("images/Ordnance/Bomb2.png");
		_damage = bombDamage*2;
		_name = "Bomb";
		break;
	case WeaponsManager::LG_BOMB:
		Load("images/Ordnance/Bomb3.png");
		_damage = bombDamage*4;
		_name = "Bomb";
		break;
	case WeaponsManager::SM_LASER:
		_name = "Laser";
		_damage = laserDamage;
		Load("images/Ordnance/Laser.png");
		break;
	case WeaponsManager::MED_LASER:
		Load("images/Ordnance/Laser2.png");
		_damage = laserDamage*2;
		_name = "Laser";
		break;
	case WeaponsManager::LG_LASER:
		Load("images/Ordnance/Laser3.png");
		_damage = laserDamage*4;
		_name = "Laser";
		break;
	case WeaponsManager::TOTAL_WEAPONS:
		//TODO: Should never get here
		break;
	default:
		break;
	}


	assert( IsLoaded() );	//The image should be loaded or we've got a problem

							//Figure out what the center of the missile is, and use the X and Y of this position as the origin
	GetSprite().setOrigin(GetSprite().getGlobalBounds().width / 2,
		GetSprite().getGlobalBounds().height / 2);

	_name += std::to_string(numWeapons);

	numWeapons++;
}

Weapon::~Weapon()
{

}

void Weapon::Draw(sf::RenderWindow& rw)
{
	VisibleGameObject::Draw(rw);
}



void Weapon::Update(const float& elapsedTime)
{
	//Missiles speed up the longer they're on screen
	if (_type == WeaponsManager::SM_MISSILE ||
		_type == WeaponsManager::MED_MISSILE ||
		_type == WeaponsManager::LG_MISSILE)
	{
		SetXVelocity(GetXVelocity() * (1 + elapsedTime));
		SetYVelocity(GetYVelocity() * (1 + elapsedTime));
	}

	//Update location
	GetSprite().move(GetXVelocity() * elapsedTime, GetYVelocity() * elapsedTime);

	//If we've moved out of the screen, then hide the missile and stop updating
	if (GetPosition().y > (Game::SCREEN_HEIGHT + (GetHeight() / 2)) ||
		GetPosition().y < 0.0 - (GetHeight() / 2))
	{
		Game::GetWeaponsManager().HideObject(this);
	}
}

void Weapon::SetXVelocity(const float& newVelocity)
{
	_xVelocity = newVelocity;
	UpdateDirection();

}
void Weapon::SetYVelocity(const float& newVelocity)
{
	_yVelocity = newVelocity;
	UpdateDirection();
}

