#pragma once

#include "stdafx.h"

class PlaneSelectScreen
{

public:
	PlaneSelectScreen();

	//Options for the menu selection
	enum MenuResult
	{
		Nothing,
		J31,
		J20,
		T50,
		F22,
		F35,
		MainMenu,
		Exit
	};

	struct MenuItem
	{
	public:
		sf::Rect<int> rect;
		MenuResult action;
	};

	MenuResult Show(sf::RenderWindow& window);

private:
	MenuResult GetMenuResponse(sf::RenderWindow& window);
	MenuResult HandleClick(int x, int y);
	std::list<MenuItem> _menuItems;

	void UpdateBox(sf::RenderWindow& window, sf::Sprite& updatedSprite, bool newVal);
};