#include "stdafx.h"
#include "WeaponsManager.h"
#include "Game.h"
#include "Weapon.h"


WeaponsManager::WeaponsManager()
{
	CreateAllOrdnance();
}

WeaponsManager::~WeaponsManager()
{

}

VisibleGameObject* WeaponsManager::GetWeapon(WeaponType type)
{
	if (_weaponNameMap[type].size() == 0 )
	{
		//TODO - We should report an error if no weapons have been created and we're requesting one
		return NULL;
	}
	else
	{
		unsigned int curAvailableWeapon = _curWeaponToUse[type];

		//Looks up the name of the currently available weapon from the object manager and returns it
		VisibleGameObject* returnObj = Game::GetGameObjectManager().Get(_weaponNameMap[type][curAvailableWeapon]);

		//Update the next available weapon
		curAvailableWeapon++;
		if (curAvailableWeapon == _weaponNameMap[type].size())
		{
			curAvailableWeapon = 0;
		}
		_curWeaponToUse[type] = curAvailableWeapon;

		//unpause the object
		returnObj->Pause(false);

		//make object visible
		returnObj->SetVisible(true);

		return returnObj;
	}
}
void WeaponsManager::HideObject(VisibleGameObject* objToHide)
{
	//Hide the object well off screen
	objToHide->SetPosition(Game::SCREEN_WIDTH + objToHide->GetWidth(), Game::SCREEN_HEIGHT + objToHide->GetHeight());

	//Pause the object so it doesn't waste time trying to update
	objToHide->Pause(true);

	//Make object invisible
	objToHide->SetVisible(false);
}

void  WeaponsManager::CreateAllOrdnance()
{
	//Create a large pool of all types of of weapons for later use
	for (int i = 0; i < WEAPON_NUM; i++)
	{
		//For each type of weapon, make an object
		for (int j = 0; j < TOTAL_WEAPONS; j++)
		{
			Weapon* newWeapon = new Weapon(0.0f, 0.0f, static_cast<WeaponType>(j));				//Create the new weapon
			HideObject(newWeapon);										//Move the weapon off screen and pause it
			_weaponNameMap[static_cast<WeaponType>(j)].push_back(newWeapon->GetName());	//Add the name of the weapon to our list

			//Add the weapon to the object manager
			const_cast<GameObjectManager&> (Game::GetGameObjectManager()).Add(newWeapon);
		}
	}
}

float WeaponsManager::GetRateOfFire(const WeaponType& weapon)
{
	switch (weapon)
	{
	case WeaponsManager::SM_MISSILE:
		return 0.15f;
	case WeaponsManager::MED_MISSILE:
		return 0.22f;
	case WeaponsManager::LG_MISSILE:
		return 0.28f;
	case WeaponsManager::SM_BULLET:
		return 0.025f;
	case WeaponsManager::MED_BULLET:
		return 0.1f;
	case WeaponsManager::LG_BULLET:
		return 0.2f;
	case WeaponsManager::SM_BOMB:
		return 0.1f;
	case WeaponsManager::MED_BOMB:
		return 0.25f;
	case WeaponsManager::LG_BOMB:
		return 0.5f;
	case WeaponsManager::SM_LASER:
		return 0.1f;
	case WeaponsManager::MED_LASER:
		return 0.28f;
	case WeaponsManager::LG_LASER:
		return 0.3f;
	case WeaponsManager::TOTAL_WEAPONS:
		//TODO: Should never get here
		break;
	default:
		break;
	}
		
	return 0.0f;
}

float WeaponsManager::GetWeaponSpeed(const WeaponType& weapon)
{
	switch (weapon)
	{
	case WeaponsManager::SM_MISSILE:
		return 1100.0f;
	case WeaponsManager::MED_MISSILE:
		return 1000.0f;
	case WeaponsManager::LG_MISSILE:
		return 900.0f;
	case WeaponsManager::SM_BULLET:
		return 1200.0f;
	case WeaponsManager::MED_BULLET:
		return 1200.0f;
	case WeaponsManager::LG_BULLET:
		return 1200.0f;
	case WeaponsManager::SM_BOMB:
		return 1000.0f;
	case WeaponsManager::MED_BOMB:
		return 900.0f;
	case WeaponsManager::LG_BOMB:
		return 800.0f;
	case WeaponsManager::SM_LASER:
		return 1200.0f;
	case WeaponsManager::MED_LASER:
		return 1100.0f;
	case WeaponsManager::LG_LASER:
		return 1000.0f;
	case WeaponsManager::TOTAL_WEAPONS:
		//TODO: Should never get here
		break;
	default:
		break;
	}

	return 0.0f;
}