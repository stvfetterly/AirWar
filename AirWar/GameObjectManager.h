#pragma once
#include "VisibleGameObject.h"

class GameObjectManager
{
public:
	GameObjectManager();
	~GameObjectManager();

	void Add(VisibleGameObject* gameObject);
	void AddDecoration(VisibleGameObject* gameObject);				//Holds and handles purely decorative objects
	void Remove(const std::string& name);							//Note - do not call from inside update loop, add the object to the deletion queue instead		
	void RemoveDecoration(const std::string& name);					//Note - do not call from inside update loop, add the object to the decoration deletion queue instead		

	VisibleGameObject* Get(const std::string& name) const;
	VisibleGameObject* GetDecoration(const std::string& name) const;
	
	void DrawAll(sf::RenderWindow& renderWindow);
	void UpdateAll();											
	void SetPause(bool pause);
	bool GetPause() { return _paused; }

	std::vector<VisibleGameObject*> GetCollisionList(const sf::Rect<float>& objRect);
	void AddToDeletionQueue(const std::string& newItem) { _deletionQueue.push_back(newItem); }
	void AddToDeletionDecorationQueue(const std::string& newItem) { _deletionDecorationQueue.push_back(newItem); }

	std::map<std::string, sf::Texture>& GetObjectTextureMap() { return _textureMap; }
	void CacheTexture(const std::string& filename);

private:
	std::map<std::string, VisibleGameObject*> _gameObjects;
	std::map<std::string, VisibleGameObject*> _decorationObjects;
	std::map<std::string, sf::Texture> _textureMap;		//Keeps a hashmap of textures to prevent re-loading of images for speeding up performance of game objects

	std::vector<std::string> _deletionQueue;				//Used to hold a list of objects that should be deleted after update
	std::vector<std::string> _deletionDecorationQueue;
	sf::Clock clock;
	
	bool _paused;

	struct GameObjectDeallocator
	{
		void operator()(const std::pair<std::string, VisibleGameObject*>& p)
		{
			delete p.second;
		}
	};
};
