#pragma once
#include "VisibleGameObject.h"
#include "WeaponsManager.h"

class Aircraft : public VisibleGameObject
{
public:
	const static float COLLISION_DAMAGE;

	const static float VERY_FAST_AIRCRAFT_SPEED;
	const static float FAST_AIRCRAFT_SPEED;
	const static float AVG_AIRCRAFT_SPEED;
	const static float SLOW_AIRCRAFT_SPEED;
	const static float VERY_SLOW_AIRCRAFT_SPEED;

	const static float VERY_HEAVY_AIRCRAFT_MASS;
	const static float HEAVY_AIRCRAFT_MASS;
	const static float AVG_AIRCRAFT_MASS;
	const static float LIGHT_AIRCRAFT_MASS;
	const static float VERY_LIGHT_AIRCRAFT_MASS;

	enum ControlType { Player, AIEnemy, AIWingman };

	Aircraft();
	virtual ~Aircraft();

	ControlType GetType() { return _type; }

	//Plane actions
	void Fire(const float& xVel, const float& yVel, const WeaponsManager::WeaponType& weaponType);
	void Damage(const float& damageAmount);
	void Stun(const float& stunTime);
	bool IsStunned(const float& elapsedTime);
	void Explode();
	virtual void Update(const float& elapsedTime);			//Updates plane position

	float GetHealth() { return _health; }
	float GetFullHealth() { return _fullHealth; }
	void SetHealth(float health) { _health = health; }

protected:
	virtual void FiringRules(const float& elapsedTime);
	void DamageDetection();

	static int numAircraft;

	float _maxVelocity;			//maximum speed of this aircraft
	float _stun;				//keeps track of amount of time left until plane can move again
	float _autoFire;			//keeps track of amount of time left until AI should fire again
	float _fullHealth;			//Total amount of health for the aircraft
	float _health;				//Amount of health left for the aircraft
	float _mass;				//Used to calculate the steering precision
	float _rateOfFire;			//Used to set the rate of fire for primary weapon
	float _rateOfFire2;			//Used to set the rate of fire for secondary weapon

	ControlType _type;			//is this plane AI, or user controlled

	WeaponsManager::WeaponType _weaponType;		//Keeps track of the pirmary type of weapon that this plane is using
	WeaponsManager::WeaponType _weaponType2;	//Keeps track of the secondary type of weapon that this plane is using
};