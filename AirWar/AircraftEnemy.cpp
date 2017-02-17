#include "stdafx.h"
#include "AircraftEnemy.h"
#include "Game.h"

int AircraftEnemy::numEnemies = 0;

AircraftEnemy::AircraftEnemy(const std::string& image,
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
	DamageDetection();
}

void AircraftEnemy::FiringRules(const float& elapsedTime)
{

}