#include "stdafx.h"
#include "Weapon.h"
#include "ServiceLocator.h"
#include "Paddle.h"
#include "Game.h"

int Weapon::numWeapons = 0;

Weapon::Weapon(float xVel, float yVel, WeaponsManager::WeaponType type) : _xVelocity(xVel), _yVelocity(yVel), _damage(0.0), _type(type)
{
	const float missileDamage = 25;
	const float bombDamage = 50;
	const float bulletDamage = 10;
	const float laserDamage = 15;


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
	numWeapons--;
}

void Weapon::Draw(sf::RenderWindow& rw)
{
	VisibleGameObject::Draw(rw);
}

void Weapon::Update(const float& elapsedTime)
{
	//Check if a collision with the opponent paddle has occured
//	Paddle* paddle;

	//If we're moving up, then we should be checking for a collision
	/*	if (_velocity < 0)
	{
	paddle = dynamic_cast<Paddle*>(Game::GetGameObjectManager().Get("Paddle2"));
	}
	else //Otherwise we're going down looking for paddle 1 collision
	{
	paddle = dynamic_cast<Paddle*>(Game::GetGameObjectManager().Get("Paddle1"));
	}

	//If the paddle was found
	if (paddle != NULL)
	{
	sf::Rect<float> PaddleRect = paddle->GetBoundingRect();
	sf::Rect<float> LaserRect = GetBoundingRect();

	//Check if the laser has hit the paddle
	if (PaddleRect.intersects(LaserRect))
	{
	//If the paddle has been hit, set it's velocity to zero
	paddle->Stun();

	//hide the laser
	SetPosition(Game::SCREEN_WIDTH * 2, Game::SCREEN_HEIGHT * 2);
	return;
	}
	}
	else
	{
	//TODO: Error handling, we should have found a paddle by now
	}*/


	//Update location
	GetSprite().move(GetXVelocity() * elapsedTime, GetYVelocity() * elapsedTime);
/*
	//If we've moved out of the screen, then delete the rocket
	if (GetPosition().y > (Game::SCREEN_HEIGHT + (GetHeight() / 2)) ||
		GetPosition().y < 0.0 - (GetHeight() / 2))
	{
		const_cast<GameObjectManager&>(Game::GetGameObjectManager()).AddToDelQ(GetName());
	}
	*/

	//If we've moved out of the screen, then hide the missile
}