#pragma once
#include "Aircraft.h"

class AircraftPlayer : public Aircraft
{
public:
	AircraftPlayer(const std::string& image, float maxVelocity, float mass, float health, WeaponsManager::WeaponType weaponType, WeaponsManager::WeaponType weaponType2);
	
	void FiringRules(const float& elapsedTime);
	void Update(const float& elapsedTime);			//Updates plane position
};