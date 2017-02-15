#pragma once

#include "VisibleGameObject.h"
#include "WeaponsManager.h"

class Weapon : public VisibleGameObject
{
public:
	Weapon(float xVel, float yVel, WeaponsManager::WeaponType type);
	~Weapon();

	void Update(const float& elapsedTime);
	void Draw(sf::RenderWindow& rw);

	float GetDamage() { return _damage; }
	void SetDamage(const float& newDamage) { _damage = newDamage; }


private:
	float _xVelocity;      //positive = down, negative = up
	float _yVelocity;
	float _damage;			//Amount of damage that a weapon will do
	static int numWeapons;

	WeaponsManager::WeaponType _type;

};