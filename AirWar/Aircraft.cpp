#include "stdafx.h"
#include "Aircraft.h"
#include "Weapon.h"
#include "Game.h"

const float Aircraft::VERY_FAST_FIRING_RATE = 0.1f;
const float Aircraft::FAST_FIRING_RATE = 0.25f;
const float Aircraft::AVG_FIRING_RATE = 0.5f;
const float Aircraft::SLOW_FIRING_RATE = 1.0f;
const float Aircraft::VERY_SLOW_FIRING_RATE = 2.0f;

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
	               WeaponsManager::WeaponType weaponType):
	_stun(0.0f), _autoFire(0.0f), _rateOfFire(AVG_FIRING_RATE), _weaponType(weaponType)
{
	_type = type;
	_maxVelocity = maxVelocity;
	_mass = mass;
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
void Aircraft::Fire(const float& elapsedTime)
{
	//Grab next available missile, set to current location
	VisibleGameObject* missile = Game::GetWeaponsManager().GetWeapon(_weaponType);
	missile->SetPosition(this->GetPosition().x, this->GetPosition().y);
	missile->SetYVelocity(-1200.0f);
}

void Aircraft::Damage(float damageAmount)
{
	_health -= damageAmount;

	if (_health < 0)
		Explode();
}

void Aircraft::Stun(float stunTime)
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
	//RULES FOR FIRING
	if (_rateOfFire < 0.0)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			Fire(elapsedTime);  //Fire upon space bar press
			_rateOfFire = VERY_FAST_FIRING_RATE;
		}
	}
	else
	{
		_rateOfFire -= elapsedTime;
	}


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

	//Ensure that paddle speed stays within speed limits for x and y
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

	//Paddle will bounce off the wall if it gets to the left side
	float leftBound = GetSprite().getLocalBounds().width / 2;
	float rightBound = Game::SCREEN_WIDTH - GetSprite().getLocalBounds().width / 2;
	float upBound = GetSprite().getLocalBounds().height / 2;
	float loBound = Game::SCREEN_HEIGHT - (GetSprite().getLocalBounds().height / 2);

	if (pos.x < leftBound)	//Bounce off left
	{
		this->SetPosition(leftBound, pos.y);
		_xVelocity = -_xVelocity;
	}
	else if (pos.x > rightBound)	//Bounce off right
	{
		this->SetPosition(rightBound, pos.y);
		_xVelocity = -_xVelocity;
	}

	if (pos.y < upBound)	//Bounce off top
	{
		this->SetPosition(pos.x, upBound);
		_yVelocity = -_yVelocity;
	}
	else if (pos.y > loBound)	//Bounce off bottom
	{
		this->SetPosition(pos.x, loBound);
		_yVelocity = -_yVelocity;
	}

	//Move the location of the paddle
	GetSprite().move(_xVelocity * elapsedTime, _yVelocity * elapsedTime);
}
void Aircraft::UpdateAuto(const float& elapsedTime)
{

}