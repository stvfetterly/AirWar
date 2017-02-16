#include "stdafx.h"
#include "Game.h"
#include "MainMenuScreen.h"
#include "SplashScreen.h"
#include "SFMLSoundProvider.h"
#include "ServiceLocator.h"
#include "Aircraft.h"
#include "PlaneSelectScreen.h"

//Initialize static variables
Game::GameState Game::_gameState = Uninitialized;
sf::RenderWindow Game::_mainWindow;
GameObjectManager Game::_gameObjectManager;
WeaponsManager Game::_weaponsManager;

bool Game::_music = true;
Game::GameDifficulty Game::_difficulty = Game::Wannabe;



//Starts the game
void Game::Start(void)
{
	if (_gameState != Uninitialized)
		//TODO: error handling for this state - should always start as uninitialized
		return;

	//Create the sound service and save it in the register
	SFMLSoundProvider soundProvider;
	ServiceLocator::RegisterServiceLocator(&soundProvider);

	//Creates main window with specified resolution, 32 bit colour, and a title of AirWar!
	_mainWindow.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32), "AirWar!");
	
	//Create an enemy plane for testing				
	Aircraft* enemy = new Aircraft(Aircraft::AIEnemy, "images/aircraft/enemy/AS2.png", Aircraft::AVG_AIRCRAFT_SPEED, Aircraft::AVG_AIRCRAFT_MASS, 500, WeaponsManager::MED_LASER, WeaponsManager::LG_BOMB);
	enemy->SetPosition(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4);
	_gameObjectManager.Add(enemy);



	//Pause all game objects so they don't start moving while at the splash screen
	_gameObjectManager.SetPause(true);

	_gameState = Game::ShowingSplash;

	//Continue to loop the game until an exit command is recieved
	while (!IsExiting())
	{
		GameLoop();
	}

	_mainWindow.close();
}

//Variable is set when user wants to exit the game
bool Game::IsExiting()
{
	if (_gameState == Game::Exiting)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//This is where everything happens in the game
void Game::GameLoop()
{
	sf::Event currentEvent;
	_mainWindow.pollEvent(currentEvent);  //Check if an event has occurred

	if (_music)
	{
		//Ensure that the game music is always playing if enabled
	//	ServiceLocator::GetAudio()->PlayMusic();
	}
	else
	{
		//turn off the music
		if (ServiceLocator::GetAudio()->IsSongPlaying())
		{
			ServiceLocator::GetAudio()->StopMusic();
		}
	}

	//Determine what actions should take place based on state of game
	switch (_gameState)
	{
		//User is playing the game
		case Game::Playing:
			//Clears screen to black
			_mainWindow.clear(sf::Color(0, 0, 0));

			//Update the positions of objects
			_gameObjectManager.UpdateAll();

			//Update display
			_gameObjectManager.DrawAll(_mainWindow);

			//Displays the screen
			_mainWindow.display();

			//If we get an exiting event, close the game
			if (currentEvent.type == sf::Event::Closed)
			{
				_gameState = Game::Exiting;
			}

			//If the user presses esc, back out to the menu
			if (currentEvent.type == sf::Event::KeyPressed)
			{
				if (currentEvent.key.code == sf::Keyboard::Escape)
				{
					ShowMenu();
				}
			}

			//If the user presses P or Pause/Break or Enter, pause the game
			if (currentEvent.type == sf::Event::KeyPressed)
			{
 				if (currentEvent.key.code == sf::Keyboard::P     ||
					currentEvent.key.code == sf::Keyboard::Pause ||
					currentEvent.key.code == sf::Keyboard::Return)
				{
					_gameObjectManager.SetPause(!_gameObjectManager.GetPause());
				}
			}
			break;

		//show the main menu
		case Game::ShowingMenu:
			_gameObjectManager.SetPause(true);
			ShowMenu();
			break;
		//Show the splash screen
		case Game::ShowingSplash:
			_gameObjectManager.SetPause(true);
			ShowSplashScreen();
			break;
		case Game::ShowingPlaneSelect:
			_gameObjectManager.SetPause(true);
	}
}

void Game::ShowSplashScreen()
{
	_gameState = Game::ShowingSplash;
	SplashScreen splashScreen;
	splashScreen.Show(_mainWindow);
	
	//After user has clicked on splash screen, go to main menu
	_gameState = Game::ShowingMenu;
}

void Game::ShowPlaneSelect()
{
	_gameState = Game::ShowingPlaneSelect;
	PlaneSelectScreen planeSelect;
	PlaneSelectScreen::MenuResult result = planeSelect.Show(_mainWindow);

	Aircraft* player;
	switch (result)
	{
	case PlaneSelectScreen::J31:
		player = new Aircraft(Aircraft::Player, "images/aircraft/player/J31Yellow.png", Aircraft::VERY_FAST_AIRCRAFT_SPEED, Aircraft::VERY_LIGHT_AIRCRAFT_MASS, 500, WeaponsManager::SM_BULLET, WeaponsManager::SM_BOMB);
		AddPlayerAircraft(player);
		break;
	case PlaneSelectScreen::J20:
		player = new Aircraft(Aircraft::Player, "images/aircraft/player/J20Pink.png", Aircraft::VERY_SLOW_AIRCRAFT_SPEED, Aircraft::VERY_HEAVY_AIRCRAFT_MASS, 500, WeaponsManager::MED_MISSILE, WeaponsManager::LG_BOMB);
		AddPlayerAircraft(player);
		break;
	case PlaneSelectScreen::T50:
		player = new Aircraft(Aircraft::Player, "images/aircraft/player/T50.png", Aircraft::VERY_FAST_AIRCRAFT_SPEED, Aircraft::VERY_HEAVY_AIRCRAFT_MASS, 500, WeaponsManager::LG_LASER, WeaponsManager::LG_BOMB);
		AddPlayerAircraft(player);
		break;
	case PlaneSelectScreen::F35:
		player = new Aircraft(Aircraft::Player, "images/aircraft/player/F35red.png", Aircraft::AVG_AIRCRAFT_SPEED, Aircraft::AVG_AIRCRAFT_MASS, 500, WeaponsManager::LG_BULLET, WeaponsManager::MED_BOMB);
		AddPlayerAircraft(player);
		break;
	case PlaneSelectScreen::F22:
		player = new Aircraft(Aircraft::Player, "images/aircraft/player/F22orange.png", Aircraft::FAST_AIRCRAFT_SPEED, Aircraft::LIGHT_AIRCRAFT_MASS, 500, WeaponsManager::SM_MISSILE, WeaponsManager::SM_BOMB);
		AddPlayerAircraft(player);
		break;
	case PlaneSelectScreen::MainMenu:
		ShowMenu();
		break;
	case PlaneSelectScreen::Exit:
		_gameState = Game::Exiting;
		break;
	default:
		//Clicked on a part of the screen that does nothing - do nothing!
		break;
	}
}

void Game::AddPlayerAircraft(Aircraft* player)
{
	//Set the name to 'Player' for fast lookup later!
	player->SetName("Player");

	//Start player in middle of screen
	player->SetPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	//If the player has gone back to the main menu to select a different plane, 
	//update the health of this new plane to match the percentage of the previous one
	Aircraft* oldPlane = dynamic_cast<Aircraft*>(_gameObjectManager.Get("Player"));
	if (oldPlane != NULL)
	{
		float healthPercentage = oldPlane->GetHealth() / oldPlane->GetFullHealth();
		player->SetHealth(player->GetFullHealth() * healthPercentage);

		//Remove the old plane
		_gameObjectManager.Remove("Player");
	}

	_gameObjectManager.Add(player);

	//Plane selected, let's start playing the game!
	_gameState = Game::Playing;

	//Start all the visible objects moving!
	_gameObjectManager.SetPause(false);
}

//Displays main menu, allows user to play game or exit
void Game::ShowMenu()
{
	_gameState = Game::ShowingMenu;
	MainMenu mainMenu;
	MainMenu::MenuResult result = mainMenu.Show(_mainWindow);

	switch (result)
	{
	case MainMenu::Play:
		ShowPlaneSelect();
		break;
		//handles selected options from the menu
	case MainMenu::OptMusic:
		_music = !_music;
		break;
	case MainMenu::OptSound:
		ServiceLocator::GetAudio()->MuteSounds(!ServiceLocator::GetAudio()->IsSoundMuted());
		break;
	case MainMenu::OptDifPansy:
		_difficulty = Pansy;
		break;
	case MainMenu::OptDifWannabe:
		_difficulty = Wannabe;
		break;
	case MainMenu::OptDifHard:
		_difficulty = Hardcore;
		break;
	case MainMenu::Splash:
		ShowSplashScreen();
		break;
	case MainMenu::Exit:
		_gameState = Game::Exiting;
		break;
	default:
		//Clicked on a part of the screen that does nothing - do nothing!
		break;
	}
}

//Get events from the game (keyboard inputs)
const sf::Event Game::GetInput()
{
	sf::Event currentEvent;
	_mainWindow.pollEvent(currentEvent);
	return currentEvent;
}

GameObjectManager& Game::GetGameObjectManager()
{
	return Game::_gameObjectManager;
}

WeaponsManager& Game::GetWeaponsManager()
{
	return Game::_weaponsManager;
}

