#pragma once
#include "stdafx.h"
#include "GameObjectManager.h"
#include "WeaponsManager.h"

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
	const static GameObjectManager& GetGameObjectManager();
	static WeaponsManager& GetWeaponsManager();

	//Game options
	static bool _gunFire;
	static bool _music;
	static GameDifficulty _difficulty;
	static bool _constantMotion;

private:
	static bool IsExiting();
	static void GameLoop();

	static void ShowSplashScreen();
	static void ShowMenu();

	enum GameState 
	{
		Uninitialized,
		ShowingSplash,
		Paused,
		ShowingMenu,
		Playing,
		Exiting
	};

	static GameState _gameState;
	static sf::RenderWindow _mainWindow;
	static GameObjectManager _gameObjectManager;
	static WeaponsManager _weaponsManager;
	//static WeaponsManager* _weaponsManager;
//	WeaponsManager* _weaponsManager;
//	WeaponsManager _weaponsManager;

	static void UpdateImages();
};
