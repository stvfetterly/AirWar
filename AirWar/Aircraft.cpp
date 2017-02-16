#include "stdafx.h"
#include "Aircraft.h"
#include "Weapon.h"
#include "Game.h"

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

Aircraft::Aircraft(ControlType type, 
				   const std::string& image,
	               float maxVelocity,
	               float mass, 
	               float health,
	               WeaponsManager::WeaponType weaponType,
				   WeaponsManager::WeaponType weaponType2):
	_stun(0.0f), _autoFire(0.0f), _rateOfFire(0.0f), _rateOfFire2(0.0f), 
	_weaponType(weaponType), _weaponType2(weaponType2)
{
	_type = type;
	_maxVelocity = maxVelocity;
	_mass = mass;
	_fullHealth = health;
	_health = health;

	//Load the image
	Load(image);

	assert(IsLoaded());		//If the image wasn't loaded, we've got a problem

							//Figure out what the center of the paddle is, and use the X and Y of this position as the origin
	GetSprite().setOrigin(GetSprite().getGlobalBounds().width / 2,
		GetSprite().getGlobalBounds().height / 2);

	_name = "Aircraft" + std::to_string(numAircraft);

	numAircraft++;
}

Aircraft::~Aircraft()
{
	numAircraft--;
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
	//If this aircraft is player controlled, restart game and return to menu

	//Otherwise remove this aircraft from the game
}

void Aircraft::Update(const float& elapsedTime)
{
	//If the plane is stunned, do not allow movement
	if (_stun > 0)
	{
		_stun -= elapsedTime;
	}
	else
	{
		//Automatic fire from the AI
		if (_type == Aircraft::AIEnemy || _type == Aircraft::AIWingman)
		{
			_autoFire -= elapsedTime;
			if (_autoFire < 0)
			{
				//Fire();
			}

			UpdateAuto(elapsedTime);
		}
		else   //Update location of manual paddle
		{
			UpdateManual(elapsedTime);
		}
	}
}

void Aircraft::UpdateManual(const float& elapsedTime)
{
	ManualFiring(elapsedTime);

	//RULES FOR MANUAL MOTION

	//Bigger planes take longer to slow than small planes.
	float brakeRate = (VERY_HEAVY_AIRCRAFT_MASS / _mass) / 4;

	const float ignoreVelocicity = 0.5f;

	//if the user isn't moving the plane, the plane will slow down.
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
		!sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		if (_xVelocity > ignoreVelocicity)
			_xVelocity -= brakeRate;
		else if (_xVelocity < -ignoreVelocicity)
			_xVelocity += brakeRate;
		else
			_xVelocity = 0.0;
	}

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
		!sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		if (_yVelocity > ignoreVelocicity)
			_yVelocity -= brakeRate;
		else if (_yVelocity < -ignoreVelocicity)
			_yVelocity += brakeRate;
		else
			_yVelocity = 0.0;
	}

	//A faster aircraft increments velocity faster
	float velocityIncrement = 1 + (_maxVelocity / Aircraft::VERY_SLOW_AIRCRAFT_SPEED);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		_xVelocity -= velocityIncrement;		//Left makes the airplane move left
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		_xVelocity += velocityIncrement;	//Right makes the airplane move right
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		_yVelocity += velocityIncrement;	//Down makes the airplane move down
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		_yVelocity -= velocityIncrement;	//Up makes the airplane move up
	}

	//Ensure that aircraft speed stays within speed limits for x and y
	if (_xVelocity > _maxVelocity)
		_xVelocity = _maxVelocity;
	else if (_xVelocity < -_maxVelocity)
		_xVelocity = -_maxVelocity;

	if (_yVelocity > _maxVelocity)
		_yVelocity = _maxVelocity;
	else if (_yVelocity < -_maxVelocity)
		_yVelocity = -_maxVelocity;


	//KEEP THE MANUAL PLANE ON SCREEN BY BOUNCING OFF EDGE
	sf::Vector2f pos = this->GetPosition();

	//Aircraft will bounce off the wall if it gets to any edge
	float leftBound = GetSprite().getLocalBounds().width / 2;
	float rightBound = Game::SCREEN_WIDTH - GetSprite().getLocalBounds().width / 2;
	float upBound = GetSprite().getLocalBounds().height / 2;
	float loBound = Game::SCREEN_HEIGHT - (GetSprite().getLocalBounds().height / 2);

	if (pos.x < leftBound)	//Bounce off left
	{
		this->SetPosition(leftBound, pos.y);
		_xVelocity = -_xVelocity / 2;
	}
	else if (pos.x > rightBound)	//Bounce off right
	{
		this->SetPosition(rightBound, pos.y);
		_xVelocity = -_xVelocity / 2;
	}

	if (pos.y < upBound)	//Bounce off top
	{
		this->SetPosition(pos.x, upBound);
		_yVelocity = -_yVelocity / 2;
	}
	else if (pos.y > loBound)	//Bounce off bottom
	{
		this->SetPosition(pos.x, loBound);
		_yVelocity = -_yVelocity / 2;
	}

	//Move the location of the plane
	GetSprite().move(_xVelocity * elapsedTime, _yVelocity * elapsedTime);

	//Detect if damage has been taken
	DamageDetection();
}
void Aircraft::ManualFiring(const float& elapsedTime)
{
	//RULES FOR FIRING

	//Primary fire - Space
	if (_rateOfFire <= 0.0)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			//Player primary weapon always fires UP, so yVel is negative
			Fire(0.0, -WeaponsManager::GetWeaponSpeed(_weaponType), _weaponType);
			_rateOfFire = WeaponsManager::GetRateOfFire(_weaponType);
		}
	}
	else
	{
		_rateOfFire -= elapsedTime;
	}

	//Secondary Fire - Alt
	if (_rateOfFire2 <= 0.0)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt))
		{
			//Player secondary weapon always fires DOWN, so yVel is negative
			Fire(0.0, WeaponsManager::GetWeaponSpeed(_weaponType2), _weaponType2);
			_rateOfFire2 = WeaponsManager::GetRateOfFire(_weaponType2);
		}
	}
	else
	{
		_rateOfFire2 -= elapsedTime;
	}
}

void Aircraft::DamageDetection()
{
	//Get the collision list for this plane
	std::vector<VisibleGameObject*> collisionList = Game::GetGameObjectManager().CollisionList(this->GetBoundingRect());

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


void Aircraft::UpdateAuto(const float& elapsedTime)
{
	DamageDetection();
}