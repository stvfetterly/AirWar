#include "stdafx.h"
#include "Game.h"
#include "Aircraft.h"
#include "EnemyManager.h"

Aircraft* EnemyManager::CreateEnemyPlane(EnemyManager::PlaneType type)
{
	Aircraft* enemy = NULL;
	switch (type)
	{
	case AS2:
		enemy = new Aircraft(Aircraft::AIEnemy, "images/aircraft/enemy/AS2.png", Aircraft::AVG_AIRCRAFT_SPEED, Aircraft::AVG_AIRCRAFT_MASS, 500, WeaponsManager::MED_LASER, WeaponsManager::LG_BOMB);
		break;
	case Dornier335:
		break;
	case F15blue:
		break;
	case F15navy:
		break;
	case F22b:
		break;
	case F35b:
		break;
	case FuturePlane:
		break;
	case SBJ:
		break;
	case SU27:
		break;
	case SU27b:
		break;
	case SU30:
		break;
	case SU47:
		break;
	case Zero:
		break;
	default:
		//TODO - ERROR, should never get here
		break;
	}

	if (NULL != enemy)
	{
		Game::GetGameObjectManager().Add(enemy);
	}
	else
	{
		//TODO - SHOULD NEVER HAVE A NULL PLANE RETURNED
	}

	return enemy;
}