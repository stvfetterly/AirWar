#include "stdafx.h"
#include "Cloud.h"
#include "BackgroundManager.h"
#include "Game.h"

int Cloud::totalNumClouds = 0;

const float Cloud::CONTRAIL_DEL_TIME = 0.5f;

Cloud::Cloud() : _type(Big), _opacity(1.0f)
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
	_contrailDeletionTime = CONTRAIL_DEL_TIME;
	_BGmultiplier = 1.1f;

	float start;
	float end;

	if (_type == Big)
	{
		start = 0.0f;
		end = 11.0f;
	}
	else
	{
		start = 12.0f;
		end = 18.0f;
	}

	//Load a random cloud each time created
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist(start, end);
	
	//Use a random rotation each time created
	std::uniform_real_distribution<float> dist2(0.0f, 359.0f);

	switch (static_cast<int>(dist(mt)))
	{
	case 0:
		Load("images/Clouds/Cloud1.png");
		_name = "Cloud";
		break;
	case 1:
		Load("images/Clouds/Cloud2.png");
		_name = "Cloud";
		break;
	case 2:
		Load("images/Clouds/Cloud3.png");
		_name = "Cloud";
		break;
	case 3:
		Load("images/Clouds/Cloud4.png");
		_name = "Cloud";
		break;
	case 4:
		Load("images/Clouds/Cloud5.png");
		_name = "Cloud";
		break;
	case 5:
		Load("images/Clouds/Cloud6.png");
		_name = "Cloud";
		break;
	case 6:
		Load("images/Clouds/Cloud7.png");
		_name = "Cloud";
		break;
	case 7:
		Load("images/Clouds/Cloud8.png");
		_name = "Cloud";
		break;
	case 8:
		Load("images/Clouds/Cloud9.png");
		_name = "Cloud";
		break;
	case 9:
		Load("images/Clouds/Cloud10.png");
		_name = "Cloud";
		break;
	case 10:
		Load("images/Clouds/CloudSM1.png");
		_name = "YContrail";
		break;
	case 11:
		Load("images/Clouds/CloudSM2.png");
		_name = "YContrail";
		break;
	case 12:
		Load("images/Clouds/CloudSM3.png");
		_name = "YContrail";
		break;
	case 13:
		Load("images/Clouds/CloudSM4.png");
		_name = "YContrail";
		break;
	case 14:
		Load("images/Clouds/CloudSM5.png");
		_name = "YContrail";
		break;
	case 15:
		Load("images/Clouds/CloudSM6.png");
		_name = "YContrail";
		break;
	case 16:
		Load("images/Clouds/CloudSM7.png");
		_name = "YContrail";
		break;
	case 17:
		Load("images/Clouds/CloudSM8.png");
		_name = "YContrail";
		break;
	case 18:
		Load("images/Clouds/CloudSM9.png");
		_name = "YContrail";
		break;
	}

	_name += std::to_string(totalNumClouds);
	GetSprite().setRotation(dist2(mt));

	int opacity = static_cast<int>(_opacity * 255.0f);  //255 is completely solid, 0 is completely transparent

	//Set the opacity and darkness of the cloud
	GetSprite().setColor(sf::Color(255, 255, 255, opacity));

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
	if (_type == Big)
	{
		//Clouds match the speed of the background image
		//SetYVelocity((BackgroundManager::SPEED_OF_BACKGROUND_IMAGE / 10) * elapsedTime);
		SetYVelocity(BackgroundManager::SPEED_OF_BACKGROUND_IMAGE / 2.0f);
		//SetXVelocity(30.0f);

		//Update location
		GetSprite().move(GetXVelocity() * elapsedTime, GetYVelocity() * elapsedTime);

		//If the location is well past what's visible, then remove the cloud
		if (GetPosition().x > Game::SCREEN_WIDTH + (GetWidth()) ||
			GetPosition().x < 0.0f - (GetWidth()) ||
			GetPosition().y > Game::SCREEN_HEIGHT + (GetHeight()) ||
			GetPosition().y < 0.0f - (GetWidth()))
		{
			//Queue for deletion
			Game::GetGameObjectManager().AddToDeletionDecorationQueue(_name);
		}
	}
	else
	{
		//If the contrail has lasted as long as necessary, delete it
		if (_contrailDeletionTime <= 0.0f)
		{
			_contrailDeletionTime = CONTRAIL_DEL_TIME;
			Game::GetWeaponsManager().HideObject(this);		//Remove the contrail
		}
		else
		{
			_contrailDeletionTime -= elapsedTime;
		}
	}
}