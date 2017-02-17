#pragma once
#include "VisibleGameObject.h"

class GameObjectManager
{
public:
	GameObjectManager();
	~GameObjectManager();

	void Add(VisibleGameObject* gameObject);
	void Remove(const std::string& name);							//Note - do not call from inside update loop, add the object to the deletion queue instead		
	int GetObjectCount() const;
	VisibleGameObject* Get(const std::string& name) const;
	
	void DrawAll(sf::RenderWindow& renderWindow);
	void UpdateAll();											
	void SetPause(bool pause);
	bool GetPause() { return _paused; }

	std::vector<VisibleGameObject*> GetCollisionList(const sf::Rect<float>& objRect);
	void AddToDeletionQueue(const std::string& newItem) { _deletionQueue.push_back(newItem); }

private:
	const static float WAVE_INTERVAL;
	
	std::map<std::string, VisibleGameObject*> _gameObjects;
	std::vector<std::string> _deletionQueue;				//Used to hold a list of objects that should be deleted after update
	sf::Clock clock;
	
	float _timeToNextWave;

	bool _paused;

	struct GameObjectDeallocator
	{
		void operator()(const std::pair<std::string, VisibleGameObject*>& p)
		{
			delete p.second;
		}
	};
};
