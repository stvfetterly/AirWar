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
PlaneManager Game::_planeManager;
sf::Texture Game::_backgroundImage;
sf::Texture Game::_backgroundImage2;
sf::Sprite Game::_background;
float Game::_backgroundImagePixels = 0.0f;
bool Game::_backgroundInverted = false;

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
	
	//Load background image textures
	//Problem if no background found
	assert(_backgroundImage.loadFromFile("Images/Backgrounds/TestBackground3.png"));
	assert(_backgroundImage2.loadFromFile("Images/Backgrounds/TestBackground2.png"));

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

//Shows/updates the background image
void Game::ShowBackground(const float& timeChange)
{
	//Clears all items that we may be trying to draw from _mainwindow
	_mainWindow.clear(sf::Color(0, 0, 0));


	if (_backgroundInverted)
	{
		//Go down the inverted image
		_backgroundImagePixels += (timeChange * 300);
	}
	else
	{
		//Go up the regular image
		_backgroundImagePixels -= (timeChange * 300);
	}


	//Background Pixels go from:
	// size of the picture - screen height = bottom screen of image    to
	// zero, then wrap over to
	// -size of the picture + screen height = top most inversion of image
	if (_backgroundImagePixels < 0.0)
	{
		//We're going to go down from the top of the inverted picture now
		_backgroundInverted = true;

		//Show a screen worth of the inverted image - Get top of screen, add the height of the screen
		_backgroundImagePixels = 0.0 + static_cast<float>(Game::SCREEN_HEIGHT * 2);
	}
	else if (_backgroundImagePixels > _backgroundImage.getSize().y /*+ Game::SCREEN_HEIGHT THIS IS THE MISSING HEIGHT*/)
	{
		//We're going to go up from the bottom of the inverted picture now
		_backgroundInverted = false;

		//Show a screen worth of the regular image - Get bottom of screen, subtract height of screen
		_backgroundImagePixels = _backgroundImage.getSize().y - (static_cast<float>(Game::SCREEN_HEIGHT * 2));
	}

	if (_backgroundInverted)
	{
		//negative game height is passed in to mirror image around x axis
		_background.setTextureRect(sf::IntRect(0, static_cast<int>(_backgroundImagePixels), Game::SCREEN_WIDTH, -Game::SCREEN_HEIGHT));
	}
	else
	{
		_background.setTextureRect(sf::IntRect(0, static_cast<int>(_backgroundImagePixels), Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT));
	}
	_background.setTexture(_backgroundImage);

	//Get ready to draw background image
	_mainWindow.draw(_background);
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

			//Update the positions of objects
			_gameObjectManager.UpdateAll();

			//Get ready to draw the background image
			//ShowBackground();

			//Get ready to draw all moving game objects over the background
			_gameObjectManager.DrawAll(_mainWindow);

			//Draw all game objects over the background
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
		player = _planeManager.CreatePlane(PlaneManager::Player_J31);
		break;
	case PlaneSelectScreen::J20:
		player = _planeManager.CreatePlane(PlaneManager::Player_J20);
		break;
	case PlaneSelectScreen::T50:
		player = _planeManager.CreatePlane(PlaneManager::Player_T50);
		break;
	case PlaneSelectScreen::F35:
		player = _planeManager.CreatePlane(PlaneManager::Player_F35);
		break;
	case PlaneSelectScreen::F22:
		player = _planeManager.CreatePlane(PlaneManager::Player_F22);
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

