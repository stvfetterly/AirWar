#pragma once
#include "stdafx.h"
#include "GameObjectManager.h"
#include "WeaponsManager.h"
#include "PlaneManager.h"
#include "Aircraft.h"

class Game
{

public:
	const static int SCREEN_WIDTH = 1024;
	const static int SCREEN_HEIGHT = 768;
	const static int MAX_DEGREES = 360;
	const static int NUM_PADDLES = 2;
	
	enum GameDifficulty {Pansy, Wannabe, Hardcore};
	
	static void Start();
	static sf::RenderWindow& GetWindow();
	const static sf::Event GetInput();
	static GameObjectManager& GetGameObjectManager();
	static WeaponsManager& GetWeaponsManager();
	static PlaneManager& GetPlaneManager();

	//Game options
	static bool _music;
	static GameDifficulty _difficulty;

private:
	static bool IsExiting();
	static void GameLoop();

	static void ShowSplashScreen();
	static void ShowMenu();
	static void ShowPlaneSelect();

	enum GameState 
	{
		Uninitialized,
		ShowingSplash,
		Paused,
		ShowingMenu,
		ShowingPlaneSelect,
		Playing,
		Exiting
	};

	static GameState _gameState;
	static sf::RenderWindow _mainWindow;
	static GameObjectManager _gameObjectManager;
	static WeaponsManager _weaponsManager;
	static PlaneManager _planeManager;

	static void AddPlayerAircraft(Aircraft* player);
};
