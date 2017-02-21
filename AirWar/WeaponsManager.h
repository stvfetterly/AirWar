#pragma once
#include "VisibleGameObject.h"
#include "Cloud.h"

class WeaponsManager
{
	
public:
	enum WeaponType
	{
		SM_MISSILE = 0,
		MED_MISSILE = 1,
		LG_MISSILE = 2,
		SM_BULLET = 3,
		MED_BULLET = 4, 
		LG_BULLET = 5,
		SM_BOMB = 6,
		MED_BOMB = 7,
		LG_BOMB = 8,
		SM_LASER = 9,
		MED_LASER = 10, 
		LG_LASER = 11,
		TOTAL_WEAPONS = 12
	};
	
	WeaponsManager();
	~WeaponsManager();

	VisibleGameObject* GetWeapon(WeaponType type);
	void HideObject(VisibleGameObject* objToHide);
	static float GetRateOfFire(const WeaponType& weapon);
	static float GetWeaponSpeed(const WeaponType& weapon);
	Cloud* GetContrail();

private:
	static const int WEAPON_NUM = 30;
	static const int CONTRAIL_NUM = 700;

	std::map<WeaponType, std::vector<std::string>> _weaponNameMap;		//Keeps track of the names of all the weapons available
	std::map<WeaponType, unsigned int> _curWeaponToUse;					//Keeps track of the next weapon to use

	std::vector<Cloud*> _contrailCache;					//Keeps track of all the contrails available for missiles
	unsigned int _curContrailToUse;						//Keeps track of the next contrail to use

	void CreateAllOrdnance();
};