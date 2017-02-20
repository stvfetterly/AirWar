#include "stdafx.h"
#include "Cloud.h"
#include "BackgroundManager.h"
#include "Game.h"

int Cloud::totalNumClouds = 0;

Cloud::Cloud() : _type(Light), _opacity(1.0f)
{
	Init();
}

Cloud::Cloud(CloudType type, float _opacity) : _type(type), _opacity(_opacity)
{
	Init();
}

void Cloud::Init()
{
	totalNumClouds++;
	_name = "Cloud" + std::to_string(totalNumClouds);

	_BGmultiplier = 1.1f;

	//Load a random cloud each time created
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist(0.0f, 11.0f);
	
	//Use a random rotation each time created
	std::uniform_real_distribution<float> dist2(0.0f, 359.0f);

	switch (static_cast<int>(dist(mt)))
	{
	case 0:
		Load("images/Clouds/Cloud1.png");
		break;
	case 1:
		Load("images/Clouds/Cloud2.png");
		break;
	case 2:
		Load("images/Clouds/Cloud3.png");
		break;
	case 3:
		Load("images/Clouds/Cloud4.png");
		break;
	case 4:
		Load("images/Clouds/Cloud5.png");
		break;
	case 5:
		Load("images/Clouds/Cloud6.png");
		break;
	case 6:
		Load("images/Clouds/Cloud7.png");
		break;
	case 7:
		Load("images/Clouds/Cloud8.png");
		break;
	case 8:
		Load("images/Clouds/Cloud9.png");
		break;
	case 9:
		Load("images/Clouds/Cloud10.png");
		break;
	case 10:
		Load("images/Clouds/Cloud11.png");
		break;
	}

	GetSprite().setRotation(dist2(mt));

	int opacity = static_cast<int>(_opacity * 255.0f);  //255 is completely solid, 0 is completely transparent
	int darkness = 255;

	switch (_type)
	{
	case Dark:
		darkness = static_cast<int>(darkness * 0.6f);
		break;
	case Med:
		darkness = static_cast<int>(darkness * 0.8f);
		break;
	case Light:
		break;
	}

	//Set the opacity and darkness of the cloud
	GetSprite().setColor(sf::Color(darkness, darkness, darkness, opacity));

	//Set sprite origin
	GetSprite().setOrigin(GetSprite().getGlobalBounds().width / 2,
		GetSprite().getGlobalBounds().height / 2);
}

Cloud::~Cloud()
{

}

void Cloud::Draw(sf::RenderWindow& rw)
{
	VisibleGameObject::Draw(rw);
}

void Cloud::Update(const float& elapsedTime)
{
	//Clouds match the speed of the background image
	//SetYVelocity((BackgroundManager::SPEED_OF_BACKGROUND_IMAGE / 10) * elapsedTime);
	SetYVelocity(BackgroundManager::SPEED_OF_BACKGROUND_IMAGE / 2.0f);
	//SetXVelocity(30.0f);

	//Update location
	GetSprite().move(GetXVelocity() * elapsedTime, GetYVelocity() * elapsedTime);

	//If the location is well past what's visible, then remove the cloud
	if ( GetPosition().x > Game::SCREEN_WIDTH + (GetWidth())		||
		 GetPosition().x < 0.0f - (GetWidth() )						||
		 GetPosition().y > Game::SCREEN_HEIGHT + (GetHeight() )		||
		 GetPosition().y < 0.0f - (GetWidth() )						)
	{
		//Queue for deletion
		Game::GetGameObjectManager().AddToDeletionQueue(_name);
	}
}