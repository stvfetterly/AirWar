#pragma once
#include "VisibleGameObject.h"

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

private:
	static const int WEAPON_NUM = 40;

	std::map<WeaponType, std::vector<std::string>> _weaponNameMap;		//Keeps track of the names of all the weapons available
	std::map<WeaponType, unsigned int> _curWeaponToUse;					//Keeps track of the next weapon to use

	void CreateAllOrdnance();
};