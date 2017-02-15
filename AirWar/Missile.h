#pragma once

#include "VisibleGameObject.h"

class Missile : public VisibleGameObject
{
public:
	static int numMissiles;
	Missile(float xVel, float yVel);
	~Missile();

	void Update(const float& elapsedTime);
	void Draw(sf::RenderWindow& rw);
//	float GetVelocity() { return _velocity; }
//	void SetVelocity(const float& newVelocity) { _velocity = newVelocity; }

private:
	float _xVelocity;      //positive = down, negative = up
	float _yVelocity;

};