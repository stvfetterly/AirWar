#include "stdafx.h"
#include "Game.h"
#include "AircraftEnemy.h"
#include "AircraftPlayer.h"
#include "PlaneManager.h"

Aircraft* PlaneManager::CreatePlane(PlaneManager::PlaneType type)
{
	Aircraft* plane = NULL;
	switch (type)
	{
	case Enemy_AS2:
		plane = new AircraftEnemy("images/aircraft/enemy/AS2.png", Aircraft::AVG_AIRCRAFT_SPEED, Aircraft::AVG_AIRCRAFT_MASS, 500, WeaponsManager::MED_LASER, WeaponsManager::LG_BOMB);
		break;
	case Enemy_Dornier335:
		break;
	case Enemy_F15blue:
		break;
	case Enemy_F15navy:
		break;
	case Enemy_F22b:
		break;
	case Enemy_F35b:
		break;
	case Enemy_FuturePlane:
		break;
	case Enemy_SBJ:
		break;
	case Enemy_SU27:
		break;
	case Enemy_SU27b:
		break;
	case Enemy_SU30:
		break;
	case Enemy_SU47:
		break;
	case Enemy_Zero:
		break;
	case Player_J31:
		plane = new AircraftPlayer("images/aircraft/player/J31Yellow.png", Aircraft::VERY_FAST_AIRCRAFT_SPEED, Aircraft::VERY_LIGHT_AIRCRAFT_MASS, 500, WeaponsManager::SM_BULLET, WeaponsManager::SM_BOMB);
		break;
	case Player_J20:
		plane = new AircraftPlayer("images/aircraft/player/J20Pink.png", Aircraft::VERY_SLOW_AIRCRAFT_SPEED, Aircraft::VERY_HEAVY_AIRCRAFT_MASS, 500, WeaponsManager::MED_MISSILE, WeaponsManager::LG_BOMB);
		break;
	case Player_T50:
		plane = new AircraftPlayer("images/aircraft/player/T50.png", Aircraft::VERY_FAST_AIRCRAFT_SPEED, Aircraft::VERY_HEAVY_AIRCRAFT_MASS, 500, WeaponsManager::LG_LASER, WeaponsManager::LG_BOMB);
		break;
	case Player_F22:
		plane = new AircraftPlayer("images/aircraft/player/F35red.png", Aircraft::AVG_AIRCRAFT_SPEED, Aircraft::AVG_AIRCRAFT_MASS, 500, WeaponsManager::LG_BULLET, WeaponsManager::MED_BOMB);
		break;
	case Player_F35:
		plane = new AircraftPlayer("images/aircraft/player/F22orange.png", Aircraft::FAST_AIRCRAFT_SPEED, Aircraft::LIGHT_AIRCRAFT_MASS, 500, WeaponsManager::SM_MISSILE, WeaponsManager::SM_BOMB);
		break;
	default:
		//TODO - ERROR, should never get here
		break;
	}

	//If the player has gone back to the main menu to select a different plane, 
	//update the health of this new plane to match the percentage of the previous one
	//that was used
	AircraftPlayer* oldPlane = dynamic_cast<AircraftPlayer*>(Game::GetGameObjectManager().Get("Player"));
	if (oldPlane != NULL)
	{
		float healthPercentage = oldPlane->GetHealth() / oldPlane->GetFullHealth();
		plane->SetHealth(plane->GetFullHealth() * healthPercentage);

		//Remove the old plane
		Game::GetGameObjectManager().Remove("Player");
	}

	if (NULL != plane)
	{
		Game::GetGameObjectManager().Add(plane);
	}
	else
	{
		//TODO - SHOULD NEVER HAVE A NULL PLANE RETURNED
	}

	return plane;
}