#include "stdafx.h"
#include "Game.h"
#include "AircraftEnemy.h"
#include "AircraftPlayer.h"
#include "PlaneManager.h"
#include "HealthBar.h"

bool PlaneManager::_isPlayerCreated = false;
float PlaneManager::_timeToNextWave = 0.0f;
const float PlaneManager::WAVE_INTERVAL = 15.0f;

Aircraft* PlaneManager::CreatePlane(PlaneManager::PlaneType type)
{
	Aircraft* plane = NULL;
	switch (type)
	{
	case Enemy_AS2:
		plane = new AircraftEnemy("images/aircraft/enemy/AS2.png", Aircraft::VERY_SLOW_AIRCRAFT_SPEED, Aircraft::AVG_AIRCRAFT_MASS, 500, WeaponsManager::MED_LASER, WeaponsManager::MED_BOMB, AircraftEnemy::UP_AND_DOWN);
		break;
	case Enemy_Dornier335:
		plane = new AircraftEnemy("images/aircraft/enemy/Dornier335.png", Aircraft::AVG_AIRCRAFT_SPEED, Aircraft::AVG_AIRCRAFT_MASS, 50, WeaponsManager::SM_BULLET, WeaponsManager::SM_BOMB, AircraftEnemy::LEFT_AND_RIGHT);
		break;
	case Enemy_F15blue:
		plane = new AircraftEnemy("images/aircraft/enemy/F15blue.png", Aircraft::SLOW_AIRCRAFT_SPEED, Aircraft::AVG_AIRCRAFT_MASS, 100, WeaponsManager::MED_BULLET, WeaponsManager::MED_BOMB, AircraftEnemy::SAW_SHAPED_LR);
		break;
	case Enemy_F15navy:
		plane = new AircraftEnemy("images/aircraft/enemy/F15navy.png", Aircraft::SLOW_AIRCRAFT_SPEED, Aircraft::AVG_AIRCRAFT_MASS, 100, WeaponsManager::MED_BULLET, WeaponsManager::MED_BOMB, AircraftEnemy::SAW_SHAPED_UD);
		break;
	case Enemy_F22b:
		plane = new AircraftEnemy("images/aircraft/enemy/F22b.png", Aircraft::SLOW_AIRCRAFT_SPEED, Aircraft::AVG_AIRCRAFT_MASS, 150, WeaponsManager::SM_MISSILE, WeaponsManager::SM_BOMB, AircraftEnemy::SAW_SHAPED_LR);
		break;
	case Enemy_F35b:
		plane = new AircraftEnemy("images/aircraft/enemy/F35b.png", Aircraft::SLOW_AIRCRAFT_SPEED, Aircraft::AVG_AIRCRAFT_MASS, 175, WeaponsManager::LG_BULLET, WeaponsManager::MED_BOMB, AircraftEnemy::SAW_SHAPED_LR);
		break;
	case Enemy_FuturePlane:
		plane = new AircraftEnemy("images/aircraft/enemy/FuturePlane.png", Aircraft::VERY_FAST_AIRCRAFT_SPEED, Aircraft::AVG_AIRCRAFT_MASS, 500, WeaponsManager::LG_LASER, WeaponsManager::LG_MISSILE, AircraftEnemy::PEEKABOO_B);
		break;
	case Enemy_SBJ:
		plane = new AircraftEnemy("images/aircraft/enemy/SBJ.png", Aircraft::AVG_AIRCRAFT_SPEED, Aircraft::VERY_LIGHT_AIRCRAFT_MASS, 20, WeaponsManager::SM_LASER, WeaponsManager::SM_BULLET, AircraftEnemy::DIAGONAL_BACK_AND_FORTH);
		break;
	case Enemy_SU27:
		plane = new AircraftEnemy("images/aircraft/enemy/SU27.png", Aircraft::VERY_FAST_AIRCRAFT_SPEED, Aircraft::VERY_LIGHT_AIRCRAFT_MASS, 220, WeaponsManager::MED_MISSILE, WeaponsManager::MED_BOMB, AircraftEnemy::PEEKABOO_T);
		break;
	case Enemy_SU27b:
		plane = new AircraftEnemy("images/aircraft/enemy/SU27b.png", Aircraft::VERY_FAST_AIRCRAFT_SPEED, Aircraft::VERY_LIGHT_AIRCRAFT_MASS, 245, WeaponsManager::SM_MISSILE, WeaponsManager::LG_BOMB, AircraftEnemy::PEEKABOO_R);
		break;
	case Enemy_SU30:
		plane = new AircraftEnemy("images/aircraft/enemy/SU30.png", Aircraft::VERY_FAST_AIRCRAFT_SPEED, Aircraft::VERY_LIGHT_AIRCRAFT_MASS, 250, WeaponsManager::SM_MISSILE, WeaponsManager::LG_BOMB, AircraftEnemy::PEEKABOO_L);
		break;
	case Enemy_SU47:
		plane = new AircraftEnemy("images/aircraft/enemy/SU47.png", Aircraft::VERY_FAST_AIRCRAFT_SPEED, Aircraft::VERY_LIGHT_AIRCRAFT_MASS, 600, WeaponsManager::SM_LASER, WeaponsManager::SM_LASER, AircraftEnemy::DIAGONAL_BACK_AND_FORTH);
		break;
	case Enemy_Zero:
		plane = new AircraftEnemy("images/aircraft/enemy/Zero.png", Aircraft::AVG_AIRCRAFT_SPEED, Aircraft::AVG_AIRCRAFT_MASS, 50, WeaponsManager::MED_BULLET, WeaponsManager::SM_BOMB, AircraftEnemy::LEFT_AND_RIGHT);
		break;
	case Player_J31:
		plane = new AircraftPlayer("images/aircraft/player/J31Yellow.png", Aircraft::VERY_FAST_AIRCRAFT_SPEED, Aircraft::VERY_LIGHT_AIRCRAFT_MASS, 600, WeaponsManager::SM_BULLET, WeaponsManager::SM_BOMB);
		break;
	case Player_J20:
		plane = new AircraftPlayer("images/aircraft/player/J20Pink.png", Aircraft::VERY_SLOW_AIRCRAFT_SPEED, Aircraft::VERY_HEAVY_AIRCRAFT_MASS, 1500, WeaponsManager::MED_MISSILE, WeaponsManager::LG_BOMB);
		break;
	case Player_T50:
		plane = new AircraftPlayer("images/aircraft/player/T50.png", Aircraft::VERY_FAST_AIRCRAFT_SPEED, Aircraft::VERY_HEAVY_AIRCRAFT_MASS, 850, WeaponsManager::LG_LASER, WeaponsManager::LG_BOMB);
		break;
	case Player_F22:
		plane = new AircraftPlayer("images/aircraft/player/F22orange.png", Aircraft::FAST_AIRCRAFT_SPEED, Aircraft::LIGHT_AIRCRAFT_MASS, 700, WeaponsManager::SM_MISSILE, WeaponsManager::SM_BOMB);
		break;
	case Player_F35:
		plane = new AircraftPlayer("images/aircraft/player/F35red.png", Aircraft::AVG_AIRCRAFT_SPEED, Aircraft::AVG_AIRCRAFT_MASS, 1000, WeaponsManager::LG_BULLET, WeaponsManager::MED_BOMB);
		break;
	default:
		//TODO - ERROR, should never get here
		break;
	}

	if (plane->GetType() == Aircraft::Player)
	{
		//If the player has gone back to the main menu to select a different plane, 
		//update the health of this new plane to match the percentage of the previous one
		//that was used, and the position
		AircraftPlayer* oldPlane = dynamic_cast<AircraftPlayer*>(Game::GetGameObjectManager().Get("ZPlayer"));
		if (oldPlane != NULL)
		{
			float healthPercentage = oldPlane->GetHealth() / oldPlane->GetFullHealth();
			plane->SetHealth(plane->GetFullHealth() * healthPercentage);
			plane->SetPosition(oldPlane->GetPosition().x, oldPlane->GetPosition().y);

			//Remove the old plane
			Game::GetGameObjectManager().Remove("ZPlayer");
		}
		//If we're making a new Player plane and the old one is blown up or gone, then set the plane in the middle of the screen
		else
		{
			plane->SetPosition(Game::SCREEN_WIDTH / 2, Game::SCREEN_HEIGHT / 2);
			_isPlayerCreated = true;
		}

		//Make a health bar
		HealthBar* playerHealth = new HealthBar("PlayerHealth");

		//Set it to the middle, bottom of screen
		playerHealth->SetPosition(Game::SCREEN_WIDTH / 2, Game::SCREEN_HEIGHT - (playerHealth->GetHeight() * 2));

		playerHealth->AddToObjectManager();
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

void PlaneManager::CreateWave()
{
	//C11 style random number between 0 and the start of the player controlled planes
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist(0.0f, static_cast<float>(Player_J31));

	PlaneType type = static_cast<PlaneType>(static_cast<int>(dist(mt)));
	
	CreateWave(type);
}

void PlaneManager::CreateWave(PlaneManager::PlaneType type)
{
	switch (type)
	{
		case Enemy_AS2:
			StaggeredTop(type, 4.0f);
			break;
		case Enemy_Dornier335:
			StaggeredRight(type, 6.0f);
			break;
		case Enemy_F15blue:
			StaggeredLeft(type, 2);
			StaggeredRight(type, 2);
			break;
		case Enemy_F15navy:
			StaggeredLeft(type, 2);
			StaggeredRight(type, 2);
			break;
		case Enemy_F22b:
			StraightTop(type, 4.0f);
			break;
		case Enemy_F35b:
			StaggeredTop(type, 3.0f);
			break;
		case Enemy_FuturePlane:
			StraightTop(type, 2.0f);
			break;
		case Enemy_SBJ:
			StraightTop(type, 7.0);
			break;
		case Enemy_SU27:
			StaggeredTop(type, 5.0f);
			break;
		case Enemy_SU27b:
			StaggeredLeft(type, 3.0f);
			break;
		case Enemy_SU30:
			StaggeredRight(type, 3.0f);
			break;
		case Enemy_SU47:
			StraightBottom(type, 1.0f);
			StraightTop(type, 2.0f);
			break;
		case Enemy_Zero:
			StaggeredLeft(Enemy_Zero, 5.0f);
			break;
		default:
			//TODO - Error, should never create a wave of planes that aren't enemies
			break;
	}
}

//Creates a wave straight from the top
void PlaneManager::StraightTop(PlaneType type, float numberOfPlanes)
{
	for (float i = 1; i < (numberOfPlanes + 1); i++)
	{
		Aircraft* newplane = CreatePlane(type);
		newplane->SetPosition(Game::SCREEN_WIDTH / (numberOfPlanes + 1) * i, - newplane->GetHeight());
	}
}

//Creates a staggered wave striaght from the bottom
void PlaneManager::StraightBottom(PlaneType type, float numberOfPlanes)
{
	for (float i = 1; i < (numberOfPlanes + 1); i++)
	{
		Aircraft* newplane = CreatePlane(type);
		newplane->SetPosition(Game::SCREEN_WIDTH / (numberOfPlanes + 1) * i, Game::SCREEN_HEIGHT + newplane->GetHeight());
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

//Creates a staggred wave from the left
void PlaneManager::StaggeredLeft(PlaneType type, float numberOfPlanes)
{
	for (float i = 1; i < numberOfPlanes + 1; i++)
	{
		Aircraft* newplane = CreatePlane(type);
		newplane->SetPosition(-(newplane->GetWidth() * i * 4), Game::SCREEN_HEIGHT / (numberOfPlanes + 1) * i);
	}
}

//Creates a staggred wave from the rigth
void PlaneManager::StaggeredRight(PlaneType type, float numberOfPlanes)
{
	for (float i = 1; i < numberOfPlanes + 1; i++)
	{
		Aircraft* newplane = CreatePlane(type);
		newplane->SetPosition(Game::SCREEN_WIDTH + (newplane->GetWidth() * i * 4), Game::SCREEN_HEIGHT / (numberOfPlanes + 1) * i);
	}
}

void PlaneManager::Update(const float& elapsedTime)
{
	//Only create more enemy waves if a player exists!
	if (isPlayerCreated())
	{
		//Check if a new wave should be launched
		if (_timeToNextWave <= 0.0)
		{
			//Launch wave!
			PlaneManager::CreateWave();
			_timeToNextWave = WAVE_INTERVAL;
		}
		else
		{
			_timeToNextWave -= elapsedTime;
		}
	}
}

