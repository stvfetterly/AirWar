#pragma once
#include "stdafx.h"

class VisibleGameObject
{
public:
	VisibleGameObject();
	virtual ~VisibleGameObject();

	virtual void Load(const std::string& filename);
	virtual void Draw(sf::RenderWindow& window);
	virtual void Update(const float& elapsedTime);
	virtual void SetPosition(const float& x, const float& y);

	virtual bool IsLoaded() const;

	virtual float GetWidth() const;
	virtual float GetHeight() const;
	virtual sf::Rect<float> GetBoundingRect() const;
	virtual sf::Vector2f GetPosition() const;
	virtual std::string GetName() const { return _name; }
	virtual void SetName(const std::string& name) { _name = name; }

	virtual bool IsPaused() const { return _isPaused; }
	virtual void Pause(bool newVal) { _isPaused = newVal; }

	virtual float GetXVelocity() { return _xVelocity; }
	virtual void SetXVelocity(const float& newVelocity) { _xVelocity = newVelocity; }
	virtual float GetYVelocity() { return _yVelocity; }
	virtual void SetYVelocity(const float& newVelocity) { _yVelocity = newVelocity; }

	bool IsVisible() { return _visible; }
	void SetVisible(bool visible) { _visible = visible; }
	

protected:
	sf::Sprite& GetSprite();
	bool _isPaused;
	std::string _name;			//Name of object

	float _xVelocity;		//positive = right, negative = left
	float _yVelocity;		//positive = down, negative = up


private:
	static int numObjects;

	sf::Sprite _sprite;
	sf::Texture _image;
	std::string _filename;
	bool _isLoaded;
	bool _visible;
};
