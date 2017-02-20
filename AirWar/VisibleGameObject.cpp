#include "stdafx.h"
#include "VisibleGameObject.h"
#include "Game.h"

int VisibleGameObject::numObjects = 0;

//Default constructor - initialize variables
VisibleGameObject::VisibleGameObject() : _isLoaded(false), _isPaused(false), _xVelocity(0.0), _yVelocity(0.0), _visible(true), _image(NULL)
{
	_name = "VisibleGameObject" + std::to_string(numObjects);
	numObjects++;
}

//Destructor
VisibleGameObject::~VisibleGameObject()
{
}

void VisibleGameObject::Load(const std::string& filename)
{
/*	//Now that we know an image exists in the texture map, point to it and use it
	_image = &(Game::GetGameObjectManager().GetObjectTextureMap()[filename]);
	assert(_image != NULL);	//if we're trying to load an image that doesn't exist, we've got a big problem
	_sprite.setTexture(*_image, true);
	_isLoaded = true;
	*/

	//If a texture doesn't exist then load from file
	if (Game::GetGameObjectManager().GetObjectTextureMap().count(filename) == 0)
	{
		sf::Texture image;

		//if we fail to load the image, reset variables
		if (image.loadFromFile(filename) == false)
		{
			_isLoaded = false;
			return;
		}
		else //Saves image into texture map
		{
			Game::GetGameObjectManager().GetObjectTextureMap().insert(std::pair<std::string, sf::Texture>(filename, image));
		}
	}
	_image = &Game::GetGameObjectManager().GetObjectTextureMap()[filename];
	_sprite.setTexture(*_image, true);
	_isLoaded = true;
}

void VisibleGameObject::Draw(sf::RenderWindow& renderWindow)
{
	//Draw an image if we've loaded anything
	if (_isLoaded)
	{
		renderWindow.draw(_sprite);
	}
}

void VisibleGameObject::Update(const float& elapsedTime)
{

}

void VisibleGameObject::SetPosition(const float& x, const float& y)
{
	if (_isLoaded)
	{
		_sprite.setPosition(x, y);
	}
}

//Returns the position if the object has been loaded, otherwise returns empty vector
sf::Vector2f VisibleGameObject::GetPosition() const
{
	if (_isLoaded)
	{
		return _sprite.getPosition();
	}
	return sf::Vector2f();
}

sf::Sprite& VisibleGameObject::GetSprite()
{
	return _sprite;
}

bool VisibleGameObject::IsLoaded() const
{
	return _isLoaded;
}

float VisibleGameObject::GetHeight() const
{
	return _sprite.getGlobalBounds().height;
}

float VisibleGameObject::GetWidth() const
{
	return _sprite.getGlobalBounds().width;
}

//Returns a rectangle that contains the object
sf::Rect<float> VisibleGameObject::GetBoundingRect() const
{
	return _sprite.getGlobalBounds();
}

float VisibleGameObject::GetAngleInDegrees()
{
	//Set correct angle algorithm
	static float theta = 0;

	//If the object is heading up then just use normal sin
	if (_yVelocity <= 0)
	{
		float hypotenuse = std::sqrtf(_xVelocity*_xVelocity + _yVelocity*_yVelocity);


		//Check if object has stopped moving
		if (hypotenuse == 0)
		{
			//Do nothing to rotation
			return theta;
		}
		else
		{
			float value = _xVelocity / hypotenuse;
			theta = static_cast<float>(std::asinf(value) * (180.0f / M_PI));		//Convert back to degrees
		}
	}
	else
		//Object is heading down, so to get correct angle of rotation we need to subtract it from 180
	{
		float hypotenuse = std::sqrtf(_xVelocity*_xVelocity + _yVelocity*_yVelocity);

		if (hypotenuse == 0)
		{
			//Do nothing to rotation, the object is not moving
			return theta;
		}
		else
		{
			float value = _xVelocity / hypotenuse;
			theta = static_cast<float>(std::asinf(value) * (180.0f / M_PI));		//Convert back to degrees
		}

		theta = 180 - theta;
	}

	return theta;
}

void VisibleGameObject::UpdateDirection()
{
	GetSprite().setRotation(GetAngleInDegrees());
}