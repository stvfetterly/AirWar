#pragma once
#include "VisibleGameObject.h"
#include "WeaponsManager.h"

class Aircraft : public VisibleGameObject
{
public:
	static int numAircraft;

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

	const static float VERY_FAST_FIRING_RATE;
	const static float FAST_FIRING_RATE;
	const static float AVG_FIRING_RATE;
	const static float SLOW_FIRING_RATE;
	const static float VERY_SLOW_FIRING_RATE;

	enum ControlType { Player, AIEnemy, AIWingman };
	
	Aircraft(ControlType type, const std::string& image, float maxVelocity, float mass, float health, WeaponsManager::WeaponType weaponType);

	virtual ~Aircraft();

	ControlType GetType() { return _type; }

	//Plane actions
	virtual void Fire(const float& elapsedTime);
	void Damage(float damageAmount);
	void Stun(float stunTime);
	void Explode();
	void Update(const float& elapsedTime);			//Updates plane position

private:
	float _maxVelocity;		//maximum speed of this aircraft
	float _stun;			//keeps track of amount of time left until plane can move again
	float _autoFire;		//keeps track of amount of time left until AI should fire again
	float _health;			//Total amount of health for the aircraft
	float _mass;			//Used to calculate the steering precision
	float _rateOfFire;		//Used to set the rate of fire

	ControlType _type;		//is this plane AI, or user controlled
	WeaponsManager::WeaponType _weaponType;		//Keeps track of the type of weapon that this plane is using

	void UpdateManual(const float& elapsedTime);
	virtual void UpdateAuto(const float& elapsedTime);
};