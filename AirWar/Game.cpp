#include "stdafx.h"
#include "Game.h"
#include "MainMenuScreen.h"
#include "SplashScreen.h"
#include "SFMLSoundProvider.h"
#include "ServiceLocator.h"
#include "Aircraft.h"
//#include "WeaponsManager.h"

//Initialize static variables
Game::GameState Game::_gameState = Uninitialized;
sf::RenderWindow Game::_mainWindow;
GameObjectManager Game::_gameObjectManager;
WeaponsManager Game::_weaponsManager;

bool Game::_gunFire = true;
bool Game::_music = true;
Game::GameDifficulty Game::_difficulty = Game::Wannabe;
bool Game::_constantMotion = true;



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
	



/*	//Create score boards
	Score* score1 = new Score();
	Score* score2 = new Score();
	score1->SetPosition((1024 / 2), 740);		//Scores go in the middle of the screen below the paddles
	score2->SetPosition((1024 / 2), 28);
	_gameObjectManager.Add("Score1", score1);
	_gameObjectManager.Add("Score2", score2);
	*/

/*	//Creates paddles
	Paddle* player1 = new Paddle(Paddle::Manual);									//player 1 is the lower paddle
	Paddle* player2 = new Paddle(Paddle::Auto);										//player 2 is the upper paddle
	player1->SetPosition((1024 / 2), static_cast<const float>(Paddle::LOW_Y_POS));	//Paddle1 in the middle, bottom
	player2->SetPosition((1024 / 2), static_cast<const float>(Paddle::TOP_Y_POS));	//Paddle2 in the middle, top
	_gameObjectManager.Add("Paddle1", player1);
	_gameObjectManager.Add("Paddle2", player2);*/
	
/*	//Create the ball
	GameBall* ball = new GameBall();
	ball->SetPosition((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2) - 15);
	_gameObjectManager.Add("Ball", ball);
	*/


	//Create very fast, very light player aircraft
	Aircraft* player1 = new Aircraft(Aircraft::Player, "images/aircraft/J31Yellow.png", Aircraft::VERY_FAST_AIRCRAFT_SPEED, Aircraft::VERY_LIGHT_AIRCRAFT_MASS, 500, WeaponsManager::SM_BULLET);
	
	//Create very slow, heavy player aircraft
	Aircraft* player1b = new Aircraft(Aircraft::Player, "images/aircraft/J20Pink.png", Aircraft::VERY_SLOW_AIRCRAFT_SPEED, Aircraft::VERY_HEAVY_AIRCRAFT_MASS, 500, WeaponsManager::LG_MISSILE);

	//Create normal speed, normal weight aircraft
	Aircraft* player1c = new Aircraft(Aircraft::Player, "images/aircraft/F35red.png", Aircraft::AVG_AIRCRAFT_SPEED, Aircraft::AVG_AIRCRAFT_MASS, 500, WeaponsManager::MED_MISSILE);

	//Create very heavy, very fast aircraft
	Aircraft* player1d = new Aircraft(Aircraft::Player, "images/aircraft/T50.png", Aircraft::VERY_FAST_AIRCRAFT_SPEED, Aircraft::VERY_HEAVY_AIRCRAFT_MASS, 500, WeaponsManager::LG_LASER);

	//Create light, fast aircraft
	Aircraft* player1e = new Aircraft(Aircraft::Player, "images/aircraft/F22orange.png", Aircraft::FAST_AIRCRAFT_SPEED, Aircraft::LIGHT_AIRCRAFT_MASS, 500, WeaponsManager::MED_BOMB);

	player1->SetPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	player1b->SetPosition(SCREEN_WIDTH / 3, SCREEN_HEIGHT / 2);
	player1c->SetPosition(SCREEN_WIDTH / 3 * 2, SCREEN_HEIGHT / 2);
	player1d->SetPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3);
	player1e->SetPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3 * 2);

	_gameObjectManager.Add(player1);
	_gameObjectManager.Add(player1b);
	_gameObjectManager.Add(player1c);
	_gameObjectManager.Add(player1d);
	_gameObjectManager.Add(player1e);

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
					_gameState = Game::ShowingMenu;
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

/*			//Space fires the laser for user controlled paddle
			if (currentEvent.type == sf::Event::KeyPressed)
			{
				if (currentEvent.key.code == sf::Keyboard::Space)
				{
					Paddle* firingPaddle = dynamic_cast<Paddle*>( _gameObjectManager.Get("Paddle1") );
					if (firingPaddle != NULL)
					{
						firingPaddle->FireLaser();
					}
					else
					{
						//TODO - Error handling, can't find paddle!
					}
				}
			}*/
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
	}
}

void Game::ShowSplashScreen()
{
	SplashScreen splashScreen;
	splashScreen.Show(_mainWindow);
	_gameState = Game::ShowingMenu;
}

//Displays main menu, allows user to play game or exit
void Game::ShowMenu()
{
	MainMenu mainMenu;
	MainMenu::MenuResult result = mainMenu.Show(_mainWindow);

	switch (result)
	{
	case MainMenu::Play:
		_gameState = Game::Playing;
		//Start all the objects moving!
		_gameObjectManager.SetPause(false);
		break;
		//handles selected options from the menu
	case MainMenu::OptGun:
		_gunFire = !_gunFire;
		break;
	case MainMenu::OptMusic:
		_music = !_music;
		break;
	case MainMenu::OptSound:
		ServiceLocator::GetAudio()->MuteSounds(!ServiceLocator::GetAudio()->IsSoundMuted());
		break;
	case MainMenu::OptDifPansy:
		_difficulty = Pansy;
		UpdateImages();
		break;
	case MainMenu::OptDifWannabe:
		_difficulty = Wannabe;
		UpdateImages();
		break;
	case MainMenu::OptDifHard:
		_difficulty = Hardcore;
		UpdateImages();
		break;
	case MainMenu::OptConstMotion:
		_constantMotion = !_constantMotion;
		break;
	case MainMenu::Exit:
		_gameState = Game::Exiting;
		break;
	default:
		//Clicked on a part of the screen that does nothing - do nothing!
		break;
	}
}

void Game::UpdateImages()
{
/*	//Load smaller paddles and smaller lasers for the player for hardcore gaming
	Paddle* paddle[Game::NUM_PADDLES];
	paddle[0] = dynamic_cast<Paddle*>(GetGameObjectManager().Get("Paddle1"));
	paddle[1] = dynamic_cast<Paddle*>(GetGameObjectManager().Get("Paddle2"));

	for (int i = 0; i < Game::NUM_PADDLES; i++)
	{
		if (NULL != paddle[i])
		{
			if (_difficulty == Hardcore)
			{
				//Smaller is harder for player, and makes the computer more accurate
				paddle[i]->Load("images/paddleSmall.png");
			}
			else
			{
				paddle[i]->Load("images/paddle.png");
			}
		}
	}

	Laser* laser[3];
	//Player users lasers 3, 4, and 5
	laser[0] = dynamic_cast<Laser*>(GetGameObjectManager().Get("Laser3"));
	laser[1] = dynamic_cast<Laser*>(GetGameObjectManager().Get("Laser4"));
	laser[2] = dynamic_cast<Laser*>(GetGameObjectManager().Get("Laser5"));
	for (int i = 0; i < 3; i++)
	{
		if (_difficulty == Hardcore)
		{
			laser[i]->Load("images/LaserSmall.png");
		}
		else
		{
			laser[i]->Load("images/Laser.png");
		}
	}*/
}

//Get events from the game (keyboard inputs)
const sf::Event Game::GetInput()
{
	sf::Event currentEvent;
	_mainWindow.pollEvent(currentEvent);
	return currentEvent;
}

const GameObjectManager& Game::GetGameObjectManager()
{
	return Game::_gameObjectManager;
}

WeaponsManager& Game::GetWeaponsManager()
{
	return Game::_weaponsManager;
}
