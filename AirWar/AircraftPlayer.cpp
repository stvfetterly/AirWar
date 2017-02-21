#include "stdafx.h"
#include "AircraftPlayer.h"
#include "Game.h"

AircraftPlayer::AircraftPlayer(const std::string& image,
	float maxVelocity, float mass, float health,
	WeaponsManager::WeaponType weaponType,
	WeaponsManager::WeaponType weaponType2)
{
	_maxVelocity = maxVelocity;
	_mass = mass;
	_health = health;
	_fullHealth = health;
	_weaponType = weaponType;
	_weaponType2 = weaponType2;
	
	_stun = 0.0f;
	_autoFire = 0.0f;
	_rateOfFire = 0.0f;
	_rateOfFire2 = 0.0f;

	_type = ControlType::Player;

	//Load the image
	Load(image);

	assert(IsLoaded());		//If the image wasn't loaded, we've got a problem

							//Figure out what the center of the plane is, and use the X and Y of this position as the origin
	GetSprite().setOrigin(GetSprite().getGlobalBounds().width / 2,
		GetSprite().getGlobalBounds().height / 2);

	_name = "ZPlayer";

	//Set player to appear in middle of screen
	SetPosition(Game::SCREEN_WIDTH / 2, Game::SCREEN_HEIGHT / 2);
}

void AircraftPlayer::Update(const float& elapsedTime)
{
	FiringRules(elapsedTime);

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

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		_xVelocity += velocityIncrement;	//Right makes the airplane move right
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		_yVelocity += velocityIncrement;	//Down makes the airplane move down
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
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

void AircraftPlayer::FiringRules(const float& elapsedTime)
{
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