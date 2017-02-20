#include "stdafx.h"
#include "GameObjectManager.h"
#include "Game.h"
#include "BackgroundManager.h"


GameObjectManager::GameObjectManager() : _paused(false)
{
	//Cache all object textures at startup to avoid slowdowns later on during the game
	
	//Checkboxes
	CacheTexture("images/Menu/checkChecked.png");
	CacheTexture("images/Menu/checkEmpty.png");

	//Clouds
	CacheTexture("images/Clouds/Cloud1.png");
	CacheTexture("images/Clouds/Cloud2.png");
	CacheTexture("images/Clouds/Cloud3.png");
	CacheTexture("images/Clouds/Cloud4.png");
	CacheTexture("images/Clouds/Cloud5.png");
	CacheTexture("images/Clouds/Cloud6.png");
	CacheTexture("images/Clouds/Cloud7.png");
	CacheTexture("images/Clouds/Cloud8.png");
	CacheTexture("images/Clouds/Cloud9.png");
	CacheTexture("images/Clouds/Cloud10.png");
	CacheTexture("images/Clouds/Cloud11.png");

	//Booms
	CacheTexture("images/Booms/Boom1.png");
	CacheTexture("images/Booms/Boom2.png");
	CacheTexture("images/Booms/Boom3.png");
	CacheTexture("images/Booms/Boom4.png");
	CacheTexture("images/Booms/Boom5.png");
	CacheTexture("images/Booms/Boom6.png");
	CacheTexture("images/Booms/Boom7.png");
	CacheTexture("images/Booms/Boom8.png");
	CacheTexture("images/Booms/BoomB1.png");
	CacheTexture("images/Booms/BoomB2.png");
	CacheTexture("images/Booms/BoomB3.png");
	CacheTexture("images/Booms/BoomB4.png");
	CacheTexture("images/Booms/BoomB5.png");
	CacheTexture("images/Booms/BoomB6.png");
	CacheTexture("images/Booms/BoomC1.png");
	CacheTexture("images/Booms/BoomC2.png");
	CacheTexture("images/Booms/BoomC3.png");
	CacheTexture("images/Booms/BoomC4.png");
	CacheTexture("images/Booms/BoomC5.png");
	CacheTexture("images/Booms/BoomC6.png");
	CacheTexture("images/Booms/BoomC7.png");
	CacheTexture("images/Booms/BoomC8.png");
	CacheTexture("images/Booms/BoomC9.png");
	CacheTexture("images/Booms/BoomD1.png");
	CacheTexture("images/Booms/BoomD2.png");
	CacheTexture("images/Booms/BoomD3.png");
	CacheTexture("images/Booms/BoomD4.png");
	CacheTexture("images/Booms/BoomD5.png");
	CacheTexture("images/Booms/BoomD6.png");
	CacheTexture("images/Booms/BoomD7.png");
	CacheTexture("images/Booms/BoomD8.png");
	CacheTexture("images/Booms/BoomD9.png");
	CacheTexture("images/Booms/BoomD10.png");
	CacheTexture("images/Booms/BoomD11.png");
	CacheTexture("images/Booms/BoomE1.png");
	CacheTexture("images/Booms/BoomE2.png");
	CacheTexture("images/Booms/BoomE3.png");
	CacheTexture("images/Booms/BoomE4.png");
	CacheTexture("images/Booms/BoomE5.png");
	CacheTexture("images/Booms/BoomE6.png");
	CacheTexture("images/Booms/BoomE7.png");
	CacheTexture("images/Booms/BoomE8.png");
	CacheTexture("images/Booms/BoomE9.png");
	CacheTexture("images/Booms/BoomE10.png");

	//Planes
	CacheTexture("images/aircraft/enemy/AS2.png");
	CacheTexture("images/aircraft/enemy/Dornier335.png");
	CacheTexture("images/aircraft/enemy/F15blue.png");
	CacheTexture("images/aircraft/enemy/F15navy.png");
	CacheTexture("images/aircraft/enemy/F22b.png");
	CacheTexture("images/aircraft/enemy/F35b.png");
	CacheTexture("images/aircraft/enemy/FuturePlane.png");
	CacheTexture("images/aircraft/enemy/SBJ.png");
	CacheTexture("images/aircraft/enemy/SU27.png");
	CacheTexture("images/aircraft/enemy/Su27b.png");
	CacheTexture("images/aircraft/enemy/SU30.png");
	CacheTexture("images/aircraft/enemy/SU47.png");
	CacheTexture("images/aircraft/enemy/Zero.png");
	CacheTexture("images/aircraft/player/F22orange.png");
	CacheTexture("images/aircraft/player/F35red.png");
	CacheTexture("images/aircraft/player/J20Pink.png");
	CacheTexture("images/aircraft/player/J31Yellow.png");
	CacheTexture("images/aircraft/player/T50.png");

	//Icons
	CacheTexture("Images/Icons/HealthBar.png");

	//Weapons
	CacheTexture("Images/Ordnance/Bomb.png");
	CacheTexture("Images/Ordnance/Bomb2.png");
	CacheTexture("Images/Ordnance/Bomb3.png");
	CacheTexture("Images/Ordnance/Bullet.png");
	CacheTexture("Images/Ordnance/Bullet2.png");
	CacheTexture("Images/Ordnance/Bullet3.png");
	CacheTexture("Images/Ordnance/Laser.png");
	CacheTexture("Images/Ordnance/Laser2.png");
	CacheTexture("Images/Ordnance/Laser3.png");
	CacheTexture("Images/Ordnance/Missile.png");
	CacheTexture("Images/Ordnance/Missile2.png");
	CacheTexture("Images/Ordnance/Missile3.png");
}
GameObjectManager::~GameObjectManager()
{
	std::for_each(_gameObjects.begin(), _gameObjects.end(), GameObjectDeallocator());
}

void GameObjectManager::CacheTexture(const std::string& filename)
{
	sf::Texture tmpTexture;

	assert(tmpTexture.loadFromFile(filename));
	_textureMap[filename] = tmpTexture;
}

void GameObjectManager::Add(VisibleGameObject* gameObject)
{
	_gameObjects.insert(std::pair<std::string, VisibleGameObject*>(gameObject->GetName(), gameObject));
}

//Deletes item from map if found
void GameObjectManager::Remove(const std::string& name)
{
	std::map<std::string, VisibleGameObject*>::iterator results = _gameObjects.find(name);
	
	//If we've found the item to remove, remove it
	if (results != _gameObjects.end())
	{
		delete results->second;
		_gameObjects.erase(results);
	}
}

//Return the object associated with the given name, if not found return NULL
VisibleGameObject* GameObjectManager::Get(const std::string& name) const
{
	std::map<std::string, VisibleGameObject*>::const_iterator results = _gameObjects.find(name);
	
	//if we've failed to find the object, return NULL
	if (results == _gameObjects.end())
	{
		return NULL;
	}

	return results->second;
}

int GameObjectManager::GetObjectCount() const
{
	return _gameObjects.size();
}

//Updates the display of items being managed
void GameObjectManager::DrawAll(sf::RenderWindow& renderWindow)
{
	for (auto itr = _gameObjects.begin(); itr != _gameObjects.end(); ++itr)
	{
		//Don't waste time drawing stuff that's invisible
		if (itr->second->IsVisible())
		{
			itr->second->Draw(renderWindow);
		}
	}
}

//Updates time for each item
void GameObjectManager::UpdateAll()
{
	//finds the amount of time since last frame
	float timeDelta = clock.restart().asSeconds();

	for (auto itr = _gameObjects.begin(); itr != _gameObjects.end(); ++itr)
	{
		//Only update items if they aren't paused
		if (itr->second->IsPaused() == false)
		{
			itr->second->Update(timeDelta);
		}
	}

	//Check the deletion queue
	if (_deletionQueue.size() > 0)
	{
		//remove each item queued up to be deleted when it's safe to do so
		for (auto itr = _deletionQueue.begin(); itr < _deletionQueue.end(); ++itr)
		{
			Remove(*itr);
		}
		_deletionQueue.clear();
	}

	if (!_paused)
	{
		PlaneManager::Update(timeDelta);
		Game::GetBackgroundManager().Update(timeDelta);
	}
}

void GameObjectManager::SetPause(bool pause)
{
	if (_paused != pause)
	{
		//Pause/Unpause everything on screen!
		for (auto itr = _gameObjects.begin(); itr != _gameObjects.end(); ++itr)
		{
			if (itr->second->IsVisible())
			{
				itr->second->Pause(pause);
			}
		}

		//record that we've paused the game so we don't need to loop through everything again
		_paused = pause;
	}
}

//Returns a list of all intersecting objects with a given rectangle
std::vector<VisibleGameObject*> GameObjectManager::GetCollisionList(const sf::Rect<float>& objRect)
{
	std::vector<VisibleGameObject*> collisionList;

	for (auto itr = _gameObjects.begin(); itr != _gameObjects.end(); ++itr)
	{
		if (  itr->second->IsPaused() == false						&&	//Only check items if they aren't paused
			  objRect.intersects(itr->second->GetBoundingRect())	)	//Only record items if they intersect with the given rectangle	
		{
			collisionList.push_back(itr->second);
		}
	}

	return collisionList;
}
