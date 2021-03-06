#include "stdafx.h"
#include "MainMenuScreen.h"
#include "ServiceLocator.h"
#include "Game.h"

MainMenu::MainMenu()
{
	//Load the checked and unchecked image for use by checkboxes
	//(Big problem if they're not found)
	assert(_checkedImage.loadFromFile("images/menu/checkChecked.png"));
	assert(_unCheckedImage.loadFromFile("images/menu/checkEmpty.png"));

	//Set position of all checkboxes
	_musicBox.setPosition(90, 235);
	_soundBox.setPosition(90, 300);
	_pansyBox.setPosition(155, 425);
	_wannabeBox.setPosition(155, 480);
	_hardcoreBox.setPosition(155, 530);

	//SETUP ALL CLICKABLE REIGONS
	//Play menu item coordinates
	MenuItem playButton;
	playButton.rect.top = 20;		//top
	playButton.rect.height = 60;	//bottom
	playButton.rect.left = 60;		//left
	playButton.rect.width = 240;	//right
	playButton.action = Play;

	//Music Option
	MenuItem optMusic;
	optMusic.rect.top = 230;		//top
	optMusic.rect.height = 50;		//bottom
	optMusic.rect.left = 90;		//left
	optMusic.rect.width = 225;		//right
	optMusic.action = OptMusic;

	//SoundFX Option
	MenuItem optSound;
	optSound.rect.top = 295;		//top
	optSound.rect.height = 50;		//bottom
	optSound.rect.left = 90;		//left
	optSound.rect.width = 250;		//right
	optSound.action = OptSound;

	//Pansy Option
	MenuItem optPansy;
	optPansy.rect.top = 420;		//top
	optPansy.rect.height = 45;		//bottom
	optPansy.rect.left = 155;		//left
	optPansy.rect.width = 210;		//right
	optPansy.action = OptDifPansy;

	//Wannabe Option
	MenuItem optWannabe;
	optWannabe.rect.top = 475;		//top
	optWannabe.rect.height = 45;	//bottom
	optWannabe.rect.left = 155;		//left
	optWannabe.rect.width = 275;	//right
	optWannabe.action = OptDifWannabe;

	//HardCore Option
	MenuItem optHard;
	optHard.rect.top = 525;			//top
	optHard.rect.height = 45;		//bottom
	optHard.rect.left = 155;		//left
	optHard.rect.width = 275;		//right
	optHard.action = OptDifHard;

	//Exit menu item coordinates
	MenuItem exitButton;
	exitButton.rect.left = 70;		//left
	exitButton.rect.width = 100;	//right
	exitButton.rect.top = 680;		//top
	exitButton.rect.height = 60;	//bottom
	exitButton.action = Exit;

	//Add so that they'll be checked when user clicks a button later
	_menuItems.push_back(playButton);
	_menuItems.push_back(optMusic);
	_menuItems.push_back(optSound);
	_menuItems.push_back(optPansy);
	_menuItems.push_back(optWannabe);
	_menuItems.push_back(optHard);
	_menuItems.push_back(exitButton);
}

void MainMenu::UpdateBox(sf::RenderWindow& window, sf::Sprite& updateSprite, bool newVal)
{
	if (newVal == true)
	{
		updateSprite.setTexture(_checkedImage, false);
	}
	else
	{
		updateSprite.setTexture(_unCheckedImage, false);
	}
}



//Show the main menu image, set up the clickable coordinates for option selection
MainMenu::MenuResult MainMenu::Show(sf::RenderWindow& window)
{
	//Clears the window before trying to draw stuff
	window.clear(sf::Color(0, 0, 0));

	//Load menu image from file
	sf::Texture image;
	assert(image.loadFromFile("images/Menu/mainmenu.png"));
	//image.loadFromFile("Images/Backgrounds/TestBackground.png");
	sf::Sprite sprite(image);

	//Load checkboxes
	UpdateBox(window, _musicBox, Game::_music);
	UpdateBox(window, _soundBox, !ServiceLocator::GetAudio()->IsSoundMuted());
	UpdateBox(window, _pansyBox, Game::Pansy == Game::_difficulty);
	UpdateBox(window, _wannabeBox, Game::Wannabe == Game::_difficulty);
	UpdateBox(window, _hardcoreBox, Game::Hardcore == Game::_difficulty);

	//Draw the whole menu
	window.draw(sprite);
	window.draw(_musicBox);
	window.draw(_soundBox);
	window.draw(_pansyBox);
	window.draw(_wannabeBox);
	window.draw(_hardcoreBox);
	window.display();

	 return GetMenuResponse(window);
}

//
MainMenu::MenuResult MainMenu::HandleClick(int x, int y)
{
	std::list<MenuItem>::iterator it;

	//Check if one of the menu items has been clicked
	for (it = _menuItems.begin(); it != _menuItems.end(); it++)
	{
		sf::Rect<int> menuItemRect = (*it).rect;
		if (menuItemRect.height + menuItemRect.top > y			&&	//bottom
			menuItemRect.top < y								&&	//top
			menuItemRect.left < x								&&  //left
			menuItemRect.left + menuItemRect.width > x)				//right
		{
			//Play selection sound
			ServiceLocator::GetAudio()->PlaySound("sounds/menu/Click.wav");

			return (*it).action;
		}
	}

	return Nothing;
}

MainMenu::MenuResult  MainMenu::GetMenuResponse(sf::RenderWindow& window)
{
	sf::Event menuEvent;

	while (true)
	{
		//Loop forever, checking for a mouse button pressed or window closed event
		while (window.pollEvent(menuEvent))
		{
			if (menuEvent.type == sf::Event::MouseButtonPressed)
			{
				return HandleClick(menuEvent.mouseButton.x, menuEvent.mouseButton.y);
			}
			if (menuEvent.type == sf::Event::KeyPressed &&
				menuEvent.key.code == sf::Keyboard::Escape)
			{
				//Hitting escape will back out to the splash screen
				return Splash;
			}
			if (menuEvent.type == sf::Event::Closed)
			{
				return Exit;
			}
		}
	}
}