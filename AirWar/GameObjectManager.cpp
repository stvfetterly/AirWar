#include "stdafx.h"
#include "GameObjectManager.h"
#include "Game.h"

GameObjectManager::GameObjectManager() : _paused(false)
{
}
GameObjectManager::~GameObjectManager()
{
	for (auto itr = _gameObjects.begin(); itr != _gameObjects.end(); ++itr)
	{
		//itr.GameObjectDeallocator();
	}
	std::for_each(_gameObjects.begin(), _gameObjects.end(), GameObjectDeallocator());
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
