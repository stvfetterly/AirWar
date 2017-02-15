#pragma once
#include "VisibleGameObject.h"

class GameObjectManager
{
public:
	GameObjectManager();
	~GameObjectManager();

	void Add(VisibleGameObject* gameObject);
	void Remove(const std::string& name);						//If you wish to remove items during the update function, use the deletion queue
	int GetObjectCount() const;
	VisibleGameObject* Get(const std::string& name) const;
	
	void DrawAll(sf::RenderWindow& renderWindow);
	void UpdateAll();											//Updates all objects, deletes anything added to the deletion queue
	void SetPause(bool pause);
	bool GetPause() { return _paused; }
	void AddToDelQ(const std::string& name);

private:
	std::map<std::string, VisibleGameObject*> _gameObjects;
	sf::Clock clock;
	std::vector<std::string> _deletionQueue;					//holds the names of objects to be deleted at the end of the next update


	bool _paused;

	

	struct GameObjectDeallocator
	{
		void operator()(const std::pair<std::string, VisibleGameObject*>& p)
		{
			delete p.second;
		}
	};
};
