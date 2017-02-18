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
sf::Sprite Game::_backgroundInv;
float Game::_backgroundImagePixels = Game::SCREEN_HEIGHT;
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
	//assert(_backgroundImage.loadFromFile("Images/Backgrounds/BG-NewYork.png"));
	//assert(_backgroundImage.loadFromFile("Images/Backgrounds/BG-Oahu.png"));
	//assert(_backgroundImage.loadFromFile("Images/Backgrounds/BG-Swamp.png"));
	assert(_backgroundImage.loadFromFile("Images/Backgrounds/BG-Airport.png"));
	//assert(_backgroundImage.loadFromFile("Images/Backgrounds/BG-Siberia.png"));

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
/*
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
	else if (_backgroundImagePixels > _backgroundImage.getSize().y )
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
}*/

void Game::ShowBackground(const float& timeChange)
{
	//Clears all items that we may be trying to draw from _mainwindow
	_mainWindow.clear(sf::Color(0, 0, 0));

	//Location on the background image that we will show this time
	_backgroundImagePixels += (timeChange * 200);

	//Check that the pixels are still on the screen
	if (_backgroundImagePixels > _backgroundImage.getSize().y + Game::SCREEN_HEIGHT)
	{
		_backgroundImagePixels = Game::SCREEN_HEIGHT;	//Show at least one screen
		_backgroundInverted = !_backgroundInverted;		//Time to invert background
	}


	int _backgroundImagePixelsInt = static_cast<int>(std::round(_backgroundImagePixels));

	//Case screen is fully regular:
	if (_backgroundInverted == false && _backgroundImagePixels <= Game::SCREEN_HEIGHT)
	{
		//Top of the regular image: lowest displayable screen height - the increment that has occured
		int regTop = _backgroundImage.getSize().y - Game::SCREEN_HEIGHT - _backgroundImagePixelsInt;

		_background.setTexture(_backgroundImage);
		_background.setTextureRect(sf::IntRect(0, regTop, Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT));
		_background.setPosition(0, 0);
		_mainWindow.draw(_background);
	}
	//Case screen is fully inverted:
	else if (_backgroundInverted == true && _backgroundImagePixels <= Game::SCREEN_HEIGHT)
	{
		//Top of the inverted image just starts at zero and increments by the amount that has occured
		int invTop = 0 + Game::SCREEN_HEIGHT + _backgroundImagePixelsInt;

		_background.setTexture(_backgroundImage);
		_background.setTextureRect(sf::IntRect(0, invTop, Game::SCREEN_WIDTH, -Game::SCREEN_HEIGHT));
		_background.setPosition(0, 0);
		_mainWindow.draw(_background);
	}

	//Case screen is partially reg/inverted - coming from normal
	if (_backgroundInverted == true && 
		_backgroundImagePixels > Game::SCREEN_HEIGHT)
	{
		float invertedPixelHeight = _backgroundImagePixels - Game::SCREEN_HEIGHT;
		float regularPixelHeight = Game::SCREEN_HEIGHT - invertedPixelHeight;
		int invertedPixelHeightInt = static_cast<int>(std::round(invertedPixelHeight));

		//Set textures for the regular and inverted images - we need to draw both
		_background.setTexture(_backgroundImage);
		_backgroundInv.setTexture(_backgroundImage);

		float movingVal = _backgroundImage.getSize().y - SCREEN_HEIGHT - _backgroundImagePixels - SCREEN_HEIGHT;


		//Find the portion of each image to display

		//This is the top of the reg image
		_background.setTextureRect(sf::IntRect(0, 0, SCREEN_WIDTH, static_cast<int>(std::round(regularPixelHeight))));	//Stop at the top of the image

		//This is the moving inverted image to make a matching top for the reg image
		_backgroundInv.setTextureRect(sf::IntRect(0, _backgroundImagePixelsInt - SCREEN_HEIGHT, SCREEN_WIDTH, -invertedPixelHeightInt));
		//_backgroundInv.setTextureRect(sf::IntRect(0, _backgroundImagePixels - _backgroundImage.getSize().y, SCREEN_WIDTH, -invertedPixelHeight));

		//Set location of each texture so that it draws properly on screen
		_background.setPosition(0, Game::SCREEN_HEIGHT - regularPixelHeight);	//Regular stays at bottom until it disappears
		_backgroundInv.setPosition(0, 0);										//Inv starts where the regular ends and keeps scrolling down until it takes over
	
		//Draw them
		_mainWindow.draw(_background);
		_mainWindow.draw(_backgroundInv);
	}

	//Case screen is partially reg/inverted - coming from inverted
	if (_backgroundInverted == false && 
		_backgroundImagePixels > Game::SCREEN_HEIGHT)
	{
		float regularPixelHeight = _backgroundImagePixels - Game::SCREEN_HEIGHT;
		float invertedPixelHeight = Game::SCREEN_HEIGHT - regularPixelHeight;
		int invertedPixelHeightInt = static_cast<int>(std::round(invertedPixelHeight));
		int regularPixelHeightInt = static_cast<int>(std::round(regularPixelHeight));

		//Set textures for the regular and inverted images - we need to draw both
		_background.setTexture(_backgroundImage);
		_backgroundInv.setTexture(_backgroundImage);
		

		//Find the portion of each image to display
		//This is the top of the reg image, it keeps moving to match the lower inverted image
		_background.setTextureRect(sf::IntRect(0, _backgroundImage.getSize().y - (_backgroundImagePixelsInt - SCREEN_HEIGHT), SCREEN_WIDTH, regularPixelHeightInt));

		//This is the moving inverted image to make a matching top for the reg image
		_backgroundInv.setTextureRect(sf::IntRect(0, _backgroundImage.getSize().y, SCREEN_WIDTH, -invertedPixelHeightInt));		//stop at the top of image

		//Set location of each texture so that it draws properly on screen
		_background.setPosition(0, 0);												//Inverted stays at top until it disappears
		_backgroundInv.setPosition(0, Game::SCREEN_HEIGHT - invertedPixelHeight);	//Regular starts where the inv ends and keeps going down until it takes over

		//Draw them
		_mainWindow.draw(_background);
		_mainWindow.draw(_backgroundInv);
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

