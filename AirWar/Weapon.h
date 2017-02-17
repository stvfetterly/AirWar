#pragma once

#include "VisibleGameObject.h"
#include "WeaponsManager.h"

class Weapon : public VisibleGameObject
{
public:
	enum FiredFrom {PLAYER, ENEMY};

	Weapon(float xVel, float yVel, WeaponsManager::WeaponType type);
	~Weapon();

	void Update(const float& elapsedTime);
	void Draw(sf::RenderWindow& rw);

	float GetDamage() { return _damage; }
	void SetDamage(const float& newDamage) { _damage = newDamage; }

	void SetXVelocity(const float& newVelocity);
	void SetYVelocity(const float& newVelocity);

	void SetFiredFrom(const FiredFrom& newVal) { _firedFrom = newVal; }
	FiredFrom GetFiredFrom() { return _firedFrom; }

	WeaponsManager::WeaponType GetType() { return _type; }

private:
	float _damage;			//Amount of damage that a weapon will do
	static int numWeapons;
	WeaponsManager::WeaponType _type;
	FiredFrom _firedFrom;

};