#include "stdafx.h"
#include "AircraftEnemy.h"
#include "Game.h"

int AircraftEnemy::numEnemies = 0;

AircraftEnemy::AircraftEnemy(const std::string& image,
							float maxVelocity, float mass, float health, 
							WeaponsManager::WeaponType weaponType,
							WeaponsManager::WeaponType weaponType2,
							AI_Type type) : b_goUp(false), b_goRight(false)
{
	_maxVelocity = maxVelocity/2;
	_mass = mass;
	_health = health;
	_fullHealth = health;
	_weaponType = weaponType;
	_weaponType2 = weaponType2;
	_AI_type = type;

	_stun = 0.0f;
	_autoFire = 0.0f;
	_rateOfFire = 0.0f;
	_rateOfFire2 = 0.0f;

	_type = ControlType::AIEnemy;

	//Load the image
	Load(image);

	assert(IsLoaded());		//If the image wasn't loaded, we've got a problem

							//Figure out what the center of the plane is, and use the X and Y of this position as the origin
	GetSprite().setOrigin(GetSprite().getGlobalBounds().width / 2,
		GetSprite().getGlobalBounds().height / 2);

	_name = "Enemy" + std::to_string(numAircraft);
	numAircraft++;
}

AircraftEnemy::~AircraftEnemy()
{
}

void AircraftEnemy::Update(const float& elapsedTime)
{
	//Check for collisions/damage
	DamageDetection();

	//Movement behaviour
	switch (_AI_type)
	{
	case LEFT_AND_RIGHT:
		MoveLeftAndRight(0, Game::SCREEN_WIDTH);
		break;
	case UP_AND_DOWN:
		MoveUpAndDown(0, Game::SCREEN_HEIGHT);
		break;
	case PEEKABOO_B:
		PeekABoo(BOTTOM);
		break;
	case PEEKABOO_T:
		PeekABoo(TOP);
		break;
	case PEEKABOO_L:
		PeekABoo(LEFT);
		break;
	case PEEKABOO_R:
		PeekABoo(RIGHT);
		break;
	case DIAGONAL_BACK_AND_FORTH:
		MoveLeftAndRight(0, Game::SCREEN_WIDTH);
		MoveUpAndDown(0, Game::SCREEN_HEIGHT);
		break;
	case SAW_SHAPED_LR:
		MoveLeftAndRight(0, Game::SCREEN_WIDTH);
		MoveUpAndDown(Game::SCREEN_HEIGHT/3, 2* Game::SCREEN_HEIGHT / 3);
	case SAW_SHAPED_UD:
		MoveLeftAndRight(Game::SCREEN_WIDTH / 3, 2 * Game::SCREEN_WIDTH / 3);
		MoveUpAndDown(0, Game::SCREEN_HEIGHT);
	}

	//Make sure we're not accelerating too quickly
	EnforceSpeedLimit();

	//Adjust the direction of the image used for the aircraft
	UpdateDirection();

	//Finally, let's move the plane!
	GetSprite().move(_xVelocity * elapsedTime, _yVelocity * elapsedTime);
}

void AircraftEnemy::MoveUpAndDown(int topEdge, int bottomEdge)
{
	//A faster aircraft increments velocity faster
	float velocityIncrement = 1 + (_maxVelocity / Aircraft::VERY_SLOW_AIRCRAFT_SPEED);

	//If we go off the top of the screen, start heading the other way
	if (GetPosition().y < topEdge)
	{
		_yVelocity += velocityIncrement;
		b_goUp = false;
	}
	//If we go off the bottom, start heading the other way
	else if (GetPosition().y > bottomEdge)
	{
		_yVelocity -= velocityIncrement;
		b_goUp = true;
	}
	//If we're somewhere in the screen just continue on
	else
	{
		if (b_goUp)
		{
			_yVelocity -= velocityIncrement;
		}
		else
			_yVelocity += velocityIncrement;
	}
}

void AircraftEnemy::MoveLeftAndRight(int leftEdge, int rightEdge)
{
	//A faster aircraft increments velocity faster
	float velocityIncrement = 1 + (_maxVelocity / Aircraft::VERY_SLOW_AIRCRAFT_SPEED);

	//If we go off the left of the screen, start heading right
	if (GetPosition().x < leftEdge)
	{
		_xVelocity += velocityIncrement;
		b_goRight = true;
	}
	//If we go off the right, start heading left
	else if (GetPosition().x > rightEdge)
	{
		_xVelocity -= velocityIncrement;
		b_goRight = false;
	}
	//If we're somewhere in the screen just continue on
	else
	{
		if (b_goRight)
		{
			_xVelocity += velocityIncrement;
		}
		else
			_xVelocity -= velocityIncrement;
	}
}

void AircraftEnemy::PeekABoo(PeekABooType peekType)
{
	//A faster aircraft increments velocity faster
	float velocityIncrement = 1 + (_maxVelocity / Aircraft::VERY_SLOW_AIRCRAFT_SPEED);

	switch (peekType)
	{
	case TOP:
		//If we go off the top of the screen, start heading the other way
		if (GetPosition().y < 0.0)
		{
			_yVelocity += velocityIncrement;
		}
		else
		{
			_yVelocity -= velocityIncrement;
		}
		break;
	case BOTTOM:
		//If we go off the bottom, start heading the other way
		if (GetPosition().y > Game::SCREEN_HEIGHT)
		{
			_yVelocity -= velocityIncrement;
		}
		else
		{
			_yVelocity += velocityIncrement;
		}
		break;
	case LEFT:
		//If we go off the right, start heading left
		if (GetPosition().x > Game::SCREEN_WIDTH)
		{
			_xVelocity -= velocityIncrement;
		}
		else
		{
			_xVelocity += velocityIncrement;
		}
		break;
	case RIGHT:
		//If we go off the left of the screen, start heading right
		if (GetPosition().x < 0.0)
		{
			_xVelocity += velocityIncrement;
		}
		else
		{
			_xVelocity -= velocityIncrement;
		}
		break;
	}
}

void AircraftEnemy::EnforceSpeedLimit()
{
	//Ensure that aircraft speed stays within speed limits for x and y
	if (_xVelocity > _maxVelocity)
		_xVelocity = _maxVelocity;
	else if (_xVelocity < -_maxVelocity)
		_xVelocity = -_maxVelocity;

	if (_yVelocity > _maxVelocity)
		_yVelocity = _maxVelocity;
	else if (_yVelocity < -_maxVelocity)
		_yVelocity = -_maxVelocity;
}

void AircraftEnemy::FiringRules(const float& elapsedTime)
{

}