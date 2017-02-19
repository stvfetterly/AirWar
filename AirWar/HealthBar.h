#pragma once
#include "VisibleGameObject.h"

class HealthBar : public VisibleGameObject
{
public:
	HealthBar();
	HealthBar(std::string name);
	~HealthBar();

	void Update(const float& elapsedTime);
	void Draw(sf::RenderWindow& rw);
	void AddToObjectManager();

private:
	static int totalNumHealthBars;
	void Init();

	float _percentLeft;
};