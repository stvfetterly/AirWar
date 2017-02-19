#pragma once
#include "stdafx.h"
#include "GameObjectManager.h"
#include "WeaponsManager.h"
#include "PlaneManager.h"
#include "BackgroundManager.h"
#include "Aircraft.h"

class Game
{

public:
	const static int SCREEN_WIDTH = 1024;
	const static int SCREEN_HEIGHT = 768;
	const static int MAX_DEGREES = 360;

	enum GameDifficulty { Pansy, Wannabe, Hardcore };

	static void Start();
	static sf::RenderWindow& GetWindow() { return _mainWindow; }
	const static sf::Event GetInput();
	static GameObjectManager& GetGameObjectManager() { return _gameObjectManager; }
	static WeaponsManager& GetWeaponsManager() { return _weaponsManager; }
	static PlaneManager& GetPlaneManager() { return _planeManager; }
	static BackgroundManager& GetBackgroundManager() { return _backgroundManager; }

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
	static BackgroundManager _backgroundManager;
};
