#include "stdafx.h"
#include "HealthBar.h"
#include "Game.h"
#include "AircraftPlayer.h"

int HealthBar::totalNumHealthBars = 0;

HealthBar::HealthBar():_percentLeft(1.0f)
{
	Init();
	_name = "HealthBar" + std::to_string(totalNumHealthBars);
}

HealthBar::HealthBar(std::string name) :_percentLeft(1.0f)
{
	_name = name;
	Init();
}

void HealthBar::Init()
{
	totalNumHealthBars++;

	Load("images/Icons/HealthBar.png");

	//Set sprite origin
	GetSprite().setOrigin(GetSprite().getGlobalBounds().width / 2,
		GetSprite().getGlobalBounds().height / 2);

	//Set transparency to 50%
	GetSprite().setColor(sf::Color(255, 255, 255, 128));
}

HealthBar::~HealthBar()
{

}

void HealthBar::AddToObjectManager()
{
	//If the health bar doesn't exist, then add it
	if (Game::GetGameObjectManager().Get(_name) == NULL)
	{
		Game::GetGameObjectManager().Add(this);
	}
	else
	{
		//Bar already exists, remove the old one add this new one
		Game::GetGameObjectManager().Remove(_name);
		Game::GetGameObjectManager().Add(this);
	}
}

void HealthBar::Update(const float& elapsedTime)
{
	//Get the player aircraft
	AircraftPlayer* player = dynamic_cast<AircraftPlayer*>(Game::GetGameObjectManager().Get("ZPlayer"));

	if (NULL != player)
	{
		float newPercent = player->GetHealth() / player->GetFullHealth();

		//If a change has happened, update the health bar
		if (newPercent != _percentLeft)
		{
			_percentLeft = newPercent;

			//Update size of health bar

			//Height and top stay the same
			int height = static_cast<int>(GetHeight());
			int top = static_cast<int>(GetPosition().y - height);

			//Width changes by the percentage of health left
			int width = static_cast<int>(GetSprite().getTextureRect().width * _percentLeft);

			//Right hand position is the center of the sprite + half the width of the texture
			//int right = GetSprite().getPosition().x + (GetSprite().getTextureRect().width / 2);

			//Left position updates with every hit.  It is the width - right hand position
			//int left = static_cast<int>(width - GetSprite().getPosition().x + (GetSprite().getTextureRect().width / 2));
			int left = static_cast<int>(GetSprite().getPosition().x - (GetSprite().getTextureRect().width) + ((GetSprite().getTextureRect().width - width)/2));

			//Update the texture so that the more damage you get the less green is visible
			GetSprite().setTextureRect( sf::IntRect(left, top, width, height) );
		}
	}
}

void HealthBar::Draw(sf::RenderWindow& rw)
{
	VisibleGameObject::Draw(rw);
}