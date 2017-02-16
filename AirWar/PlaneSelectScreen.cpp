#include "stdafx.h"
#include "PlaneSelectScreen.h"
#include "ServiceLocator.h"
#include "Game.h"

PlaneSelectScreen::PlaneSelectScreen()
{
	//SETUP ALL CLICKABLE REIGONS
	//Play menu item coordinates
	MenuItem J31Select;
	J31Select.rect.top = 0;			//top
	J31Select.rect.height = 767;	//bottom
	J31Select.rect.left = 0;		//left
	J31Select.rect.width = 195;		//right
	J31Select.action = J31;

	//Gunfire Option
	MenuItem J20Select;
	J20Select.rect.top = 0;			//top
	J20Select.rect.height = 767;	//bottom
	J20Select.rect.left = 196;		//left
	J20Select.rect.width = 231;		//right
	J20Select.action = J20;

	//Music Option
	MenuItem T50Select;
	T50Select.rect.top = 0;			//top
	T50Select.rect.height = 767;	//bottom
	T50Select.rect.left = 430;		//left
	T50Select.rect.width = 213;		//right
	T50Select.action = T50;

	//SoundFX Option
	MenuItem F22Select;
	F22Select.rect.top = 0;			//top
	F22Select.rect.height = 767;	//bottom
	F22Select.rect.left = 644;		//left
	F22Select.rect.width = 196;		//right
	F22Select.action = F22;

	//Pansy Option
	MenuItem F35Select;
	F35Select.rect.top = 0;			//top
	F35Select.rect.height = 767;	//bottom
	F35Select.rect.left = 841;		//left
	F35Select.rect.width = 182;		//right
	F35Select.action = F35;

	//Add so that they'll be checked when user clicks a button later
	_menuItems.push_back(J31Select);
	_menuItems.push_back(J20Select);
	_menuItems.push_back(T50Select);
	_menuItems.push_back(F22Select);
	_menuItems.push_back(F35Select);
}



//Show the main menu image, set up the clickable coordinates for option selection
PlaneSelectScreen::MenuResult PlaneSelectScreen::Show(sf::RenderWindow& window)
{
	//Load menu image from file
	sf::Texture image;
	image.loadFromFile("images/menu/PlaneSelectMenu.png");
	sf::Sprite sprite(image);

	//Draw the plane select menu
	window.draw(sprite);
	window.display();

	return GetMenuResponse(window);
}

//
PlaneSelectScreen::MenuResult PlaneSelectScreen::HandleClick(int x, int y)
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

PlaneSelectScreen::MenuResult  PlaneSelectScreen::GetMenuResponse(sf::RenderWindow& window)
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
				//Hitting escape will back out to the previous menu
				return MainMenu;
			}
			if (menuEvent.type == sf::Event::Closed)
			{
				return Exit;
			}
		}
	}
}