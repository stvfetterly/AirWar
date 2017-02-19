#include "stdafx.h"
#include "VisibleGameObject.h"
#include "Game.h"

int VisibleGameObject::numObjects = 0;

//Default constructor - initialize variables
VisibleGameObject::VisibleGameObject() : _isLoaded(false), _filename(""), _isPaused(false), _xVelocity(0.0), _yVelocity(0.0), _visible(true)
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
	//If a texture doesn't exist then load from file
	if (Game::GetGameObjectManager().GetObjectTextureMap().count(filename) == 0)
	{
		//if we fail to load the image, reset variables
		if (_image.loadFromFile(filename) == false)
		{
			_filename = "";
			_isLoaded = false;
			return;
		}
		else //Loads the image
		{
			Game::GetGameObjectManager().GetObjectTextureMap().insert(std::pair<std::string, sf::Texture>(_filename, _image));
			_sprite.setTexture(_image, true);
		}
	}
	else
	{
		_image = Game::GetGameObjectManager().GetObjectTextureMap()[filename];
		_sprite.setTexture(_image, true);
	}

	_filename = filename;
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
	return _sprite.getLocalBounds().height;
}

float VisibleGameObject::GetWidth() const
{
	return _sprite.getLocalBounds().width;
}

//Returns a rectangle that contains the object
sf::Rect<float> VisibleGameObject::GetBoundingRect() const
{
/*	sf::Vector2f size;
	size.x = GetWidth();
	size.y = GetHeight();

	sf::Vector2f centerPos;
	centerPos.x = _sprite.getOrigin().x;
	centerPos.y = _sprite.getOrigin().y;

	return sf::Rect<float>( centerPos.x - size.x/2,
							centerPos.y - size.x/2,
							centerPos.x + size.x/2,
							centerPos.y + size.y/2);
							**/

	return _sprite.getGlobalBounds();
}

void VisibleGameObject::UpdateDirection()
{
	sf::Sprite& thisSprite = GetSprite();

	//Set correct angle algorithm
	float theta = 0;

	//If the object is heading up then just use normal sin
	if (_yVelocity <= 0)
	{
		float hypotenuse = std::sqrtf(_xVelocity*_xVelocity + _yVelocity*_yVelocity);


		//Check if object has stopped moving
		if (hypotenuse == 0)
		{
			//Do nothing to rotation
			return;
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
			return;
		}
		else
		{
			float value = _xVelocity / hypotenuse;
			theta = static_cast<float>(std::asinf(value) * (180.0f / M_PI));		//Convert back to degrees
		}

		theta = 180 - theta;
	}
	thisSprite.setRotation(theta);
}