#include "stdafx.h"
#include "Aircraft.h"
#include "Weapon.h"
#include "Game.h"
#include "Boom.h"

const float Aircraft::COLLISION_DAMAGE = 10.0f;

const float Aircraft::VERY_FAST_AIRCRAFT_SPEED = 1400.0f;
const float Aircraft::FAST_AIRCRAFT_SPEED = 1200.0f;
const float Aircraft::AVG_AIRCRAFT_SPEED = 1000.0f;
const float Aircraft::SLOW_AIRCRAFT_SPEED = 800.0f;
const float Aircraft::VERY_SLOW_AIRCRAFT_SPEED = 400.0f;

const float Aircraft::VERY_HEAVY_AIRCRAFT_MASS = 10;
const float Aircraft::HEAVY_AIRCRAFT_MASS = 8;
const float Aircraft::AVG_AIRCRAFT_MASS = 6;
const float Aircraft::LIGHT_AIRCRAFT_MASS = 4;
const float Aircraft::VERY_LIGHT_AIRCRAFT_MASS = 2;

int Aircraft::numAircraft = 0;

Aircraft::Aircraft()
{
	_name = "Aircraft" + std::to_string(numAircraft);

	numAircraft++;
}

Aircraft::~Aircraft()
{

}

//Plane actions
void Aircraft::Fire(const float& xVel, const float& yVel, const WeaponsManager::WeaponType& weaponType)
{
	//Grab next available weapon, set to current location
	Weapon* weapon = dynamic_cast<Weapon*>(Game::GetWeaponsManager().GetWeapon(weaponType));

	if (weapon != NULL)
	{
		weapon->SetPosition(this->GetPosition().x, this->GetPosition().y);				//Weapon fires from the center of plane
		weapon->SetXVelocity(xVel);
		weapon->SetYVelocity(yVel);

		if (_type == Player)
			weapon->SetFiredFrom(Weapon::PLAYER);
		else
			weapon->SetFiredFrom(Weapon::ENEMY);
	}
	else
	{
		//TODO - Error handling, should never get anything but a weapon from the weapons handler
	}
}

void Aircraft::Damage(const float& damageAmount)
{
	_fullHealth -= damageAmount;

	if (_fullHealth < 0)
		Explode();
}

void Aircraft::Stun(const float& stunTime)
{
	_stun = stunTime;
}

void Aircraft::Explode()
{
	//Queue this aircraft to be removed from the game
	Game::GetGameObjectManager().AddToDeletionQueue(_name);
	
	//Add the boom
	Boom* explodePlane = new Boom();
	explodePlane->SetPosition(GetPosition().x, GetPosition().y);
	Game::GetGameObjectManager().Add(explodePlane);
}

bool Aircraft::IsStunned(const float& elapsedTime)
{
	//If the plane is stunned, do not allow movement but update the counter
	if (_stun > 0)
	{
		_stun -= elapsedTime;
		return false;
	}

	return true;
}

void Aircraft::Update(const float& elapsedTime)
{
	//Update function is handled by various AIs
}

void Aircraft::FiringRules(const float& elapsedTime)
{

}

void Aircraft::DamageDetection()
{
	//Get the collision list for this plane
	std::vector<VisibleGameObject*> collisionList = Game::GetGameObjectManager().GetCollisionList(this->GetBoundingRect());

	for (auto itr = collisionList.begin(); itr < collisionList.end(); ++itr)
	{
		//Check for weapons
		Weapon* weapon = dynamic_cast<Weapon*> (*itr);
		if (weapon != NULL)
		{
			//we have a weapon, now let's see if it's an enemy weapon
			if ((weapon->GetFiredFrom() == Weapon::ENEMY && (_type == Player || _type == AIWingman)) ||
				(weapon->GetFiredFrom() == Weapon::PLAYER && (_type == AIEnemy)							  ))	//Player and wingman can be hurt by ENEMY
			{
				//Deal damage from the weapon
				Damage(weapon->GetDamage());

				//After a collision, weapon should disappear
				Game::GetWeaponsManager().HideObject(weapon);
			}
		}
		//Check for aircraft
		else
		{
			Aircraft* aircraft = dynamic_cast<Aircraft*> (*itr);

			if (aircraft != NULL)
			{
				//we have collided with an aircraft, now let's see if it's an enemy aircraft
				if (aircraft->GetType() == AIEnemy && _type == Player ||
					aircraft->GetType() == Player && _type == AIEnemy)
				{
					//Deal damage to both planes for the collision
					aircraft->Damage(COLLISION_DAMAGE);
					Damage(COLLISION_DAMAGE);
				}
			}
		}
	}
}