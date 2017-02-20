#include "stdafx.h"
#include "Game.h"
#include "BackgroundManager.h"
#include "Cloud.h"


sf::Texture BackgroundManager::_backgroundImage[NUM_BACKGROUNDS];
sf::Sprite BackgroundManager::_background;
sf::Sprite BackgroundManager::_backgroundInv;

const float BackgroundManager::TIME_BETWEEN_CLOUDS = 10;
float BackgroundManager::_cloudTimer = 0.0;
float BackgroundManager::_backgroundImagePixels = Game::SCREEN_HEIGHT;
bool BackgroundManager::_backgroundInverted = false;
int BackgroundManager::SPEED_OF_BACKGROUND_IMAGE = 200;

BackgroundManager::BackgroundManager():_type(LOUISIANA)
{
	LoadTextures();
}
BackgroundManager::~BackgroundManager()
{

}

void BackgroundManager::LoadTextures()
{
	//Problem if no background found
	assert(_backgroundImage[NEW_YORK].loadFromFile("Images/Backgrounds/BG-NewYork.png"));
	assert(_backgroundImage[OAHU].loadFromFile("Images/Backgrounds/BG-Oahu.png"));
	assert(_backgroundImage[LOUISIANA].loadFromFile("Images/Backgrounds/BG-Swamp.png"));
	assert(_backgroundImage[KANSAS].loadFromFile("Images/Backgrounds/BG-Airport.png"));
	assert(_backgroundImage[SIBERIA].loadFromFile("Images/Backgrounds/BG-Siberia.png"));
}

void BackgroundManager::GenerateClouds(const float& timeChange)
{
	if (_cloudTimer <= 0.0)
	{
		//reset the timer
		_cloudTimer = TIME_BETWEEN_CLOUDS;

		//Make a cloud
		Cloud* newCloud = new Cloud(Cloud::Light, 0.7f);

		//Make it a particular size
		//newCloud->GetSprite().setScale()

		//Make it move left to right
		//newCloud->SetXVelocity(newCloud->GetXVelocity() + 5.0f);

		//Set it just off the upper left of the screen
//		newCloud->SetPosition( -newCloud->GetWidth() / 2.0f, -newCloud->GetHeight() / 2.0f );
		//Set cloud at top middle of screen
		float width = Game::SCREEN_WIDTH/2;//-newCloud->GetWidth() / 1.5f;
		float height = 0.0 - newCloud->GetHeight() / 1.5;
		newCloud->SetPosition(width, height);
		//newCloud->SetPosition(Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT);

		Game::GetGameObjectManager().Add(newCloud);
	}
	else
	{
		_cloudTimer -= timeChange;
	}
}

void BackgroundManager::Update(const float& timeChange)
{
	switch(_type)
	{
	case NEW_YORK:
		SPEED_OF_BACKGROUND_IMAGE = 250;
		break;
	case OAHU:
		SPEED_OF_BACKGROUND_IMAGE = 100;
		break;
	case LOUISIANA:
		SPEED_OF_BACKGROUND_IMAGE = 125;
		break;
	case KANSAS:
		SPEED_OF_BACKGROUND_IMAGE = 200;
		break;
	case SIBERIA:
		SPEED_OF_BACKGROUND_IMAGE = 50;
		break;
	}

	//Set textures for the regular and inverted images - we may need to draw both
	_background.setTexture(_backgroundImage[_type]);
	_backgroundInv.setTexture(_backgroundImage[_type]);

	//Location on the background image that we will show this time
	_backgroundImagePixels += (timeChange * SPEED_OF_BACKGROUND_IMAGE);

	//Check that the pixels are still on the screen
	if (_backgroundImagePixels > _backgroundImage[_type].getSize().y + Game::SCREEN_HEIGHT)
	{
		_backgroundImagePixels = Game::SCREEN_HEIGHT;	//Show at least one screen
		_backgroundInverted = !_backgroundInverted;		//Time to invert background
	}


	int _backgroundImagePixelsInt = static_cast<int>(std::round(_backgroundImagePixels));

	//Case screen is fully regular:
	if (_backgroundInverted == false && _backgroundImagePixels <= Game::SCREEN_HEIGHT)
	{
		//Top of the regular image: lowest displayable screen height - the increment that has occured
		int regTop = _backgroundImage[_type].getSize().y - Game::SCREEN_HEIGHT - _backgroundImagePixelsInt;

		_background.setTextureRect(sf::IntRect(0, regTop, Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT));
		_background.setPosition(0, 0);
		Game::GetWindow().draw(_background);
	}
	//Case screen is fully inverted:
	else if (_backgroundInverted == true && _backgroundImagePixels <= Game::SCREEN_HEIGHT)
	{
		//Top of the inverted image just starts at zero and increments by the amount that has occured
		int invTop = 0 + Game::SCREEN_HEIGHT + _backgroundImagePixelsInt;

		_background.setTextureRect(sf::IntRect(0, invTop, Game::SCREEN_WIDTH, -Game::SCREEN_HEIGHT));
		_background.setPosition(0, 0);
		Game::GetWindow().draw(_background);
	}

	//Case screen is partially reg/inverted - coming from normal
	else if (_backgroundInverted == true &&
		_backgroundImagePixels > Game::SCREEN_HEIGHT)
	{
		float invertedPixelHeight = _backgroundImagePixels - Game::SCREEN_HEIGHT;
		float regularPixelHeight = Game::SCREEN_HEIGHT - invertedPixelHeight;
		int invertedPixelHeightInt = static_cast<int>(std::round(invertedPixelHeight));

		float movingVal = _backgroundImage[_type].getSize().y - Game::SCREEN_HEIGHT - _backgroundImagePixels - Game::SCREEN_HEIGHT;

		//DISPLAY PART IMAGES
		//This is the top of the reg image
		_background.setTextureRect(sf::IntRect(0, 0, Game::SCREEN_WIDTH, static_cast<int>(std::round(regularPixelHeight))));	//Stop at the top of the image

		//This is the moving inverted image to make a matching top for the reg image
		_backgroundInv.setTextureRect(sf::IntRect(0, _backgroundImagePixelsInt - Game::SCREEN_HEIGHT, Game::SCREEN_WIDTH, -invertedPixelHeightInt));

		//Set location of each texture so that it draws properly on screen
		_background.setPosition(0, Game::SCREEN_HEIGHT - regularPixelHeight);	//Regular stays at bottom until it disappears
		_backgroundInv.setPosition(0, 0);										//Inv starts where the regular ends and keeps scrolling down until it takes over

		//Draw them
		Game::GetWindow().draw(_background);
		Game::GetWindow().draw(_backgroundInv);
	}

	//Case screen is partially reg/inverted - coming from inverted
	else if (_backgroundInverted == false &&
		_backgroundImagePixels > Game::SCREEN_HEIGHT)
	{
		float regularPixelHeight = _backgroundImagePixels - Game::SCREEN_HEIGHT;
		float invertedPixelHeight = Game::SCREEN_HEIGHT - regularPixelHeight;
		int invertedPixelHeightInt = static_cast<int>(std::round(invertedPixelHeight));
		int regularPixelHeightInt = static_cast<int>(std::round(regularPixelHeight));

		//DISPLAY PART IMAGES
		//This is the top of the reg image, it keeps moving to match the lower inverted image
		_background.setTextureRect(sf::IntRect(0, _backgroundImage[_type].getSize().y - (_backgroundImagePixelsInt - Game::SCREEN_HEIGHT), Game::SCREEN_WIDTH, regularPixelHeightInt));

		//This is the moving inverted image to make a matching top for the reg image
		_backgroundInv.setTextureRect(sf::IntRect(0, _backgroundImage[_type].getSize().y, Game::SCREEN_WIDTH, -invertedPixelHeightInt));		//stop at the top of image

		//Set location of each texture so that it draws properly on screen
		_background.setPosition(0, 0);												//Inverted stays at top until it disappears
		_backgroundInv.setPosition(0, Game::SCREEN_HEIGHT - invertedPixelHeight);	//Regular starts where the inv ends and keeps going down until it takes over

		//Draw them
		Game::GetWindow().draw(_background);
		Game::GetWindow().draw(_backgroundInv);
	}

	GenerateClouds(timeChange);
}

