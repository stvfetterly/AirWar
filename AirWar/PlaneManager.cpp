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
		plane = new AircraftEnemy("images/aircraft/enemy/AS2.png", Aircraft::SLOW_AIRCRAFT_SPEED, Aircraft::AVG_AIRCRAFT_MASS, 500, WeaponsManager::MED_LASER, WeaponsManager::MED_LASER, AircraftEnemy::UP_AND_DOWN);
		break;
	case Enemy_Dornier335:
		plane = new AircraftEnemy("images/aircraft/enemy/Dornier335.png", Aircraft::AVG_AIRCRAFT_SPEED, Aircraft::AVG_AIRCRAFT_MASS, 50, WeaponsManager::MED_LASER, WeaponsManager::MED_LASER, AircraftEnemy::LEFT_AND_RIGHT);
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
		plane = new AircraftEnemy("images/aircraft/enemy/SBJ.png", Aircraft::FAST_AIRCRAFT_SPEED, Aircraft::VERY_LIGHT_AIRCRAFT_MASS, 20, WeaponsManager::MED_LASER, WeaponsManager::MED_LASER, AircraftEnemy::DIAGONAL_BACK_AND_FORTH);
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
		plane = new AircraftEnemy("images/aircraft/enemy/Zero.png", Aircraft::AVG_AIRCRAFT_SPEED, Aircraft::AVG_AIRCRAFT_MASS, 50, WeaponsManager::MED_LASER, WeaponsManager::MED_LASER, AircraftEnemy::LEFT_AND_RIGHT);
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
	//that was used, and the position
	AircraftPlayer* oldPlane = dynamic_cast<AircraftPlayer*>(Game::GetGameObjectManager().Get("Player"));
	if (oldPlane != NULL)
	{
		float healthPercentage = oldPlane->GetHealth() / oldPlane->GetFullHealth();
		plane->SetHealth(plane->GetFullHealth() * healthPercentage);
		plane->SetPosition(oldPlane->GetPosition().x, oldPlane->GetPosition().y);

		//Remove the old plane
		Game::GetGameObjectManager().Remove("Player");
	}
	//If we're making a new Player plane and the old one is blown up or gone, then set the plane in the middle of the screen
	else if (plane->GetType() == Aircraft::Player)
	{
		plane->SetPosition(Game::SCREEN_WIDTH / 2, Game::SCREEN_HEIGHT / 2);
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

void PlaneManager::CreateWave(PlaneManager::PlaneType type)
{
	switch (type)
	{
		case Enemy_AS2:
			StaggeredTop(Enemy_AS2, 4.0f);
			break;
		case Enemy_Dornier335:
			StaggeredRight(Enemy_Dornier335, 6.0f);
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
			StraightTop(Enemy_SBJ, 7.0);
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
			StaggeredLeft(Enemy_Zero, 5.0f);
			break;
		default:
			//TODO - Error, should never create a wave of planes that aren't enemies
			break;
	}
}

//Creates a staggered wave from the top of the screen
void PlaneManager::StraightTop(PlaneType type, float numberOfPlanes)
{
	for (float i = 1; i < (numberOfPlanes + 1); i++)
	{
		Aircraft* newplane = CreatePlane(type);
		newplane->SetPosition(Game::SCREEN_WIDTH / (numberOfPlanes + 1) * i, newplane->GetHeight());
	}
}

//Creates a staggered wave from the top of the screen
void PlaneManager::StaggeredTop(PlaneType type, float numberOfPlanes)
{
	for (float i = 1; i < (numberOfPlanes + 1); i++)
	{
		Aircraft* newplane = CreatePlane(type);
		newplane->SetPosition(Game::SCREEN_WIDTH / (numberOfPlanes + 1) * i, -(newplane->GetHeight() * i));
	}
}

//Creates a wave from the left
void PlaneManager::StaggeredLeft(PlaneType type, float numberOfPlanes)
{
	for (float i = 1; i < numberOfPlanes + 1; i++)
	{
		Aircraft* newplane = CreatePlane(type);
		newplane->SetPosition(-(newplane->GetWidth() * i * 4), Game::SCREEN_HEIGHT / (numberOfPlanes + 1) * i);
	}
}

//Creates a wave from the rigth
void PlaneManager::StaggeredRight(PlaneType type, float numberOfPlanes)
{
	for (float i = 1; i < numberOfPlanes + 1; i++)
	{
		Aircraft* newplane = CreatePlane(type);
		newplane->SetPosition(Game::SCREEN_WIDTH + (newplane->GetWidth() * i * 4), Game::SCREEN_HEIGHT / (numberOfPlanes + 1) * i);
	}
}

