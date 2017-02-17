#pragma once
#include "Aircraft.h"

class AircraftEnemy : public Aircraft
{
public:
	AircraftEnemy(const std::string& image, float maxVelocity, float mass, float health, WeaponsManager::WeaponType weaponType, WeaponsManager::WeaponType weaponType2);
	~AircraftEnemy();
	void Update(const float& elapsedTime);			//Updates plane position

private:
	static int numEnemies;
	void FiringRules(const float& elapsedTime);		//Rules for firing weapon
};